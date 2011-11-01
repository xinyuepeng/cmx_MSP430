#ifndef KEYINPUT_H
#define KEYINPUT_H

#include "SysConfig.h"

//=================================================================================================
//                         GLOBE CONSTANTS DECLEAR
//=================================================================================================
#define  KEY_SCAN_ALT      70       // Key board scan alternate time (ms)
#define  KEY_RPT_START_DLY 100      // Number of scan times before auto repeat function engages
#define  KEY_RPT_DLY       50       // Number of scan times before auto repeat
                                    // executes the function again

#define  KEY_SELECT        16
#define  KEY_RETURN        1
#define  KEY_AUTOCAL       8

#define  KEY_BUF_SIZE               10    //Size of the KEYBOARD buffer 

#define  KEY_STATE_UP               1     // Key scanning states used in KeyScan()
#define  KEY_STATE_DEBOUNCE         2
#define  KEY_STATE_RPT_START_DLY    3
#define  KEY_STATE_RPT_DLY          4

struct KeyInputQueue
{
   INT8U     Buf[KEY_BUF_SIZE];           // Keyboard buffer 
   INT8U     BufInIx;                     // Index into key buf where next scan code will be inserted
   INT8U     BufOutIx;                    // Index into key buf where next scan code will be removed 
   INT8U     Number;                      // Number of keys read from the keyboard
   INT16U    Time;                  
   INT8U     ScanState;                   // Current state of key scanning function                     
   INT8U     Pressed;
};

//=================================================================================================
//                         GLOBE VARIABLE DECLEAR
//=================================================================================================
/*! \var INT16U   KEY_TIMER
    \brief hold the time value for key scan
 */
extern INT16U   KEY_TIMER;

/*! \var INT16U   BACKLIGHT_TIMER
    \brief hold the backlight enable time.
 */
extern INT16U   BACKLIGHT_TIMER;

//=================================================================================================
//                         GLOBE FUNCTION DECLEAR
//=================================================================================================
/*! \fn void Key_Init (void)
    \brief init the hardware pins for key, and global data.
 */
void     Key_Init (void); 

/*! \fn INT8U    Key_GetKeyPad(void)
	\brief get a key from keypad queue or directly read from device.
	\return the key value.
 */
INT8U    Key_GetKeyPad(void);

/*! \fn vvoid     Key_ClearBuf (void)
	\brief clear keypad queue.
 */
void     Key_ClearBuf (void);

/*! \fn BOOLEAN Key_Hit (void) 
    \brief check wheather there are keys
    
    ?? check it, change to inline?
 */
inline BOOLEAN  Key_Hit (void); 

/*! \fn INT8U Key_Decode (void)
    \brief get the keycode value from device.
	\return key value.
 */
INT8U    Key_Decode(void);                // Get scan code from current key pressed

/*! \fn INT8U Key_PopKey (void)
    \brief get key code from keyqueue.
	\return key avlue.
 */
INT8U    Key_PopKey (void);

INT8U Key_ReadKey(void);

/*! \fn BOOLEAN  Key_Pressed(void)
	\brief check whether a key is pressed.
	\return TRUE or FLASE
 */
BOOLEAN  Key_Pressed(void);

/*! \fn INT16U   Key_PressedTime(void)
	\brief get time of a key pressed.
	\return time value
 */
INT16U   Key_PressedTime(void);

/*! \fn void     Key_PowerOn(void)
	\brief enable Power pint.
 */
void     Key_PowerOn(void);

/*! \fn void     Key_ClearTime(void)
	\brief clear the time value of key pressed.
 */
void     Key_ClearTime(void);
void     Task_KeyScan(void);
inline void     EnableBackLight(unsigned char time);
inline void     DisableBackLight(void);
#endif