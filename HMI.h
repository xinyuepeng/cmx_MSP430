#ifndef HMI_H
#define HMI_H

#include "SysConfig.h"

#define  Version                 DIS_SPACE,DIS_r,DIS_6D,DIS_0
//=================================================================================================
//                      GLOBE CONSTANT DECLEAR
//=================================================================================================
#define  ROUNDUP_ONE_DEC_VAL        0.05

// P3 COM0 MARK SEGMENT CODE
#define  DIS_PEAK          0x02     // PEAK
#define  DIS_CO            0x04     // CO
#define  DIS_BAT2          0x08     // S2
#define  DIS_BAT3          0x10     // S3

// P3 COM1 MARK SEGMENT CODE
#define  DIS_O2            0x04     // O2
#define  DIS_BAT1          0x08     // S1
#define  DIS_BAT           0x10     // BAT

// P4 COM0 MARK SEGMENT CODE
#define  DIS_H2S           0x01     // H2S
#define  DIS_2H            0x10     // 2H

// P5 COM0 MARK SEGMENT CODE
#define  DIS_3H            0x01     // 3H
#define  DIS_PPM           0x02     // PPM
#define  DIS_LO            0x20     // LO
#define  DIS_SPEAKER       0x40     // SPEAKER
#define  DIS_ZERO          0x80     // ZERO

// P5 COM1 MARK SEGMENT CODE
#define  DIS_PERCENT       0x02     // %
#define  DIS_HI            0x40     // HI
#define  DIS_CAL           0x80     // CAL

// DIGIT AND CHARACTER SEGMENT CODE
#define  SEG_A             0X80
#define  SEG_B             0X40
#define  SEG_C             0X20
#define  SEG_D             0X10
#define  SEG_E             0X01
#define  SEG_G             0X02
#define  SEG_F             0X04
#define  SEG_DP            0X08

#define  DIS_SPACE         0x00   
#define  DIS_Minus         (SEG_G )
#define  DIS_0             (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F )
#define  DIS_0D            (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_DP )
#define  DIS_1             (SEG_B + SEG_C )
#define  DIS_1D            (SEG_B + SEG_C + SEG_DP)
#define  DIS_2             (SEG_A + SEG_B + SEG_D + SEG_E + SEG_G )
#define  DIS_2D            (SEG_A + SEG_B + SEG_D + SEG_E + SEG_G + SEG_DP )
#define  DIS_3D            (SEG_A + SEG_B + SEG_C + SEG_D + SEG_G + SEG_DP )
#define  DIS_3             (SEG_A + SEG_B + SEG_C + SEG_D + SEG_G )
#define  DIS_4             (SEG_B + SEG_C + SEG_F + SEG_G )
#define  DIS_4D            (SEG_B + SEG_C + SEG_F + SEG_G + SEG_DP )
#define  DIS_5             (SEG_A + SEG_C + SEG_D + SEG_F + SEG_G )
#define  DIS_5D            (SEG_A + SEG_C + SEG_D + SEG_F + SEG_G + SEG_DP  )
#define  DIS_6             (SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G )
#define  DIS_6D            (SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G + SEG_DP)
#define  DIS_7             (SEG_A + SEG_B + SEG_C )
#define  DIS_8             (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G )
#define  DIS_9             (SEG_A + SEG_B + SEG_C + SEG_D + SEG_F + SEG_G )
#define  DIS_A             (SEG_A + SEG_B + SEG_C + SEG_E + SEG_F + SEG_G )
#define  DIS_b             (SEG_C + SEG_D + SEG_E + SEG_F + SEG_G )
#define  DIS_C             (SEG_A + SEG_D + SEG_E + SEG_F )
#define  DIS_d             (SEG_B + SEG_C + SEG_D + SEG_E + SEG_G )
#define  DIS_E             (SEG_A + SEG_D + SEG_E + SEG_F + SEG_G )
#define  DIS_F             (SEG_A + SEG_E + SEG_F + SEG_G )
#define  DIS_H             (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G )
#define  DIS_I             (SEG_E + SEG_F )
#define  DIS_J             (SEG_B + SEG_C + SEG_D )
#define  DIS_L             (SEG_D + SEG_E + SEG_F )
#define  DIS_N             (SEG_A + SEG_B + SEG_C + SEG_E + SEG_F )
#define  DIS_n             (SEG_C + SEG_E + SEG_G )
#define  DIS_o             (SEG_C + SEG_D + SEG_E + SEG_G )
#define  DIS_oD            (SEG_C + SEG_D + SEG_E + SEG_G + SEG_DP )
#define  DIS_P             (SEG_A + SEG_B + SEG_E + SEG_F + SEG_G )
#define  DIS_r             (SEG_E + SEG_G )
#define  DIS_rD            (SEG_E + SEG_G + SEG_DP )
#define  DIS_t             (SEG_D + SEG_E + SEG_F + SEG_G ) 
#define  DIS_U             (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F )


