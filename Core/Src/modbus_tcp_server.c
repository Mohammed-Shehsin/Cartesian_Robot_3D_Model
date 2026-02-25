#include "modbus_tcp_server.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include <string.h>
#include <stdio.h>
#include "freertos_user.h"
#include "cmsis_os.h"
#include "modbus_regs.h"

// PLC window
volatile uint16_t mb_cmd[MB_WINDOW_LEN];
volatile uint16_t mb_fb[MB_WINDOW_LEN];

#define MB_TCP_RXBUF_SIZE 512
#define MB_TCP_TXBUF_SIZE 260

static uint16_t be16(const uint8_t *p) {
	return (uint16_t) ((uint16_t) p[0] << 8) | (uint16_t) p[1];
}
static void put16(uint8_t *p, uint16_t v) {
	p[0] = (uint8_t) (v >> 8);
	p[1] = (uint8_t) (v & 0xFF);
}

static uint16_t build_exception(uint8_t *tx, uint16_t trans, uint8_t unit,
		uint8_t func, uint8_t excode) {
	put16(&tx[0], trans);
	put16(&tx[2], 0x0000);
	put16(&tx[4], 0x0003);  // Unit + Func + Ex
	tx[6] = unit;
	tx[7] = (uint8_t) (func | 0x80);
	tx[8] = excode;
	return 9;
}

static int map_addr_to_index(uint16_t addr, uint16_t qty, uint16_t *outIndex) {
	if (addr < MODBUS_ADDR_BASE)
		return 0;
	uint32_t idx = (uint32_t) (addr - MODBUS_ADDR_BASE);

	if (qty == 0)
		return 0;
	if ((idx + qty) > MB_WINDOW_LEN)
		return 0;

	*outIndex = (uint16_t) idx;
	return 1;
}

static uint16_t handle_modbus_tcp(const uint8_t *rx, uint16_t rxLen,
		uint8_t *tx, uint16_t txMax) {
	if (rxLen < 8)
		return 0;

	uint16_t trans = be16(&rx[0]);
	uint16_t proto = be16(&rx[2]);
	uint16_t mbLen = be16(&rx[4]);
	uint8_t unit = rx[6];
	(void) mbLen;

	if (proto != 0)
		return 0;

	uint8_t func = rx[7];

	// -------- FC03: READ -> return FEEDBACK window mb_fb[]
	if (func == 0x03) {
		if (rxLen < 12)
			return 0;
		uint16_t addr = be16(&rx[8]);
		uint16_t qty = be16(&rx[10]);

		if (qty == 0 || qty > 125)
			return build_exception(tx, trans, unit, func, 0x03);

		uint16_t idx = 0;
		if (!map_addr_to_index(addr, qty, &idx))
			return build_exception(tx, trans, unit, func, 0x02);

		uint16_t pduLen = (uint16_t) (2 + (qty * 2));  // func + bc + data
		uint16_t fullLen = (uint16_t) (7 + pduLen);
		if (fullLen > txMax)
			return 0;

		put16(&tx[0], trans);
		put16(&tx[2], 0x0000);
		put16(&tx[4], (uint16_t) (1 + pduLen)); // Unit + PDU
		tx[6] = unit;

		tx[7] = 0x03;
		tx[8] = (uint8_t) (qty * 2);

		uint16_t out = 9;
		for (uint16_t i = 0; i < qty; i++) {
			put16(&tx[out], mb_fb[idx + i]);
			out += 2;
		}
		return out;
	}

	// -------- FC06: WRITE SINGLE -> store into COMMAND window mb_cmd[]
	else if (func == 0x06) {
		if (rxLen < 12)
			return 0;

		uint16_t addr = be16(&rx[8]);
		uint16_t val = be16(&rx[10]);

		uint16_t idx = 0;
		if (!map_addr_to_index(addr, 1, &idx))
			return build_exception(tx, trans, unit, func, 0x02);

		mb_cmd[idx] = val;

		put16(&tx[0], trans);
		put16(&tx[2], 0x0000);
		put16(&tx[4], 0x0006);
		tx[6] = unit;

		tx[7] = 0x06;
		put16(&tx[8], addr);
		put16(&tx[10], val);

		return 12;
	}

	// -------- FC10: WRITE MULTI -> store into COMMAND window mb_cmd[]
	else if (func == 0x10) {
		if (rxLen < 13)
			return 0;

		uint16_t addr = be16(&rx[8]);
		uint16_t qty = be16(&rx[10]);
		uint8_t bc = rx[12];

		if (qty == 0 || qty > 123)
			return build_exception(tx, trans, unit, func, 0x03);

		if (bc != (uint8_t) (qty * 2))
			return build_exception(tx, trans, unit, func, 0x03);

		if (rxLen < (uint16_t) (13 + bc))
			return 0;

		uint16_t idx = 0;
		if (!map_addr_to_index(addr, qty, &idx))
			return build_exception(tx, trans, unit, func, 0x02);

		uint16_t in = 13;
		for (uint16_t i = 0; i < qty; i++) {
			mb_cmd[idx + i] = be16(&rx[in]);
			in += 2;
		}

		// Debug (optional)
//        printf("CMD WR idx=%u qty=%u CMD=%u X=%u Y=%u Z=%u DIR=0x%04X\r\n",
//               idx, qty,
//               mb_cmd[0], mb_cmd[1], mb_cmd[2], mb_cmd[3], mb_cmd[4]);

		put16(&tx[0], trans);
		put16(&tx[2], 0x0000);
		put16(&tx[4], 0x0006);
		tx[6] = unit;

		tx[7] = 0x10;
		put16(&tx[8], addr);
		put16(&tx[10], qty);

		return 12;
	}

	return build_exception(tx, trans, unit, func, 0x01);
}

