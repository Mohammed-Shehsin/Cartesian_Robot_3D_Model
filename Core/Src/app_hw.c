/*
 * app_hw.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */


#include "app_hw.h"
#include "debug_uart.h"
#include "modbus_tcp_server.h"
#include "cmsis_os.h"
#include "modbus_regs.h"

// Motors
Stepper_t g_stepX = { STEP1_GPIO_Port, STEP1_Pin, DIR1_GPIO_Port, DIR1_Pin };
Stepper_t g_stepY = { STEP2_GPIO_Port, STEP2_Pin, DIR2_GPIO_Port, DIR2_Pin };

TB6612_t g_zMotor = {
    STBY_GPIO_Port, STBY_Pin,
    AIN1_GPIO_Port, AIN1_Pin,
    AIN2_GPIO_Port, AIN2_Pin,
    PWMA_GPIO_Port, PWMA_Pin
};

// End effector
Magnet_t g_magnet = { MAG_GPIO_Port, MAG_Pin };

// Ultrasonic sensors (your manual pins are OK)
HCSR04_t g_us1 = { GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_0, 0,0,0,0,0 };
HCSR04_t g_us2 = { GPIOA, GPIO_PIN_6, GPIOB, GPIO_PIN_1, 0,0,0,0,0 };
HCSR04_t g_us3 = { GPIOC, GPIO_PIN_3, GPIOC, GPIO_PIN_2, 0,0,0,0,0 };

void APP_HW_Init(void)
{
    extern UART_HandleTypeDef huart3;
    DebugUart_Init(&huart3);

    extern TIM_HandleTypeDef htim2;
    HCSR04_AttachTimer(&htim2);
}

extern volatile uint16_t holdingRegs[MODBUS_REG_COUNT];

void ModbusHeartbeat_Task(void const *argument)
{
    (void)argument;
    for (;;)
    {
        mb_fb[FB_HEARTBEAT]++;      // This must appear in PLC R_HR[7]
        osDelay(1000);
    }
}

