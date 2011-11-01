#include "SystemState.h"
#include "HMI.h"
#include "KeyInput.h"
#include "SystemData.h"
#include "PowerClockControl.h"
#include "ADC.h"

SysStateIndex_t current_sysstate = SSI_Booting;

static BootStateIndex_t current_bootstate = BSI_On;
static ConfStateIndex_t current_confstate = CSI_HiAlarm;
static ReadingStateIndex_t current_readingstate = RSI_Reading;
static FailWarningIndex_t current_failwarningstate = FWI_HiAlarm;
static unsigned char bStateChanged = 1;

static void Booting_update(void);
static void Booting_keyhandler(unsigned char key);
static void Booting_Timer(void);
static void Config_update(void);
static void Config_keyhandler(unsigned char key);
static void Config_Timer(void);
static void Reading_update(void);
static void Reading_keyhandler(unsigned char key);
static void Reading_Timer(void);
static void Shutdown_update(void);
static void Shutdown_keyhandler(unsigned char key);
static void Shutdown_Timer(void);
static void FailWarning_update(void);
static void FailWarning_keyhandler(unsigned char key);
static void FailWarning_Timer(void);

static inline void SystemGotoNextState(SysStateIndex_t next);
static inline void BootingGotoNextState(BootStateIndex_t next);
static inline void ConfigGotoNextState(ConfStateIndex_t next);
static inline void ReadingGotoNextState(ReadingStateIndex_t next);

static SysState_t internalState[SSI_Last] =
{
  {
        Booting_update,
        Booting_keyhandler,
        Booting_Timer,
  },
  {
        Config_update,
        Config_keyhandler,
        Config_Timer,
  },
  {
        Reading_update,
        Reading_keyhandler,
        Reading_Timer,
  },
  {
        Shutdown_update,
        Shutdown_keyhandler,
        Shutdown_Timer,
  },
  {
        FailWarning_update,
        FailWarning_keyhandler,
        FailWarning_Timer,
  },
};

static unsigned int booting_tick = 0;
static unsigned char reading_tick;
static unsigned char reading_timeout = 0;
static unsigned char shutdown_tick = 0;
static unsigned char config_tick;
static unsigned char config_timeout;
static unsigned char failwaring_tick = 0;
static void Booting_update(void)
{
    switch(current_bootstate)
    {
    case BSI_On:
        HMI_DisplayCode(DIS_SPACE,DIS_0,DIS_n,DIS_SPACE);
        break;
    case BSI_Version:
        HMI_DisplayCode(Version); 
        break;
    case BSI_CountDown:
        HMI_FloatToSegment(20-(booting_tick-32)/10);
        break;
    case BSI_SelfCheck:
        HMI_DisplayAll();
        break;
    default:
        break;
    }
}

static void Booting_keyhandler(unsigned char key)
{
    unsigned char timeout = (booting_tick-32)/10;
    if( (timeout > 7) && (timeout < 12) &&
       (key == (KEY_RETURN + KEY_SELECT)))
    {
        booting_tick = 0;
        config_tick = 0;
        config_timeout = 0;
        current_sysstate = SSI_Config;
        current_confstate = CSI_HiAlarm;
        bStateChanged = TRUE;
        Key_ClearBuf();
    }
}


static void Booting_Timer(void)
{
    booting_tick++;
    if(booting_tick < 2)
    {
        EnableBackLight(10);
    }
    else if(booting_tick == 2)
    {
        Alarm_Off();
    }
    else if(booting_tick == 12)
    {
        current_bootstate = BSI_Version;
        bStateChanged = TRUE;
    }
    else if(booting_tick == 32)
    {
        current_bootstate = BSI_CountDown;
        bStateChanged = TRUE;
    }
    else if(booting_tick > 32 && booting_tick <= 232)
    {
        if(booting_tick % 10 == 2)
            bStateChanged = TRUE;
    }
    else if(booting_tick == 233)
    {
        current_bootstate = BSI_SelfCheck;
        bStateChanged = TRUE;
        EnableBackLight(5);
        Alarm_LedOn();   
        Alarm_VibOn();
    }
    else if(booting_tick == 238)
    {
        Alarm_Off();
    }
    else if(booting_tick == 248)
    {
        HMI_DisplayAll();
        EnableBackLight(5);
        Alarm_LedOn();   
        Alarm_VibOn();
    }
    else if(booting_tick == 253)
    {
        Alarm_Off();  
    }
    else if(booting_tick > 253)
    {
        Key_ClearBuf();
        current_sysstate = SSI_Reading;
        current_readingstate = RSI_Reading;
        reading_tick = 0;
        reading_timeout = 0;
        bStateChanged = TRUE;
    }
}

