// uart3_link.c

#include "uart3_link.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UART3_LINK_LINE_MAX
#define UART3_LINK_LINE_MAX 96
#endif

static UART_HandleTypeDef *s_huart = NULL;

static uint8_t  s_rx_ch;

// working buffer (ISR fills)
static char     s_buf[UART3_LINK_LINE_MAX];
static uint32_t s_idx = 0;
static bool     s_overflow_drop = false;

// completed line buffer (hand-off)
static char     s_line[UART3_LINK_LINE_MAX];
static volatile bool s_has_line = false;

static inline void arm_rx(void)
{
    if (!s_huart) return;

    // If HAL is busy, we just don't re-arm (rare). You can add a retry elsewhere if needed.
    (void)HAL_UART_Receive_IT(s_huart, &s_rx_ch, 1);
}

void UART3_Link_Init(UART_HandleTypeDef *huart)
{
    s_huart = huart;

    s_idx = 0;
    s_overflow_drop = false;
    memset(s_buf, 0, sizeof(s_buf));
    memset(s_line, 0, sizeof(s_line));
    s_has_line = false;

    arm_rx();
}

void UART3_Link_OnRxByteIRQ(UART_HandleTypeDef *huart)
{
    if (!s_huart || !huart) return;
    if (huart->Instance != s_huart->Instance) return;

    char c = (char)s_rx_ch;

    // End-of-line
    if (c == '\n' || c == '\r')
    {
        if (!s_overflow_drop && s_idx > 0)
        {
            // terminate
            if (s_idx >= UART3_LINK_LINE_MAX) s_idx = UART3_LINK_LINE_MAX - 1;
            s_buf[s_idx] = '\0';

            // copy to output buffer first, then set flag (ordering matters)
            memcpy(s_line, s_buf, UART3_LINK_LINE_MAX);
            s_line[UART3_LINK_LINE_MAX - 1] = '\0';
            s_has_line = true;
        }

        // reset for next line (also exit overflow-drop mode)
        s_idx = 0;
        s_overflow_drop = false;
    }
    else
    {
        if (s_overflow_drop)
        {
            // drop until newline
        }
        else if (s_idx < (UART3_LINK_LINE_MAX - 1))
        {
            s_buf[s_idx++] = c;
        }
        else
        {
            // buffer full -> start dropping until EOL
            s_overflow_drop = true;
        }
    }

    arm_rx();
}

bool UART3_Link_GetLine(char *dst, uint32_t dst_len)
{
    if (!dst || dst_len == 0) return false;
    if (!s_has_line) return false;

    // Make the read atomic wrt ISR write:
    // Option 1: briefly disable UART IRQ (simple + robust)
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    // re-check after disabling (race)
    if (!s_has_line)
    {
        if (!primask) __enable_irq();
        return false;
    }

    strncpy(dst, s_line, dst_len - 1);
    dst[dst_len - 1] = '\0';
    s_has_line = false;

    if (!primask) __enable_irq();
    return true;
}

void UART3_Link_OnUartErrorIRQ(UART_HandleTypeDef *huart)
{
    if (!s_huart || !huart) return;
    if (huart->Instance != s_huart->Instance) return;

    // Clear common UART error flags (HAL typically handles some, but be explicit)
    __HAL_UART_CLEAR_OREFLAG(s_huart);
    __HAL_UART_CLEAR_FEFLAG(s_huart);
    __HAL_UART_CLEAR_NEFLAG(s_huart);
    __HAL_UART_CLEAR_PEFLAG(s_huart);

    // Restart RX
    (void)HAL_UART_AbortReceive_IT(s_huart);
    arm_rx();
}
