#include "SystemData.h"
#include "ADC.h"

/*! \var INT8U    g_ucSensorStatus
    \brief hold the value the current Sensor status.
    
    its value is SENSOR_GOOD or SENSOR_FAIL.
 */
INT8U    g_ucSensorStatus;

/*! \var INT8U    g_ucDisplayMod
    \brief hold the value 

    seems to display PPM or not.
    ?? Check it
 */
INT8U    g_ucDisplayMod;

/*! \var float g_fGasCurVal
    \brief hold the value of current A2D Gas.
 */
float    g_fGasCurVal;

/*! \var float g_fTempCoef
    \brief hold the value of current temporary Coef
 */
float    g_fTempCoef;

/*! \var float g_fSenZero
    \brief hold the Zero A2D value of the Sensor
 */
float    g_fSenZero;

/*! \var float    g_fSenSpan
    \brief hold the Sensor span value
 */
float    g_fSenSpan;

/*! \var float    g_fSenZeroHi
    \brief hold the Zero A2D value of the Sensor when gas in high scale value
 */
float    g_fSenZeroHi;

/*! \var float    g_fSenSpanHi
    \brief hold the Sensor span value when gas in high scale value
 */
float    g_fSenSpanHi;

/*! \var float    g_fGasAlarmHi
    \brief hold the gas high alarm limit value
 */
float    g_fGasAlarmHi;

/*! \var float    g_fGasAlarmLo
    \brief hold the gas low alarm limit value
 */
float    g_fGasAlarmLo;

/*! \var float    g_fGasCal
    \brief hold the value is used when calibration stand
 */
float    g_fGasCal;

/*! \var float    g_fGasCalSpan 
    \brief hold 
    
    ?? check it
 */
float    g_fGasCalSpan;

/*! \var INT16U   g_uiGasPeak
    \brief hold the max Gas value since recording data.
 */
INT16U   g_uiGasPeak;

void SystemData_Init(void)
{
  
    FCTL2  = FWKEY +  FSSEL_1 + FN0; 
    
    /* some variables will be updated from flash */
    g_fGasCurVal   = 0;               // CO MEASURE VALUE
    g_uiGasPeak    = 0;               // Gas PEAK VALUE
    g_fSenZero     = ZERO_L;          // CO ZERO VALUE (1.20V)
    g_fSenSpan     = SPAN_L;          // CO SPAN VALUE
    g_fSenZeroHi   = ZERO_H;          // CO ZERO VALUE (1.20V)
    g_fSenSpanHi   = SPAN_H;          // CO SPAN VALUE  
    g_fGasAlarmHi  = HIGH_ALARM;
    g_fGasAlarmLo  = LOW_ALARM;
    g_fGasCal      = CAL_GAS; 
    g_fTempCoef    = 1;
}


//=================================================================================================
//                         LOCAL FUNCTION DECLEAR
//=================================================================================================
void SystemData_Backup (void);
void Flasherase_SegA(void);
void Flasherase_SegB(void);
void SystemData_PEAKwrite(void);
void SystemData_Read(void);
void SystemData_STATUSwrite (void);
void SystemData_Write(void);

