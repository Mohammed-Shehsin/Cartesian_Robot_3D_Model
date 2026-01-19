/*
 * hcsr04.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

#pragma once
#include "main.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *trigPort; uint16_t trigPin;
    GPIO_TypeDef *echoPort; uint16_t echoPin;

    volatile uint32_t t_rise_us;
    volatile uint32_t t_fall_us;
    volatile uint8_t got_rise;
    volatile uint8_t got_fall;
    volatile uint32_t edges;
} HCSR04_t;

void HCSR04_AttachTimer(TIM_HandleTypeDef *htim_us);
void HCSR04_Trigger(HCSR04_t *s);
float HCSR04_ReadCmSafe(HCSR04_t *s, uint32_t timeout_ms);
void HCSR04_EXTI_Callback(HCSR04_t *s, uint16_t gpio_pin);


#endif /* INC_HCSR04_H_ */
