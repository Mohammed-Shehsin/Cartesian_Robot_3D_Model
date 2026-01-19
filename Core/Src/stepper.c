/*
 * stepper.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#include "stepper.h"

void Stepper_Step(const Stepper_t *m, int steps, uint32_t delay_ms, GPIO_PinState dir) {
    if (!m || steps <= 0) return;

    HAL_GPIO_WritePin(m->dirPort, m->dirPin, dir);
    HAL_Delay(2);

    for (int i = 0; i < steps; i++) {
        HAL_GPIO_WritePin(m->stepPort, m->stepPin, GPIO_PIN_SET);
        HAL_Delay(delay_ms);
        HAL_GPIO_WritePin(m->stepPort, m->stepPin, GPIO_PIN_RESET);
        HAL_Delay(delay_ms);
    }
}