//=================================================================================================
//                         FUNCTION PROTOTYPE
//=================================================================================================
void SystemData_Backup (void)
{
   float *FLASH_CO_ZERO;
   float *FLASH_CO_SPAN;
   float *FLASH_CO_ZERO_H;
   float *FLASH_CO_SPAN_H;
   float *FLASH_CO_HIGH;
   float *FLASH_CO_LOW; 
   char  *FLASH_STATUS;

   float *FLASH_CO_PEAK;
   char  *FLASH_SENSOR_STATUS;
   char  *FLASH_CO_START;
   float *FLASH_CO_CAL;
   

   FLASH_CO_ZERO       =(float *)0x1002;
   FLASH_CO_SPAN       =(float *)0x1008;
   FLASH_CO_ZERO_H     =(float *)0x1012;
   FLASH_CO_SPAN_H     =(float *)0x1018; 
   FLASH_CO_HIGH       =(float *)0x1024;
   FLASH_CO_LOW        =(float *)0x1030;
   FLASH_STATUS        =(char  *)0x1070;

   FLASH_CO_PEAK       =(float *)0x1082;
   FLASH_SENSOR_STATUS =(char  *)0x1088; 
   FLASH_CO_START      =(char  *)0x1090;
   FLASH_CO_CAL        =(float *)0x1092;   

  
   if (*FLASH_STATUS  == 0xF0)
   {
      g_fSenZero       = *FLASH_CO_ZERO;
      g_fSenSpan       = *FLASH_CO_SPAN;
      g_fSenZeroHi     = *FLASH_CO_ZERO_H;
      g_fSenSpanHi     = *FLASH_CO_SPAN_H; 
      g_fGasAlarmHi    = *FLASH_CO_HIGH;
      g_fGasAlarmLo    = *FLASH_CO_LOW;   
        
      g_uiGasPeak       = *FLASH_CO_PEAK;
      g_ucSensorStatus  = *FLASH_SENSOR_STATUS;
      g_ucDisplayMod    = *FLASH_CO_START;
      g_fGasCal         = *FLASH_CO_CAL;
      
   }
   else
   {
      g_fSenZero       = g_fAdcValue[ADC_Vref_SQ];
      g_fSenSpan       = SPAN_L;
      g_fSenZeroHi     = ZERO_H;
      g_fSenSpanHi     = SPAN_H;  
      g_fGasAlarmHi    = HIGH_ALARM;
      g_fGasAlarmLo    = LOW_ALARM;
               
      g_uiGasPeak       = 0;
      g_ucSensorStatus  = SENSOR_GOOD;
      g_ucDisplayMod    = 0;
      g_fGasCal         = CAL_GAS;
   }
}

void Flasherase_SegA(void)
{

   float *Flash_ptr=(float *)FLASH_SEGA;

   _DINT();
   FCTL1 = FWKEY + ERASE;          // Set Erase bit
   FCTL3 = FWKEY;                  // Clear Lock bit
   *Flash_ptr = 0;                 // Dummy write to erase Flash segment
   FCTL1 = FWKEY;                  // Clear Erase bit
   FCTL3 = FWKEY + LOCK;           // Reset LOCK bit  
   _EINT();
}

void Flasherase_SegB(void)
{
   float *Flash_ptr=(float *)FLASH_SEGB;

   _DINT();
   FCTL1 = FWKEY + ERASE;          // Set Erase bit
   FCTL3 = FWKEY;                  // Clear Lock bit
   *Flash_ptr = 0;                 // Dummy write to erase Flash segment
   FCTL1 = FWKEY;                  // Clear Erase bit
   FCTL3 = FWKEY + LOCK;           // Reset LOCK bit  
   _EINT();
}

void SystemData_PEAKwrite(void)
{
   float *FLASH_CO_PEAK;
   char  *FLASH_SENSOR_STATUS;
   char  *FLASH_CO_START;
   float *FLASH_CO_CAL;
   INT8U i;
   
   FLASH_CO_PEAK       =(float *)0x1082;
   FLASH_SENSOR_STATUS =(char  *)0x1088;  
   FLASH_CO_START      =(char  *)0x1090;
   FLASH_CO_CAL        =(float *)0x1092;
   
   Flasherase_SegA();

   _DINT();
   FCTL1 = FWKEY + WRT;            // Set WRT bit for write operation
   FCTL3 = FWKEY;                  // Clear Lock bit

   *FLASH_CO_PEAK       = g_uiGasPeak;
   *FLASH_SENSOR_STATUS = g_ucSensorStatus;
   *FLASH_CO_START      = g_ucDisplayMod;
   *FLASH_CO_CAL        = g_fGasCal;
   
   FCTL1 = FWKEY;                  // Clear WRT bit
   FCTL3 = FWKEY + LOCK;           // Reset LOCK bit
   
   for (i=0;i<=0XFE;i++);
   _EINT();
}

