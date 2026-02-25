/*
 * freertos_user.h
 *
 *  Created on: Feb 4, 2026
 *      Author: moham
 */

#ifndef INC_FREERTOS_USER_H_
#define INC_FREERTOS_USER_H_

#include "cmsis_os.h"

extern osSemaphoreId lwipReadySem;
void MX_FREERTOS_Init(void);

#endif /* INC_FREERTOS_USER_H_ */
