#ifndef ADC_H
#define ADC_H

#include "SysConfig.h"

//#define USING_INTERNAL_TEMP      1

//=================================================================================================
//                         GLOBE CONSTANTS DECLEAR
//=================================================================================================
#define  ADC_INNER_25V           0X10
#define  ADC_INNER_15V           0X00

#define  ADC_GasO2_CH            0x10     // ADC CONVERT CAHNNEL P6.0 for GAS O2
#define  ADC_Vref_CH             0x11     // ADC CONVERT CHANNEL P6.1 Vref2 1200mV
#define  ADC_GasB_CH             0x12     // ADC CONVERT CHANNEL P6.2 GAS2 OP, SMALL RANGE
#define  ADC_GasA_CH             0x13     // ADC CONVERT CHANNEL P6.3 GAS1 NOOP, BIGGER RANGE
#define  ADC_Temp_CH             0x14     // ADC CONVERT CHANNEL P6.4 EXTERNAL TEMP SENSOR
#define  ADC_Vbat_CH             0x15     // ADC CONVERT CHANNEL P6.5 VBAT
#define  ADC_VREFH               0x18     // ADC CONVERT CHANNEL VREF+
#define  ADC_VREFL               0x19     // ADC CONVERT CHANNEL VREF-

#ifdef USING_INTERNAL_TEMP
#define  ADC_Temp_Int_CH         0x1A     // ADC CONVERT CHANNEL internal temperature
#endif
#define  ADC_12VCC               0x1B     // ADC CONVERT CHANNEL A INTERNAL (VCC-VSS)/2
#define  ADC_Vref_Offset         1.0/2.5*4096

#define  ADC_GasO2_SQ            0        // Using the same array position with GasA
#define  ADC_GasA_SQ             0
#define  ADC_GasB_SQ             1
#define  ADC_Temp_SQ             2
#define  ADC_Vbat_SQ             3
#define  ADC_Vref_SQ             4
#ifdef USING_INTERNAL_TEMP
    #define  ADC_TempInt_SQ          5
#endif
//=================================================================================================
//                         GLOBE VARIABLE DECLEAR
//=================================================================================================



#define  ADC_BUF_SIZE     8
//=================================================================================================
//
//=================================================================================================
#define  ADC_SC                  0x01     // ADC CONVERT START
#define  ADC_ENC                 0x02     // ADC CONVERT ENABLE

/*! \var typedef struct ADC_BUF_BOX_DATA
    \brief a typedef for ADC round-queue structure.
 */
typedef struct
{
   float    fDataBuf[ADC_BUF_SIZE];
   INT8U    ucBoxTop;
   INT8U    ucBoxBottom;   
}  ADC_BUF_BOX_DATA;


/*! \var float g_fAdcValue[5]
    \brief realtime 5 channel ADC data.
 */
#ifdef USING_INTERNAL_TEMP
extern float g_fAdcValue[6];
#else
extern float g_fAdcValue[5]; 
#endif
//=================================================================================================
//                         GLOBE FUNCTION DECLEAR
//=================================================================================================
void ADC_Init(void);

void Task_GetADC(void);
void Task_GasReading(void);

#endif