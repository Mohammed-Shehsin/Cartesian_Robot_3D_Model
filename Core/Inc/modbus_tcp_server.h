/*
 * modbus_tcp_server.h
 *
 *  Created on: Jan 21, 2026
 *      Author: moham
 */

#ifndef INC_MODBUS_TCP_SERVER_H_
#define INC_MODBUS_TCP_SERVER_H_

#pragma once
#include <stdint.h>
#include "app_hw.h"

#define MODBUS_REG_COUNT 64

extern volatile uint16_t holdingRegs[MODBUS_REG_COUNT];


void ModbusTcpServer_Task(void const *argument);


#endif /* INC_MODBUS_TCP_SERVER_H_ */
