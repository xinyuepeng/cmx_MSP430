#ifndef SYSDATA_H
#define SYSDATA_H

#include "SysConfig.h"

//=================================================================================================
//                      GLOBE CONSTANTS DECLEAR
//=================================================================================================
#define  FLASH_SEGA        0x1080   // INFORMATION FLASH SEGMENT A START ADDRESS 
#define  FLASH_SEGB        0x1000   // INFORMATION FLASH SEGMENT B START ADDRESS 

/*! \var INT8U    g_ucSensorStatus
    \brief hold the value the current Sensor status.
    
    its value is SENSOR_GOOD or SENSOR_FAIL.
 */
extern INT8U    g_ucSensorStatus;

/*! \var INT8U    g_ucDisplayMod
    \brief hold the value 

    seems to display PPM or not.
    ?? Check it
 */
extern INT8U    g_ucDisplayMod;

/*! \var float g_fGasCurVal
    \brief hold the value of current A2D Gas.
 */
extern float    g_fGasCurVal;

/*! \var float g_fTempCoef
    \brief hold the value of current temporary Coef
 */
extern float    g_fTempCoef;

/*! \var float g_fSenZero
    \brief hold the Zero A2D value of the Sensor
 */
extern float    g_fSenZero;

/*! \var float    g_fSenSpan
    \brief hold the Sensor span value
 */
extern float    g_fSenSpan;

/*! \var float    g_fSenZeroHi
    \brief hold the Zero A2D value of the Sensor when gas in high scale value
 */
extern float    g_fSenZeroHi;

/*! \var float    g_fSenSpanHi
    \brief hold the Sensor span value when gas in high scale value
 */
extern float    g_fSenSpanHi;

/*! \var float    g_fGasAlarmHi
    \brief hold the gas high alarm limit value
 */
extern float    g_fGasAlarmHi;

/*! \var float    g_fGasAlarmLo
    \brief hold the gas low alarm limit value
 */
extern float    g_fGasAlarmLo;

/*! \var float    g_fGasCal
    \brief hold the value is used when calibration stand
 */
extern float    g_fGasCal;

/*! \var float    g_fGasCalSpan 
    \brief hold 
    
    ?? check it
 */
extern float    g_fGasCalSpan;

/*! \var INT16U   g_uiGasPeak
    \brief hold the max Gas value since recording data.
 */
extern INT16U   g_uiGasPeak;


void SystemData_Init(void);
void SystemData_Backup (void);
void SystemData_PEAKwrite(void);
void SystemData_Read(void);
void SystemData_STATUSwrite (void);
void SystemData_Write(void);

#endif