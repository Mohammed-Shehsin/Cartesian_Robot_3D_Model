#include "app_autotest.h"
#include <stdio.h>
#include "cmsis_os.h"
#include "modbus_regs.h"   // mb_cmd[], mb_fb[], CMD_CODE, MB_CMD_STOP

#include "stepper.h"
#include "tb6612.h"
#include "magnet.h"

// External hardware objects
extern Stepper_t g_stepX;
extern Stepper_t g_stepY;
extern TB6612_t  g_zMotor;
extern Magnet_t  g_magnet;

// ------------------------------------------------------
// Abort condition: PLC sets CMD_CODE to STOP (99)
// ------------------------------------------------------
#ifndef MB_CMD_STOP
#define MB_CMD_STOP 99u
#endif

static inline int should_abort(void)
{
    return (mb_cmd[CMD_CODE] == MB_CMD_STOP);
}

// ------------------------------------------------------
// Print sensors: use values already published by Control_Task
// Reused registers:
//   FB_US1_CM10 (2) -> V1_mm
//   FB_US2_CM10 (3) -> V2_mm
//   FB_US3_CM10 (4) -> US_mm
// ------------------------------------------------------
static void PrintSensors(void)
{
    uint16_t v1_mm = mb_fb[FB_US1_CM10];
    uint16_t v2_mm = mb_fb[FB_US2_CM10];
    uint16_t us_mm = mb_fb[FB_US3_CM10];

    printf("V1:%u mm  V2:%u mm  US:%u mm\r\n",
           (unsigned)v1_mm,
           (unsigned)v2_mm,
           (unsigned)us_mm);
}

// ------------------------------------------------------
// Z motor run helper
// ------------------------------------------------------
static void Z_Run(motor_dir_t dir, int cycles)
{
    TB6612_Enable(&g_zMotor, 1);
    TB6612_SetDir(&g_zMotor, dir);

    for (int i = 0; i < cycles; i++) {
        if (should_abort()) break;
        TB6612_SoftPwmOnce(&g_zMotor, 80, 50);
        osDelay(1); // yield to RTOS
    }

    TB6612_Enable(&g_zMotor, 0);
}

// ------------------------------------------------------
// Main AutoTest sequence
// ------------------------------------------------------
void AutoTest_RunOnce(void)
{
    printf("\r\n=== AUTO TEST START ===\r\n");

    if (should_abort()) goto abort;
    printf("[0] Sensors baseline (from UART->Control_Task)\r\n");
    PrintSensors();

    if (should_abort()) goto abort;
    printf("[1] X stepper forward/back\r\n");
    Stepper_Step(&g_stepX, 800, 2, GPIO_PIN_SET);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    Stepper_Step(&g_stepX, 800, 2, GPIO_PIN_RESET);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    printf("[2] Y stepper forward/back\r\n");
    Stepper_Step(&g_stepY, 2000, 2, GPIO_PIN_SET);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    Stepper_Step(&g_stepY, 2000, 2, GPIO_PIN_RESET);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    printf("[3] Z DC motor forward/back\r\n");
    Z_Run(MOTOR_FWD, 120);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    Z_Run(MOTOR_REV, 120);
    osDelay(200);
    PrintSensors();

    if (should_abort()) goto abort;
    printf("[4] Magnet on/off\r\n");
    Magnet_On(&g_magnet);
    osDelay(300);
    PrintSensors();

    if (should_abort()) goto abort;
    Magnet_Off(&g_magnet);
    osDelay(300);
    PrintSensors();

    printf("=== AUTO TEST END ===\r\n\r\n");
    return;

abort:
    printf("=== AUTO TEST ABORTED (MB_CMD_STOP) ===\r\n\r\n");
    // safe shutdown
    TB6612_Enable(&g_zMotor, 0);
    Magnet_Off(&g_magnet);
}
