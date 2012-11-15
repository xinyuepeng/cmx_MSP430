#ifndef DYNAMENTPRIMER_H
#define DYNAMENTPRIMER_H

#include "SysConfig.h"
#include "SysUart.h"

#define CD_RD     0x13      // Read Code
#define CD_DLE    0x10      // Data Link Escape
#define CD_WR     0x15      // Write Code
#define CD_ACK    0x16      // Acknowledge
#define CD_NAK    0x19      // Negative Acknowledge
#define CD_DAT    0x1A      // Single Data Frame
#define CD_EOF    0x1F      // End of Frame
#define CD_WP1    0xE5      // Write password 1
#define CD_WP2    0xA2      // Write password 2


#define CONFIG_DATA_ID        0
#define LIVE_DATA_ID          1
#define ZERO_SENSOR_ID        2
#define SPAN_SENSOR_ID        3
#define LIVE_DATA_SIMPLE_ID   6
#define USER_DATA_ID          11

extern unsigned char current_cmd;
extern unsigned char current_id;
extern float current_reading;

unsigned char PremierReadValueCmd(unsigned char *FrameStr, unsigned char id);

#endif