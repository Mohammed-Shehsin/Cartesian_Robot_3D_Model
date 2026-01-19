/*
 * debug_uart.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */
#include "debug_uart.h"

static UART_HandleTypeDef *s_huart = NULL;

void DebugUart_Init(UART_HandleTypeDef *huart) {
    s_huart = huart;
}

int _write(int file, char *ptr, int len) {
    (void)file;
    if (!s_huart) return 0;
    HAL_UART_Transmit(s_huart, (uint8_t*)ptr, (uint16_t)len, HAL_MAX_DELAY);
    return len;
}