static float fDataBackup;
static unsigned char ucDataBackup;
static unsigned int actionTimeout;
static void Config_update(void)
{
    static unsigned char flash = 0;
    flash = flash > 0 ? 0 : 1;
    switch(current_confstate)
    {
    case CSI_HiAlarm:
      HMI_DisplayCodeHi(flash);
      break;
    case CSI_HiAlarmSet:
      HMI_DisplayPresetHi(1, fDataBackup);
      break;
    case CSI_LowAlarm:
      HMI_DisplayCodeLo(flash);
      break;
    case CSI_LowAlarmSet:
      HMI_DisplayPresetLo(1, fDataBackup);
      break;
    case CSI_CalGas:
      HMI_DisplayCodeCal(flash);
      break;
    case CSI_CalGasSet:
      HMI_DisplayPresetCal(1,fDataBackup);
      break;
    case CSI_DisplayMode:
      HMI_DisplayCodeDisp(flash);
      break;
    case CSI_DisplayModeSet:
      HMI_DisplayPresetDispMod(1,ucDataBackup);
      break;
    case CSI_CalibrationPrepare:
      HMI_DisplayDoCal(flash);
      break;
    case CSI_ZeroPrepare:
      HMI_DisplayCodeZeroPrepare(flash);
      break;
    case CSI_ZeroRunning:
      HMI_DisplayCodeZeroRunning(flash);
      break;
    case CSI_SpanPrepare:
      HMI_DisplayCodeCalPrepare(flash);
      break;
    case CSI_SpanRunning:
      HMI_DisplayCodeCalRunning(flash);
      break;
    case CSI_SpanFinishing:
      HMI_DisplayCodeCalFinish(flash);
      break;
    }
}

