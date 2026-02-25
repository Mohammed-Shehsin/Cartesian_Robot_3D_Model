/*
 * stepper.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#pragma once
#include "main.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *stepPort;
    uint16_t stepPin;
    GPIO_TypeDef *dirPort;
    uint16_t dirPin;
} Stepper_t;

void Stepper_Step(const Stepper_t *m, int steps, uint32_t delay_ms, GPIO_PinState dir);


#endif /* INC_STEPPER_H_ */