/*
  "GOOD"
 */
#define  GOOD     DIS_9,DIS_o,DIS_o,DIS_d
/*
  Just clear the screen data.
 */
#define  NONE     DIS_SPACE,DIS_SPACE,DIS_SPACE,DIS_SPACE

//=================================================================================================
//                         GLOBE VARIABLE DECLEAR
//=================================================================================================
/*! \var INT8U DISPLAY_MARK1
    \brief used to count display data for DIS_PEAK+DIS_CO+DIS_BAT2+DIS_BAT3 
 */
extern INT8U DISPLAY_MARK1;       // DIS_PEAK+DIS_CO+DIS_BAT2+DIS_BAT3 
/*! \var INT8U DISPLAY_MARK2
    \brief used to count display data for DIS_O2+DIS_BAT1+DIS_BAT 
 */
extern INT8U DISPLAY_MARK2;       // DIS_O2+DIS_BAT1+DIS_BAT 
/*! \var INT8U DISPLAY_MARK3
    \brief used to count display data for DIS_H2S+DIS_2H 
 */
extern INT8U DISPLAY_MARK3;       // DIS_H2S+DIS_2H 
/*! \var INT8U DISPLAY_MARK4
    \brief used to count display data for DIS_3H+DIS_PPM+DIS_LO+DIS_SPEAKER+DIS_ZERO 
 */
extern INT8U DISPLAY_MARK4;       // DIS_3H+DIS_PPM+DIS_LO+DIS_SPEAKER+DIS_ZERO 
/*! \var INT8U DISPLAY_MARK5
    \brief DIS_PERCENT+DIS_HI+DIS_CAL 
 */
extern INT8U DISPLAY_MARK5;       // DIS_PERCENT+DIS_HI+DIS_CAL 

//=================================================================================================
//                         GLOBE FUNCTION DECLEAR
//=================================================================================================
/*! \fn void  HMI_Init(void)
    \brief initialize the display module.
*/
void  HMI_Init(void);

/*! \fn void  HMI_FreshDisplay(void)
    \brief Refesh LCM segments by segments pins data.
*/
void  HMI_FreshDisplay(void);

/*! \fn void  HMI_DisplayAll(void)
	\brief enable all segments pins data.
*/
void  HMI_DisplayAll(void);

/*! \fn void  HMI_DisplayClear(void)
	\brief set all LCM segment pins to zero.
*/
void  HMI_DisplayClear(void);

/*! \fn void  HMI_FloatToSegmentWithOneDec(float data)
	\brief show a float value with one dec.
*/
void  HMI_FloatToSegmentWithOneDec(float data);

/*! \fn void  HMI_FloatToSegment(float data)
	\brief show a float value to LCM.
	\param data the data to show.
*/
void  HMI_FloatToSegment(float data);

/*! \fn void  HMI_DisplayPeak(void)
	\brief show the gas peak frame, to display peak value and flag.
*/
void  HMI_DisplayPeak(void);

/*	\fn void  HMI_DisplayCode(char d1,char d2,char d3,char d4);
	\brief show the four characters in LCM.
	\param d1 is the first character defined in header file.
	\param d2 is the second character defined in header file.
	\param d3 is the third character defined in header file.
	\param d4 is the fourth character defined in header file.
*/
void  HMI_DisplayCode(char d1,char d2,char d3,char d4);

/*	\fn void  HMI_DisplayCodeAndMark(char d1,char d2,char d3,char d4)
	\brief show the four characters and flags in LCM.
	\param d1 is the first character defined in header file.
	\param d2 is the second character defined in header file.
	\param d3 is the third character defined in header file.
	\param d4 is the fourth character defined in header file.
*/
void  HMI_DisplayCodeAndMark(char d1,char d2,char d3,char d4);

/*	\fn void  HMI_DisplayCodePowerOff(void)
	\brief show the "OFF" in LCM.
*/
inline void  HMI_DisplayCodePowerOff(void);

/*	\fn void  HMI_DisplayCodeFail(void)
	\brief show the "FAIL" in LCM.
*/
void  HMI_DisplayCodeFail(void);

