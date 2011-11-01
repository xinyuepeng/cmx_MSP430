#ifndef POWERCLOCKCONTROL_H
#define POWERCLOCKCONTROL_H

#include "SysConfig.h"

void InitPowerClockController(void);
inline void EnablePowerOn(void);
void SystemPowerOff(void);
void EnableSysLowPowerMode();
#endif