#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include "SysConfig.h"

typedef struct SysState {
    void (*fnptr_update)(void);
    void (*fnptr_keyhandle)(unsigned char key);
    void (*fnptr_timer)(void);
} SysState_t;

typedef enum {SSI_Booting, SSI_Config, SSI_Reading, SSI_Shutdown, 
            SSI_FailWarning, SSI_Last } SysStateIndex_t;

typedef enum {BSI_On, BSI_Version, BSI_CountDown, BSI_SelfCheck, BSI_Last } BootStateIndex_t;

typedef enum {CSI_HiAlarm, CSI_HiAlarmSet, CSI_LowAlarm, CSI_LowAlarmSet, 
            CSI_CalGas, CSI_CalGasSet, CSI_DisplayMode, CSI_DisplayModeSet,
            CSI_CalibrationPrepare, CSI_ZeroPrepare, CSI_ZeroRunning,
            CSI_SpanPrepare, CSI_SpanRunning, CSI_SpanFinishing, 
            CSI_Last } ConfStateIndex_t;

typedef enum {RSI_Reading, RSI_PeakValue, RSI_LowValue, RSI_HiValue, 
            RSI_Last } ReadingStateIndex_t;

typedef enum {FWI_HiAlarm, FWI_LoAlarm, FWI_OverLo, FWI_OverHi, 
            FWI_SensorFail, FWI_Last } FailWarningIndex_t;

extern SysStateIndex_t current_sysstate;

inline void SysStateKeyHandler(unsigned char key);
inline void SysStateTimer(void);
void UpdateSystemScreen(void);
#endif