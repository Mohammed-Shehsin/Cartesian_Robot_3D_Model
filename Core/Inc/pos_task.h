/*
 * pos_task.h
 *
 *  Created on: Feb 19, 2026
 *      Author: moham
 */

#ifndef INC_POS_TASK_H_
#define INC_POS_TASK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x_mm;
    int y_mm;
    int z_mm;
    int tol_mm;   // ± tolerance
} TargetPos_t;

// Returns 1 = reached, 0 = fail/timeout/stop
uint8_t MoveToTarget_RunOnce(const TargetPos_t *tgt);

#ifdef __cplusplus
}
#endif

#endif
