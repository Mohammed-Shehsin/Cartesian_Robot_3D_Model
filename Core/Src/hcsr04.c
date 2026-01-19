/*
 * hcsr04.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#include "hcsr04.h"

static TIM_HandleTypeDef *s_tim = NULL;

void HCSR04_AttachTimer(TIM_HandleTypeDef *htim_us) {
    s_tim = htim_us;
}

static inline uint32_t micros(void) {
    return s_tim ? __HAL_TIM_GET_COUNTER(s_tim) : 0;
}

void HCSR04_Trigger(HCSR04_t *s) {
    if (!s) return;
    s->got_rise = 0;
    s->got_fall = 0;

    HAL_GPIO_WritePin(s->trigPort, s->trigPin, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(s->trigPort, s->trigPin, GPIO_PIN_SET);
    HAL_Delay(1); // OK but crude; later we can replace with true 10us delay
    HAL_GPIO_WritePin(s->trigPort, s->trigPin, GPIO_PIN_RESET);
}

float HCSR04_ReadCmSafe(HCSR04_t *s, uint32_t timeout_ms) {
    if (!s || !s_tim) return -3.0f;

    if (HAL_GPIO_ReadPin(s->echoPort, s->echoPin) == GPIO_PIN_SET)
        return -2.0f; // stuck high

    HCSR04_Trigger(s);

    uint32_t start = micros();
    uint32_t timeout_us = timeout_ms * 1000U;

    while (!s->got_fall) {
        if ((uint32_t)(micros() - start) > timeout_us)
            return -1.0f; // timeout
    }

    uint32_t pulse_us = (uint32_t)(s->t_fall_us - s->t_rise_us);
    return (float)pulse_us / 58.0f;
}

void HCSR04_EXTI_Callback(HCSR04_t *s, uint16_t gpio_pin) {
    if (!s || gpio_pin != s->echoPin || !s_tim) return;

    uint32_t t = micros();
    s->edges++;

    if (HAL_GPIO_ReadPin(s->echoPort, s->echoPin) == GPIO_PIN_SET) {
        s->t_rise_us = t;
        s->got_rise = 1;
    } else if (s->got_rise) {
        s->t_fall_us = t;
        s->got_fall = 1;
    }
}