/*	\fn void  HMI_DisplayCodeCal(INT8U ucFlash)
	\brief show the Calibration flag, and flash "CAL" in LCM.
	\param ucFlash decides whether show "CAL" string or no string, makes the string flash..
*/
void  HMI_DisplayCodeCal(INT8U ucFlash);

void  HMI_DisplayDoCal(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeDisp(INT8U ucFlash)
	\brief show the "DISP" string in LCM and flash.
	\param ucFlash decides whether show "DISP" string or no string, makes the string flash..
*/
void  HMI_DisplayCodeDisp(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeLo(INT8U ucFlash)
	\brief show LOW alarm flags and flash "LOW" string.
	\param ucFlash decides whether show "LOW" string or no string, makes the string flash.
*/
void  HMI_DisplayCodeLo(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeHi(INT8U ucFlash)
	\brief show High alarm flags and flash "Hi" string.
	\param ucFlash decides whether show "Hi" string or no string, makes the string flash.
*/
void  HMI_DisplayCodeHi(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeZeroPrepare(INT8U ucFlash)
	\brief show Zero flag and current Gas value, and flash them.
	\param ucFlash decides whether show Zero flag and current value, makes them flash.
*/
void  HMI_DisplayCodeZeroPrepare(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeZeroRunning(INT8U ucFlash)
	\brief show "0000" string and zero flag and flash it.
	\param ucFlash decides whether show "0000" string, makes them flash.
*/
void  HMI_DisplayCodeZeroRunning(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeZeroFinish(INT8U ucFlash)
	\brief show "0000" string and zero flag and flash it.
	\param ucFlash decides whether show "0000" string, makes them flash.
*/
void  HMI_DisplayCodeZeroFinish(INT8U ucFlash);

/*	\fn void HMI_DisplayCodeCalPrepare(INT8U ucFlash)
	\brief show Calibration flag and gas reading value.
	\param ucFlash decides whether show Calibration flag and gas reading value, makes them flash.
*/
void  HMI_DisplayCodeCalPrepare(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeCalRunning(INT8U ucFlash)
	\brief show Calibration flag and gas reading value.
	\param ucFlash decides whether show gas reading value, makes them flash.
*/
void  HMI_DisplayCodeCalRunning(INT8U ucFlash);

/*	\fn void  HMI_DisplayCodeCalFinish(INT8U ucFlash)
	\brief show Sensor status or sensor SPAN
	\param ucFlash decides show status or SPAN.
*/
void  HMI_DisplayCodeCalFinish(INT8U ucFlash);

/*	\fn void HMI_DisplayNormal(void)
	\brief show gas reading value or "SAFE" string, according system display mode.
*/
void  HMI_DisplayNormal(void);

/*	\fn void  HMI_DisplayNormalOverLo(void)
	\brief show "-OR" string, if PPM mode, show "PPM" flag.
*/
void  HMI_DisplayNormalOverLo(void);

/*	\fn void  HMI_DisplayNormalOverHi(void)
	\brief show "OR" string, if PPM mode, show "PPM" flag.
*/
void  HMI_DisplayNormalOverHi(void);

/*	\fn void  HMI_DisplayNormalSpan(void)
	\brief show Zero and Calibration flag, and SPAN value.
*/
void  HMI_DisplayNormalSpan(void);

/*	\fn void  HMI_DisplayNormalHi(void)
	\brief show High alarm, and "Hi" string or gas value according system display mode.
*/
void  HMI_DisplayNormalHi(void);

/*	\fn void  HMI_DisplayNormalLo(void)
	\brief show Low alarm, and "Lo" string or gas value according system display mode.
*/
void  HMI_DisplayNormalLo(void);

/*	\fn void  HMI_DisplayPresetHi((INT8U ucFlash, float fDispData).
	\brief show pre-set gas High alarm threshold value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetHi(INT8U ucFlash, float fDispData);

/*	\fn void  HMI_DisplayPresetLo(void)
	\brief show pre-set gas Low alarm threshold value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetLo(INT8U ucFlash, float fDispData);

/*	\fn void   HMI_DisplayPresetCal(INT8U ucFlash, float fDispData)
	\brief show pre-set gas Calibration value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetCal(INT8U ucFlash, float fDispData);

/*	\fn void  HMI_DisplayPresetDispMod(INT8U ucFlash, INT8U ucDisMode)
	\brief show system pre-set display mode as "OFF" or "ON"
	\param ucFlash makes the value flash.
	\param ucDisMode decides the "OFF" or "ON" string.
*/
void  HMI_DisplayPresetDispMod(INT8U ucFlash, INT8U ucDisMode);

#endif