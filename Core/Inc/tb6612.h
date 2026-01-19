/*
 * tb6612.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_TB6612_H_
#define INC_TB6612_H_

#pragma once
#include "main.h"
#include <stdint.h>

typedef enum { MOTOR_FWD = 0, MOTOR_REV } motor_dir_t;

typedef struct {
    GPIO_TypeDef *stbyPort; uint16_t stbyPin;
    GPIO_TypeDef *ain1Port; uint16_t ain1Pin;
    GPIO_TypeDef *ain2Port; uint16_t ain2Pin;
    GPIO_TypeDef *pwmPort;  uint16_t pwmPin;   // PWMA pin (GPIO software toggling)
} TB6612_t;

void TB6612_Enable(const TB6612_t *m, uint8_t enable);
void TB6612_SetDir(const TB6612_t *m, motor_dir_t dir);
void TB6612_SoftPwmOnce(const TB6612_t *m, uint8_t duty_percent, uint32_t period_ms);



#endif /* INC_TB6612_H_ */