void SystemData_Write(void)
{
   float *FLASH_CO_ZERO;
   float *FLASH_CO_SPAN;
   float *FLASH_CO_ZERO_H;
   float *FLASH_CO_SPAN_H;
   float *FLASH_CO_HIGH;
   float *FLASH_CO_LOW; 
   float *FLASH_CO_SPRV;   
   char  *FLASH_STATUS;

   float *FLASH_CO_PEAK;
   char  *FLASH_SENSOR_STATUS;
   char  *FLASH_CO_START;
   float *FLASH_CO_CAL;
   
   INT8U i;
   
   FLASH_CO_ZERO       =(float *)0x1002;
   FLASH_CO_SPAN       =(float *)0x1008;
   FLASH_CO_ZERO_H     =(float *)0x1012;
   FLASH_CO_SPAN_H     =(float *)0x1018; 
   FLASH_CO_HIGH       =(float *)0x1024;
   FLASH_CO_LOW        =(float *)0x1030;
   FLASH_CO_SPRV       =(float *)0x1034; 
   FLASH_STATUS        =(char  *)0x1070;

   FLASH_CO_PEAK       =(float *)0x1082;
   FLASH_SENSOR_STATUS =(char  *)0x1088; 
   FLASH_CO_START      =(char  *)0x1090;
   FLASH_CO_CAL        =(float *)0x1092;

   Flasherase_SegB();
   Flasherase_SegA();
   
   _DINT();
   FCTL1 = FWKEY + WRT;            // Set WRT bit for write operation
   FCTL3 = FWKEY;                  // Clear Lock bit

   *FLASH_CO_ZERO       = g_fSenZero;
   *FLASH_CO_SPAN       = g_fSenSpan;
   *FLASH_CO_ZERO_H     = g_fSenZeroHi;
   *FLASH_CO_SPAN_H     = g_fSenSpanHi; 
   *FLASH_CO_HIGH       = g_fGasAlarmHi;
   *FLASH_CO_LOW        = g_fGasAlarmLo;
   *FLASH_CO_SPRV       = g_fGasCalSpan;
   *FLASH_STATUS        = 0xF0;           // Write value to flash
   
   
   *FLASH_CO_PEAK       = g_uiGasPeak;
   *FLASH_SENSOR_STATUS = g_ucSensorStatus;
   *FLASH_CO_START      = g_ucDisplayMod;
   *FLASH_CO_CAL        = g_fGasCal;         // Write value to flash

   FCTL1 = FWKEY;                  // Clear WRT bit
   FCTL3 = FWKEY + LOCK;           // Reset LOCK bit
   
   for (i=0;i<=0XFE;i++);
   _EINT();
}
       
void SystemData_STATUSwrite (void)
{
   float *FLASH_CO_ZERO;
   float *FLASH_CO_SPAN;
   float *FLASH_CO_ZERO_H;
   float *FLASH_CO_SPAN_H;
   float *FLASH_CO_HIGH;
   float *FLASH_CO_LOW; 
   char  *FLASH_STATUS;

   float *FLASH_CO_PEAK;
   char  *FLASH_SENSOR_STATUS;
   char  *FLASH_CO_START;
   float *FLASH_CO_CAL;
   

   FLASH_CO_ZERO       =(float *)0x1002;
   FLASH_CO_SPAN       =(float *)0x1008;
   FLASH_CO_ZERO_H     =(float *)0x1012;
   FLASH_CO_SPAN_H     =(float *)0x1018; 
   FLASH_CO_HIGH       =(float *)0x1024;
   FLASH_CO_LOW        =(float *)0x1030;
   FLASH_STATUS        =(char  *)0x1070;

   FLASH_CO_PEAK       =(float *)0x1082;
   FLASH_SENSOR_STATUS =(char  *)0x1088; 
   FLASH_CO_START      =(char  *)0x1090;
   FLASH_CO_CAL        =(float *)0x1092;

   Flasherase_SegB();
   Flasherase_SegA();
   
   _DINT();
   FCTL1 = FWKEY + WRT;            // Set WRT bit for write operation
   FCTL3 = FWKEY;                  // Clear Lock bit

   *FLASH_CO_ZERO       = g_fSenZero;
   *FLASH_CO_SPAN       = g_fSenSpan;
   *FLASH_CO_ZERO_H     = g_fSenZeroHi;
   *FLASH_CO_SPAN_H     = g_fSenSpanHi; 
   *FLASH_CO_HIGH       = g_fGasAlarmHi;
   *FLASH_CO_LOW        = g_fGasAlarmLo;
   *FLASH_STATUS        = 0xF0;           // Write value to flash
   
   
   *FLASH_CO_PEAK       = g_uiGasPeak;
   *FLASH_SENSOR_STATUS = 1;
   *FLASH_CO_START      = g_ucDisplayMod;
   *FLASH_CO_CAL        = g_fGasCal;         // Write value to flash
   

   FCTL1 = FWKEY;                  // Clear WRT bit
   FCTL3 = FWKEY + LOCK;           // Reset LOCK bit
   _EINT();
} 