static uint16_t netbuf_copy_all(struct netbuf *buf, uint8_t *dst,
		uint16_t dstMax) {
	uint16_t total = 0;
	do {
		void *data;
		u16_t len;
		netbuf_data(buf, &data, &len);
		if (len == 0)
			continue;

		uint16_t canCopy = len;
		if ((uint32_t) total + canCopy > dstMax)
			canCopy = (uint16_t) (dstMax - total);

		if (canCopy > 0) {
			memcpy(&dst[total], data, canCopy);
			total += canCopy;
		}
		if (total >= dstMax)
			break;
	} while (netbuf_next(buf) >= 0);

	return total;
}

void ModbusTcpServer_Task(void const *argument) {
	(void) argument;

	osSemaphoreWait(lwipReadySem, osWaitForever);
	osDelay(50);

	memset((void*) mb_cmd, 0, sizeof(mb_cmd));
	memset((void*) mb_fb, 0, sizeof(mb_fb));

	struct netconn *listener = netconn_new(NETCONN_TCP);
	if (!listener) {
		for (;;)
			sys_msleep(1000);
	}

	netconn_bind(listener, IP_ADDR_ANY, 502);
	netconn_listen(listener);

	for (;;) {

		struct netconn *client = NULL;

		if (netconn_accept(listener, &client) == ERR_OK) {
			uint8_t rxbuf[MB_TCP_RXBUF_SIZE];
			uint16_t rxcnt = 0;

			struct netbuf *buf;

			while (netconn_recv(client, &buf) == ERR_OK) {
				uint8_t tmp[MB_TCP_RXBUF_SIZE];
				uint16_t got = netbuf_copy_all(buf, tmp, sizeof(tmp));
				netbuf_delete(buf);
				if (got == 0)
					continue;

				if ((uint32_t) rxcnt + got <= sizeof(rxbuf)) {
					memcpy(&rxbuf[rxcnt], tmp, got);
					rxcnt += got;
				} else {
					rxcnt = 0;
					continue;
				}

				while (rxcnt >= 8) {
					uint16_t mb_len = be16(&rxbuf[4]);
					uint16_t frame_len = (uint16_t) (6 + mb_len);

					if (frame_len < 8 || frame_len > sizeof(rxbuf)) {
						rxcnt = 0;
						break;
					}
					if (rxcnt < frame_len)
						break;

					uint8_t tx[MB_TCP_TXBUF_SIZE];
					uint16_t outLen = handle_modbus_tcp(rxbuf, frame_len, tx,
							sizeof(tx));
					if (outLen > 0)
						netconn_write(client, tx, outLen, NETCONN_COPY);

					uint16_t remaining = (uint16_t) (rxcnt - frame_len);
					if (remaining > 0)
						memmove(rxbuf, &rxbuf[frame_len], remaining);
					rxcnt = remaining;
				}
			}

			netconn_close(client);
			netconn_delete(client);
		}

		sys_msleep(10);
	}
}
