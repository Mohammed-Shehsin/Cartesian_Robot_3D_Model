/*
 * app_hw.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_APP_HW_H_
#define INC_APP_HW_H_

#pragma once
#include "main.h"
#include "stepper.h"
#include "tb6612.h"
#include "magnet.h"
#include "hcsr04.h"

// Global hardware instances (defined in app_hw.c)
extern Stepper_t g_stepX;
extern Stepper_t g_stepY;

extern TB6612_t  g_zMotor;

extern Magnet_t  g_magnet;

extern HCSR04_t  g_us1;
extern HCSR04_t  g_us2;
extern HCSR04_t  g_us3;

// Optional: call once after MX_* init to attach timers etc.
void APP_HW_Init(void);


#endif /* INC_APP_HW_H_ */
