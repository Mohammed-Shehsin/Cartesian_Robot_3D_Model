/*
 * uart3_link.h
 *
 *  Created on: Feb 19, 2026
 *      Author: moham
 */

#ifndef INC_UART3_LINK_H_
#define INC_UART3_LINK_H_

#include "stm32f7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Call once after MX_USART3_UART_Init()
void UART3_Link_Init(UART_HandleTypeDef *huart);

// Call this from HAL_UART_RxCpltCallback()
void UART3_Link_OnRxByteIRQ(UART_HandleTypeDef *huart);

// Returns true if a NEW value arrived since last read
bool UART3_Link_GetLatestInt(int32_t *out_value);

// Optional: access raw last line (for debugging)
const char* UART3_Link_GetLastLine(void);

void UART3_Link_OnUartErrorIRQ(UART_HandleTypeDef *huart);

bool UART3_Link_GetLine(char *dst, uint32_t dst_len);


#ifdef __cplusplus
}
#endif

#endif /* INC_UART3_LINK_H_ */
