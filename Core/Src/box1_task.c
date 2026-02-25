/*
 * box1_task.c
 *
 *  Created on: Feb 19, 2026
 *      Author: moham
 */
#include "box1_task.h"
#include "pos_task.h"
#include "magnet.h"
extern Magnet_t  g_magnet;
uint8_t Box1_RunOnce(void)
{
    TargetPos_t t = { .x_mm = 250, .y_mm = 100, .z_mm = 115, .tol_mm = 10 };

    return MoveToTarget_RunOnce(&t);
}