static void Config_keyhandler(unsigned char key)
{
    config_timeout = 0;
    switch(key)
    {
    case KEY_SELECT:
      if(current_confstate == CSI_HiAlarmSet)
      {
            if(g_fGasAlarmHi != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate ==  CSI_LowAlarmSet)
      {
            if(g_fGasAlarmLo != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate == CSI_CalGasSet)
      {
            if(g_fGasCal != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate == CSI_DisplayModeSet)
      {
            if(g_ucDisplayMod != ucDataBackup)
              SystemData_Write();
      }
      else
            break;
      break;
    case KEY_RETURN:
      if(Key_Pressed())
        break;
      if(current_confstate == CSI_HiAlarm)
      {
            fDataBackup = g_fGasAlarmHi;
      }
      else if(current_confstate == CSI_HiAlarmSet)
      {
            if(g_fGasAlarmHi != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate ==  CSI_LowAlarm)
      {
            fDataBackup  = g_fGasAlarmLo;
      }
      else if(current_confstate ==  CSI_LowAlarmSet)
      {
            if(g_fGasAlarmLo != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate == CSI_CalGas)
      {
            fDataBackup      = g_fGasCal;
      }
      else if(current_confstate == CSI_CalGasSet)
      {
            if(g_fGasCal != fDataBackup)
            {
                SystemData_Write();
            }
      }
      else if(current_confstate == CSI_DisplayMode)
      {
            ucDataBackup = g_ucDisplayMod;
      }
      else if(current_confstate == CSI_DisplayModeSet)
      {
            if(g_ucDisplayMod != ucDataBackup)
              SystemData_Write();
      }
      else
            break;
      Alarm_Beep();
      current_confstate++;
      bStateChanged = TRUE;
      break;
    case KEY_SELECT + KEY_RETURN:
      if(Key_Pressed())
        break;
      Alarm_Beep();
      if(current_confstate == CSI_CalibrationPrepare ||
         current_confstate == CSI_ZeroPrepare ||
           current_confstate == CSI_SpanPrepare)
      {
        current_confstate++;
        bStateChanged = TRUE;
        if(current_confstate == CSI_ZeroRunning ||
         current_confstate == CSI_SpanRunning)
        {
            actionTimeout = 0;
        }
      }
      if(current_confstate == CSI_SpanFinishing && g_ucSensorStatus == SENSOR_GOOD)
      {
            Key_ClearBuf();
            current_sysstate = SSI_Reading;
            current_readingstate = RSI_Reading;
            reading_tick = 0;
            reading_timeout = 0;
            bStateChanged = TRUE;
      }
      break;
    }
}

static float s_fCalZero    = 0;
static float s_fCalZeroHi  = 0;   
static float s_fCalSpan    = 0;
static float s_fCalSpanHi  = 0;
static float s_fVref = 0;
static void Config_Timer(void)
{
    float fDataBuf1 = 0;
    float fDataBuf2 = 0;
    config_tick++;
    if(config_tick % 10 == 0)
    {
        config_timeout++;
        actionTimeout++;
    }
    if(config_tick % 5 == 0)
        bStateChanged = TRUE;
    if(config_timeout > 10 && 
       current_confstate >= CSI_HiAlarm && current_confstate <= CSI_CalibrationPrepare)
    {
        Key_ClearBuf();
        current_sysstate = SSI_Reading;
        current_readingstate = RSI_Reading;
        reading_tick = 0;
        reading_timeout = 0;
        bStateChanged = TRUE;
    }
    
    if(current_confstate == CSI_ZeroRunning)
    {
        Task_GasReading();
        if(actionTimeout < ZERO_TIME)
        {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S
            s_fCalZero   = g_fAdcValue[ADC_GasB_SQ];
            s_fCalZeroHi = g_fAdcValue[ADC_GasA_SQ];
#elif SENSOR_TYPE == SENSOR_O2
            s_fCalZero   = g_fAdcValue[ADC_GasO2_SQ];
#endif            
            s_fVref      = g_fAdcValue[ADC_Vref_SQ];  
        }
        else
        {
            current_confstate++;
            bStateChanged = TRUE;
        }
    }
    else if(current_confstate == CSI_SpanRunning)
    {
        Task_GasReading();
        if(actionTimeout < CAL_TIME)
        {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S           
            fDataBuf1     = g_fAdcValue[ADC_GasB_SQ];
            fDataBuf1     = (fDataBuf1 - s_fCalZero)/g_fGasCal;
            s_fCalSpan    = fDataBuf1 / g_fTempCoef;
                                   
            fDataBuf2     = g_fAdcValue[ADC_GasA_SQ];  
            fDataBuf2     = (fDataBuf2 - s_fCalZeroHi)/g_fGasCal;
            s_fCalSpanHi  = fDataBuf2 / g_fTempCoef;
#elif SENSOR_TYPE == SENSOR_O2
            fDataBuf1     = g_fAdcValue[ADC_GasO2_SQ];
            fDataBuf1     = fDataBuf1/g_fGasCal;
            s_fCalSpan    = fDataBuf1 / g_fTempCoef;
#endif            
            g_fGasCalSpan = s_fCalSpan * g_fGasCal / (2.0 * SENSOR_SMIN);
        }
        else
        {
            if ((SPAN_FAIL <= g_fGasCalSpan) && ( g_fGasCalSpan <= SPAN_GOOD ))
            {
               g_ucSensorStatus = SENSOR_GOOD;
               g_fSenZero       = s_fCalZero;
               g_fSenSpan       = s_fCalSpan;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S                
               g_fSenZeroHi     = s_fCalZeroHi;
               g_fSenSpanHi     = s_fCalSpanHi;           
#endif               
            }
            else
            {
               g_ucSensorStatus = SENSOR_FAIL;  
            } 
            SystemData_Write();   
            current_confstate++;
            bStateChanged = TRUE;
        }
    }
}


#define READING_POWEROFF_TIMEOUT    300
#define READING_VIEW_VALUE_TIMEOUT  200

static void Reading_update(void)
{
    switch(current_readingstate)
    {
    case RSI_Reading:
        HMI_DisplayNormal();
        break;
    case RSI_PeakValue:
        HMI_DisplayPeak();
        break;
    case RSI_HiValue:
        HMI_DisplayPresetHi(1,g_fGasAlarmHi);
        break;
    case RSI_LowValue:
        HMI_DisplayPresetLo(1,g_fGasAlarmLo);
        break;
    default:
        HMI_DisplayNormal();
        break;      
    }   
}

static void Reading_keyhandler(unsigned char key)
{ 
    if(Key_Pressed())
        return;
    switch(key)
    {
    case KEY_SELECT:
        if((current_readingstate + 1) < RSI_Last)
        {
          current_readingstate++;
        }
        else
          current_readingstate = RSI_Reading;
        bStateChanged = TRUE;
        reading_timeout = 0;
        break;
    case KEY_RETURN:
        if(current_readingstate == RSI_Reading)
        {
            
            if(Key_PressedTime() > READING_POWEROFF_TIMEOUT)
            {
                current_readingstate = RSI_Reading;
                current_sysstate = SSI_Shutdown;
                shutdown_tick = 0;
                bStateChanged = TRUE;
            }
        }
        else if(current_readingstate == RSI_PeakValue)
        {
            g_uiGasPeak = 0;
            reading_timeout = 0;
        }
        break;
    }   
}

static void Reading_Timer(void)
{
    reading_tick++;
    if(reading_tick % 10 == 0)
    {    
        Task_GasReading();
        if(RSI_Reading == current_readingstate)
            bStateChanged = TRUE;
        if(g_fGasCurVal < -g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_OverLo; 
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
        else if (g_fGasCurVal > OVER_RANGE)
        {
            current_failwarningstate = FWI_OverHi;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }       
        else if (g_fGasCurVal >= g_fGasAlarmHi)
        {
            current_failwarningstate = FWI_HiAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S       
        else if (g_fGasCurVal >= g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_LoAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#elif SENSOR_TYPE == SENSOR_O2
        else if(g_fGasCurVal <= g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_LoAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#endif       
    }
    
    if(current_readingstate != RSI_Reading)
    {
      reading_timeout++;
      if(reading_timeout > READING_VIEW_VALUE_TIMEOUT)
      {
        current_readingstate = RSI_Reading;
        reading_timeout = 0;
      }
    }
}

unsigned char shutdown_count = 0;

static void Shutdown_update(void)
{
    HMI_DisplayCodePowerOff();
}

static void Shutdown_keyhandler(unsigned char key)
{
}

static void Shutdown_Timer(void)
{
    if(shutdown_tick % 10 == 0)
    {
        Alarm_BuzzerOn(1);
        Alarm_LedOn();
    }
    else if(shutdown_tick % 10 == 5)
    {
        Alarm_BuzzerOff();
        Alarm_LedOff();
        shutdown_count++;
    }
    
    if(Key_ReadKey() != KEY_RETURN || Key_Pressed() == 0)
    {
        current_readingstate = RSI_Reading;
        current_sysstate = SSI_Reading;
        reading_tick = 0;
        bStateChanged = TRUE;
    }
    else if(shutdown_count == 5)
    {
        HMI_DisplayCode(NONE);
        SystemData_PEAKwrite();
        HMI_DisplayClear();
        Alarm_Off();
        SystemPowerOff();
    }
    shutdown_tick++;
}

static void FailWarning_update(void)
{
    switch(current_failwarningstate)
    {
    case FWI_HiAlarm:
      HMI_DisplayNormalHi();
      break;
    case FWI_LoAlarm:
      HMI_DisplayNormalLo();
      break;
    case FWI_OverLo:
      HMI_DisplayNormalOverLo();
      break;
    case FWI_OverHi:
      HMI_DisplayNormalOverHi();
      break;
    case FWI_SensorFail:
      break;              
    }
}

static void FailWarning_keyhandler(unsigned char key)
{
    if(key == KEY_RETURN)
      if(Key_PressedTime() > READING_POWEROFF_TIMEOUT)
        {
            current_readingstate = RSI_Reading;
            current_sysstate = SSI_Shutdown;
            shutdown_tick = 0;
            shutdown_count = 0;
            bStateChanged = TRUE;
        }
}

static unsigned char uiAlarmCnt = 0;
static void FailWarning_Timer(void)
{
    failwaring_tick++;
    if(failwaring_tick % 10 == 0)
    {
        switch(current_failwarningstate)
        {
        case FWI_HiAlarm:
        case FWI_OverHi:
            switch(uiAlarmCnt)
             {
                case 0:
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOn();
                   break;
                case 1:                        
                   Alarm_BuzzerOff();
                   Alarm_LedOn();
                   Alarm_VibOn();               
                   break;  
                case 2:     
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOn();               
                   break;  
                case 3:
                   Alarm_BuzzerOff();
                   Alarm_LedOn();
                   Alarm_VibOn();
                   break;
                case 4:                        
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOff();               
                   break;  
                case 5:     
                   Alarm_BuzzerOff();
                   Alarm_LedOn();
                   Alarm_VibOff();               
                   break;                           
                case 6:
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOff();
                   break;
                case 7:                        
                   Alarm_BuzzerOff();
                   Alarm_LedOn();
                   Alarm_VibOff();               
                   break;  
                case 8:     
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOff();               
                   break;               
                default:
                   Alarm_Off();
                   break;                           
             }
             uiAlarmCnt ++;
             uiAlarmCnt %= 9;                  
          break;
        case FWI_LoAlarm:
           switch(uiAlarmCnt)
             {
                case 0:
                   Alarm_BuzzerOn(1);
                   Alarm_LedOff();
                   Alarm_VibOff();
                   break;            
                case 3:              
                   Alarm_BuzzerOff();
                   Alarm_LedOn();
                   Alarm_VibOff(); 
                   break;  
                case 6:            
                   Alarm_BuzzerOff();
                   Alarm_LedOff();
                   Alarm_VibOn(); 
                   break;
                case 7:            
                   Alarm_BuzzerOff();
                   Alarm_LedOff();
                   Alarm_VibOn(); 
                   break;          
                case 8:            
                   Alarm_BuzzerOff();
                   Alarm_LedOff();
                   Alarm_VibOn(); 
                   break;                    
                default:
                   Alarm_Off();
                   break;                           
             }
             uiAlarmCnt ++;
             uiAlarmCnt %= 9; 
          break;
        case FWI_OverLo:
        case FWI_SensorFail:
            if (uiAlarmCnt < 2)
             {
                Alarm_BuzzerOn(1);
             }
             else
             {
                Alarm_Off();   
             }            
             uiAlarmCnt ++;
             uiAlarmCnt %= 10;   
          break;              
        }
        
        if(g_ucSensorStatus == SENSOR_FAIL)
            return;
        
        Task_GasReading();
        if(RSI_Reading == current_readingstate)
            bStateChanged = TRUE;
        if(g_fGasCurVal < -g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_OverLo; 
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
        else if (g_fGasCurVal > OVER_RANGE)
        {
            current_failwarningstate = FWI_OverHi;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }       
        else if (g_fGasCurVal >= g_fGasAlarmHi)
        {
            current_failwarningstate = FWI_HiAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S       
        else if (g_fGasCurVal >= g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_LoAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#elif SENSOR_TYPE == SENSOR_O2
        else if(g_fGasCurVal <= g_fGasAlarmLo)
        {
            current_failwarningstate = FWI_LoAlarm;
            bStateChanged = TRUE;
            current_sysstate = SSI_FailWarning;
        }
#endif    
        else
        {
            Alarm_Off();
            current_readingstate = RSI_Reading;
            current_sysstate = SSI_Reading;
            reading_tick = 0;
            bStateChanged = TRUE;
        }
    }
}


void UpdateSystemScreen()
{
    if(bStateChanged)
    {
        internalState[current_sysstate].fnptr_update();
        bStateChanged = 0;
    }
}

void SysStateKeyHandler(unsigned char key)
{
    internalState[current_sysstate].fnptr_keyhandle(key);
}

void SysStateTimer(void)
{
    internalState[current_sysstate].fnptr_timer();
}