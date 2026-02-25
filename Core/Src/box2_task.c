/*
 * box2_task.c
 *
 *  Created on: Feb 20, 2026
 *      Author: moham
 */
/*
 * box2_task.c
 */

#include "box2_task.h"
#include "pos_task.h"

uint8_t Box2_RunOnce(void)
{
    TargetPos_t t = {
        .x_mm  = 100,
        .y_mm  = 100,
        .z_mm  = 90,
        .tol_mm = 10
    };

    return MoveToTarget_RunOnce(&t);
}



