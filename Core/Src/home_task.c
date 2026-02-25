#include "home_task.h"
#include "pos_task.h"

uint8_t Home_RunOnce(void)
{
    TargetPos_t t = { .x_mm = 250, .y_mm = 300, .z_mm = 130, .tol_mm = 10 };
    return MoveToTarget_RunOnce(&t);
}
