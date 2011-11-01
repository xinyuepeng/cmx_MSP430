#ifndef ALARM_H
#define ALARM_H

#include "SysConfig.h"

#define  SOUND_OFF               0
#define  SOUND_POWER             1
#define  SOUND_BAT               2
#define  SOUND_CAL               3
#define  SOUND_LOW               4
#define  SOUND_HI                5
#define  SOUND_FAIL              6
#define  SOUND_ENTER             7

#define  ALARM_OVER              0X01
#define  ALARM_HI                0X02
#define  ALARM_LOW               0X04
#define  ALARM_ZERO              0X08
#define  ALARM_CAL               0X10
#define  ALARM_ON                0X20
#define  ALARM_OFF               0X40
#define  ALARM_BAT               0X80

#define  LED_OFF                 0  
#define  LED_ON                  1

#define  VIBRATION_OFF           0 
#define  VIBRATION_ON            1

void Alarm_Init(void);
void Alarm_BuzzerOn(unsigned char flag);
void Alarm_BuzzerOff(void);
void Alarm_LedOn(void);
void Alarm_LedOff(void);
void Alarm_VibOn(void);
void Alarm_VibOff(void);
void Alarm_Off(void);
void Alarm_Beep(void);
#endif