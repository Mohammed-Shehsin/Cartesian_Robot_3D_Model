/*
 * home_task.h
 *
 *  Created on: Feb 19, 2026
 *      Author: moham
 */

#ifndef INC_HOME_TASK_H_
#define INC_HOME_TASK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Returns 1 on success, 0 on fail (timeout / stop / invalid sensor)
uint8_t Home_RunOnce(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_HOME_TASK_H_ */
