/*
 * tb6612.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#include "tb6612.h"

void TB6612_Enable(const TB6612_t *m, uint8_t enable) {
    if (!m) return;
    HAL_GPIO_WritePin(m->stbyPort, m->stbyPin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void TB6612_SetDir(const TB6612_t *m, motor_dir_t dir) {
    if (!m) return;
    if (dir == MOTOR_FWD) {
        HAL_GPIO_WritePin(m->ain1Port, m->ain1Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m->ain2Port, m->ain2Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(m->ain1Port, m->ain1Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m->ain2Port, m->ain2Pin, GPIO_PIN_SET);
    }
}

void TB6612_SoftPwmOnce(const TB6612_t *m, uint8_t duty_percent, uint32_t period_ms) {
    if (!m) return;
    if (duty_percent > 100) duty_percent = 100;
    if (period_ms < 2) period_ms = 2;

    uint32_t on_time  = (period_ms * duty_percent) / 100U;
    uint32_t off_time = period_ms - on_time;

    HAL_GPIO_WritePin(m->pwmPort, m->pwmPin, GPIO_PIN_SET);
    HAL_Delay(on_time);
    HAL_GPIO_WritePin(m->pwmPort, m->pwmPin, GPIO_PIN_RESET);
    HAL_Delay(off_time);
}



