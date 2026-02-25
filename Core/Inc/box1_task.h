/*
 * box1_task.h
 *
 *  Created on: Feb 19, 2026
 *      Author: moham
 */

#ifndef INC_BOX1_TASK_H_
#define INC_BOX1_TASK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Returns 1 = reached, 0 = fail/timeout/stop
uint8_t Box1_RunOnce(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_BOX1_TASK_H_ */