void SystemData_Read(void)
{
   float *FLASH_CO_ZERO;
   float *FLASH_CO_SPAN;
   float *FLASH_CO_ZERO_H;
   float *FLASH_CO_SPAN_H;
   float *FLASH_CO_HIGH;
   float *FLASH_CO_LOW;
   float *FLASH_CO_SPRV;  
   char  *FLASH_STATUS;

   float *FLASH_CO_PEAK;
   char  *FLASH_SENSOR_STATUS;
   char  *FLASH_CO_START;
   float *FLASH_CO_CAL;
   

   FLASH_CO_ZERO       =(float *)0x1002;
   FLASH_CO_SPAN       =(float *)0x1008;
   FLASH_CO_ZERO_H     =(float *)0x1012;
   FLASH_CO_SPAN_H     =(float *)0x1018; 
   FLASH_CO_HIGH       =(float *)0x1024;
   FLASH_CO_LOW        =(float *)0x1030;
   FLASH_CO_SPRV       =(float *)0x1034; 
   FLASH_STATUS        =(char  *)0x1070;

   FLASH_CO_PEAK       =(float *)0x1082;
   FLASH_SENSOR_STATUS =(char  *)0x1088; 
   FLASH_CO_START      =(char  *)0x1090;
   FLASH_CO_CAL        =(float *)0x1092;      

   /*
     check wheather flash has saved data.
    */
   if (*FLASH_STATUS  == 0xF0)
   {
      g_fSenZero        = *FLASH_CO_ZERO;
      g_fSenSpan        = *FLASH_CO_SPAN;
      g_fSenZeroHi      = *FLASH_CO_ZERO_H;
      g_fSenSpanHi      = *FLASH_CO_SPAN_H; 
      g_fGasAlarmHi     = *FLASH_CO_HIGH;
      g_fGasAlarmLo     = *FLASH_CO_LOW;   
      g_fGasCalSpan     = *FLASH_CO_SPRV;
      g_uiGasPeak       = *FLASH_CO_PEAK;
      g_ucSensorStatus  = *FLASH_SENSOR_STATUS;
      g_ucDisplayMod    = *FLASH_CO_START;
      g_fGasCal         = *FLASH_CO_CAL;
   }
   else
   {
      g_fSenZero        = g_fAdcValue[ADC_Vref_SQ];      
      g_fSenSpan        = SPAN_L;
      g_fSenZeroHi      = ZERO_H;
      g_fSenSpanHi      = SPAN_H;  
      g_fGasAlarmHi     = HIGH_ALARM;
      g_fGasAlarmLo     = LOW_ALARM;
      g_fGasCalSpan     = 0;
      g_uiGasPeak       = 0;
      g_ucSensorStatus  = SENSOR_FAIL;
      g_ucDisplayMod    = 0;
      g_fGasCal         = CAL_GAS;      
   }
}

//=================================================================================================
/** @}*/
