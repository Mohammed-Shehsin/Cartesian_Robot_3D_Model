/*
 * debug_uart.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

#pragma once
#include "main.h"
#include <stdint.h>

void DebugUart_Init(UART_HandleTypeDef *huart);
int _write(int file, char *ptr, int len);


#endif /* INC_DEBUG_UART_H_ */
