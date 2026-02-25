/*
 * pick_task.c (PICK WITH PRE-POSITION)
 *
 * Logic:
 *  1) Magnet ON (kept ON)
 *  2) Move to pick position using MoveToTarget_RunOnce():
 *       X=100, Y=300, Z=115, tol=10
 *     (This "cares about Z" because pos_task controls Z too.)
 *  3) Time-based Z DOWN for 3.5s
 *  4) Time-based Z UP for 3.5s
 *  5) Set CMD_CODE = 99 (STOP mode) and return
 */

#include "pick_task.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

#include "main.h"
#include "modbus_regs.h"
#include "tb6612.h"
#include "magnet.h"
#include "pos_task.h"   // <-- needed for TargetPos_t + MoveToTarget_RunOnce

// HW objects
extern TB6612_t  g_zMotor;
extern Magnet_t  g_magnet;

// from app_control.c (optional to keep sensor prints alive)
extern void uart_publish_sensors_to_plc(void);

// --------- PRE-PICK POSITION ----------
#define PICK_X_MM       100
#define PICK_Y_MM       300
#define PICK_Z_SAFE_MM  1
#define PICK_TOL_MM     10

// --------- Z SETTINGS ----------
#define Z_PWM_CYCLES        5
#define Z_BURST_DELAY_MS    20

// IMPORTANT: keep these matching your real direction
#define Z_DOWN_DIR          MOTOR_REV
#define Z_UP_DIR            MOTOR_FWD

// --------- MOTION TIMES ----------
#define Z_DOWN_TIME_MS      5000
#define Z_UP_TIME_MS        5000

static inline void z_burst(motor_dir_t dir)
{
    TB6612_Enable(&g_zMotor, 1);
    TB6612_SetDir(&g_zMotor, dir);

    for (int i = 0; i < Z_PWM_CYCLES; i++) {
        TB6612_SoftPwmOnce(&g_zMotor, 80, 50);
    }

    TB6612_Enable(&g_zMotor, 0);
}

static uint8_t run_z_time(motor_dir_t dir, uint32_t duration_ms)
{
    uint32_t t0 = HAL_GetTick();

    while ((HAL_GetTick() - t0) < duration_ms)
    {
        // optional: keep sensor updates alive
        uart_publish_sensors_to_plc();

        if (mb_cmd[CMD_CODE] == MB_CMD_STOP) {
            mb_fb[FB_ERR_CODE] = 99;
            printf("[PICK] Aborted (STOP)\r\n");
            TB6612_Enable(&g_zMotor, 0);
            return 0;
        }

        z_burst(dir);
        osDelay(Z_BURST_DELAY_MS);
    }

    return 1;
}

uint8_t Pick_RunOnce(void)
{
    mb_fb[FB_ERR_CODE] = 0;
    printf("\r\n[PICK] Start (move-to-pick + Z down/up)\r\n");
    osDelay(50);

    // Magnet ON always
    Magnet_On(&g_magnet);
    printf("[PICK] Magnet ON\r\n");
    osDelay(50);

    // 1) Go to pick XY FIRST (Z ignored here)
    TargetPos_t t = {
        .x_mm   = PICK_X_MM,
        .y_mm   = PICK_Y_MM,
        .z_mm   = PICK_Z_SAFE_MM,  // ignored by MoveToTargetXY_RunOnce
        .tol_mm = PICK_TOL_MM
    };

    printf("[PICK] MoveToTargetXY: X=%d Y=%d (Z ignored)\r\n",
           t.x_mm, t.y_mm);

    if (!MoveToTargetXY_RunOnce(&t)) {
        mb_fb[FB_ERR_CODE] = 31;
        printf("[PICK] FAIL: MoveToTargetXY_RunOnce()\r\n");
        return 0;
    }

    // 2) Z DOWN 3.5s
    printf("[PICK] Z DOWN %ums\r\n", (unsigned)Z_DOWN_TIME_MS);
    if (!run_z_time(Z_DOWN_DIR, Z_DOWN_TIME_MS)) {
        return 0;
    }

    // 3) Z UP 3.5s
    printf("[PICK] Z UP %ums\r\n", (unsigned)Z_UP_TIME_MS);
    if (!run_z_time(Z_UP_DIR, Z_UP_TIME_MS)) {
        return 0;
    }

    printf("[PICK] Done ✅ -> CMD=99 (STOP)\r\n");

    // Latch STOP state after pick is done
    mb_cmd[CMD_CODE] = 99;

    return 1;
}
