#ifndef INC_MODBUS_REGS_H_
#define INC_MODBUS_REGS_H_

#include <stdint.h>

#define MB_WINDOW_LEN     8
#define MODBUS_ADDR_BASE  0   // Correct for MB_DATA_ADDR := 0 style addressing
//#define MODBUS_ADDR_BASE  40001
// --------------------
// Command window (PLC -> STM32) : W_HR[0..7]
// --------------------
typedef enum {
    CMD_CODE     = 0,   // W_HR[0]
    CMD_STEP_X   = 1,   // W_HR[1]
    CMD_STEP_Y   = 2,   // W_HR[2]
    CMD_STEP_Z   = 3,   // W_HR[3]
    CMD_DIR_BITS = 4,   // W_HR[4]
    CMD_SEQ      = 5,   // W_HR[5]  <-- IMPORTANT: sequence/trigger
    CMD_MAGNET   = 6,   // W_HR[6]
    CMD_PARAM2   = 7    // W_HR[7]  optional
} mb_cmd_idx_t;

// --------------------
// Feedback window (STM32 -> PLC) : R_HR[0..7]
// --------------------
typedef enum {
    FB_POS_X     = 0,
    FB_POS_Y     = 1,
    FB_US1_CM10  = 2,
    FB_US2_CM10  = 3,
    FB_US3_CM10  = 4,
    FB_STATUS    = 5,
    FB_ERR_CODE  = 6,
    FB_HEARTBEAT = 7
} mb_fb_idx_t;

// --------------------
// Command codes (values PLC writes into W_HR[CMD_CODE])
// --------------------
#define MB_CMD_IDLE       0u
#define MB_CMD_EXEC_MOVE  1u
#define MB_CMD_AUTOTEST   2u
#define MB_CMD_STOP       99u
#define MB_CMD_HOME      3u
#define MB_CMD_BOX1      4u
#define MB_CMD_BOX2      5u
#define MB_CMD_PICK      6u


extern volatile uint16_t mb_cmd[MB_WINDOW_LEN];
extern volatile uint16_t mb_fb[MB_WINDOW_LEN];

#endif
