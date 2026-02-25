/*
 * app_control.h
 *
 *  Created on: Feb 9, 2026
 *      Author: moham
 */

#ifndef APP_CONTROL_H_
#define APP_CONTROL_H_

#include <stdint.h>

void Control_Task(void const *argument);
void uart_publish_sensors_to_plc(void);


#endif /* APP_CONTROL_H_ */
