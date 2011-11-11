#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include "MSP430x14x.h"
#include "IODefines.h"

#define  TRUE                 1
#define  FALSE                0

typedef  unsigned    char     BOOLEAN;
typedef  unsigned    char     INT8U;            // Unsigned  8 bit quantity 
typedef  signed      char     INT8S;            // Signed    8 bit quantity
typedef  unsigned    int      INT16U;           // Unsigned 16 bit quantity
typedef  signed      int      INT16S;           // Signed   16 bit quantity
typedef  unsigned    long     INT32U;           // Unsigned 32 bit quantity
typedef  signed      long     INT32S;           // Signed   32 bit quantity
typedef  float                FP32;             // Single precision floating point
typedef  double               FP64;             // Double precision floating point 


//=================================================================================================
//                         GLOBE CONSTANTS DECLEAR
//=================================================================================================
#define  SENSOR_H2S              0
#define  SENSOR_CO               1
#define  SENSOR_O2               2

//#define  SENSOR_TYPE             SENSOR_H2S
//#define  SENSOR_TYPE             SENSOR_CO
#define SENSOR_TYPE SENSOR_O2

#if   SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S
    #define  ZERO_TIME               10*10                   // ZEROING DELAY TIME 
    #define  CAL_TIME                90*10                   // CALIBRATION DELAY TIME
#elif SENSOR_TYPE == SENSOR_O2
    #define  ZERO_TIME               1*10                   // ZEROING DELAY TIME
    #define  CAL_TIME                5*10                   // CALIBRATION DELAY TIME
#endif

#define  DISP_FLASH_RATE         8

#if   SENSOR_TYPE == SENSOR_CO
   #pragma message("This Software is for CO SENSOR")  
   #define  SENSOR_SMIN             (22.4E-9*10E3*(1+820.0/220.0)*4095.0/2.5)
   #define  LOW_ALARM               35          // <---- CO LOW ARLARM STAND IS 35 PPM 
   #define  HIGH_ALARM              70          // <---- CO HIGH ARLARM STAND IS 70 PPM  
   #define  CAL_GAS                 100         // <---- CO CAL STAND IS 100 PPM 
   #define  OVER_RANGE              1000        // CO DETECTOR RANGE 0~1000
 
   #define  HIGH_MAX                200
   #define  CAL_MAX                 250
   #define  CAL_ZERO                LOW_ALARM   // CO CALIBRATION ZERO BETWEEN +/- CAL_ZERO 

/*! \def RANG_ALT_HI/LO
  \brief Change GAS reading Pin according reading range value
 */
   #define  RANG_ALT_HI             200
   #define  RANG_ALT_LO             180   
   #pragma message("CO SENSOR RANGE 1000PPM")
#elif SENSOR_TYPE == SENSOR_H2S
   #pragma message("This Software is for H2S SENSOR")
   #define  SENSOR_SMIN             (40.0E-9*10E3*(1+820.0/220.0)*4095.0/2.5)
   #define  LOW_ALARM               10          // <---- H2S LOW ARLARM STAND IS 10 PPM 
   #define  HIGH_ALARM              20          // <---- H2S HIGH ARLARM STAND IS 20 PPM
   #define  CAL_GAS                 25          // <---- H2S CAL STAND IS 25 PPM 
   #define  OVER_RANGE              500         // H2S DETECTOR RANGE 0~500  
  
   #define  HIGH_MAX                200
   #define  CAL_MAX                 125
   #define  CAL_ZERO                LOW_ALARM   // H2S CALIBRATION ZERO BETWEEN +/- CAL_ZERO 
   #define  RANG_ALT_HI             100
   #define  RANG_ALT_LO              90   
   #pragma message("H2S SENSOR RANGE 500PPM")
#elif SENSOR_TYPE == SENSOR_O2
#pragma message("This Software is for O2 Sensor")
    #define OXY_STD     20.9
    #define CAL_GAS     OXY_STD

    #define SENSOR_SMIN ((0.11E-3*100*10*4096.0/2.5)/OXY_STD)
    #define LOW_ALARM   19.5
    #define HIGH_ALARM  23.5
    #define OVER_RANGE  30.0
    #define HIGH_MAX    20.9
    #define CAL_MAX     20.9
#else
   #message "NO SENSOR DEFINED !!!"
#endif

#define  ZERO_L                  2082.583
#define  SPAN_L                  5.476190
#define  ZERO_H                  2031.124
#define  SPAN_H                  1.166667

/*! \def SPAN_FAIL
 *  \brief A low value limit of span Gas value
 */
#define  SPAN_FAIL               g_fGasCal * 0.50     

/*! \def SPAIN_GOOD
 *  \brief A high value limit of span Gas value
 */
#define  SPAN_GOOD               g_fGasCal * 8.00 

#define  SENSOR_GOOD             1
#define  SENSOR_FAIL             0

#endif