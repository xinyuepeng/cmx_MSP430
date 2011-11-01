#include "HMI.h"
#include "SystemData.h"

/*! \var const INT8U DIS[11]
    \brief hold index data array for displaying float value.
 */
static const INT8U DIS[11]    = {DIS_0,DIS_1,DIS_2,DIS_3,DIS_4,DIS_5,DIS_6,DIS_7,DIS_8,DIS_9,DIS_Minus};

/*! \var INT8U LCD_P3_COM0_SEG 
    \brief hold the value to control the Pin3 COM0 Segement data.
 */                                     
INT8U LCD_P3_COM0_SEG  = 0;       // LCD DISPLAY PORT3 & COMO SEGMENT
INT8U LCD_P3_COM1_SEG  = 0;       // LCD DISPLAY PORT3 & COM1 SEGMENT
INT8U LCD_P4_COM0_SEG  = 0;       // LCD DISPLAY PORT4 & COM0 SEGMENT
INT8U LCD_P4_COM1_SEG  = 0;       // LCD DISPLAY PORT4 & COM1 SEGMENT
INT8U LCD_P5_COM0_SEG  = 0;       // LCD DISPLAY PORT5 & COM0 SEGMENT
INT8U LCD_P5_COM1_SEG  = 0;       // LCD DISPLAY PORT5 & COM1 SEGMENT

INT8U DISPLAY_MARK1;              // DIS_PEAK+DIS_CO+DIS_BAT2+DIS_BAT3 
INT8U DISPLAY_MARK2;              // DIS_O2+DIS_BAT1+DIS_BAT 
INT8U DISPLAY_MARK3;              // DIS_H2S+DIS_2H 
INT8U DISPLAY_MARK4;              // DIS_3H+DIS_PPM+DIS_LO+DIS_SPEAKER+DIS_ZERO 
INT8U DISPLAY_MARK5;              // DIS_PERCENT+DIS_HI+DIS_CAL 

//=================================================================================================
//                         GLOBE FUNCTION DECLEAR
//=================================================================================================
/*! \fn void  HMI_Init(void)
    \brief initialize the display module.
*/
void  HMI_Init(void)
{
    LCD_P3_COM0_SEG  = 0xff;   // LCD DISPLAY PORT3 & COMO SEGMENT
    LCD_P3_COM1_SEG  = 0;      // LCD DISPLAY PORT3 & COM1 SEGMENT
    LCD_P4_COM0_SEG  = 0xff;   // LCD DISPLAY PORT4 & COM0 SEGMENT
    LCD_P4_COM1_SEG  = 0;      // LCD DISPLAY PORT4 & COM1 SEGMENT
    LCD_P5_COM0_SEG  = 0xff;   // LCD DISPLAY PORT5 & COM0 SEGMENT
    LCD_P5_COM1_SEG  = 0;      // LCD DISPLAY PORT5 & COM1 SEGMENT   
   
    DISPLAY_MARK1 = 0;         // DIS_PEAK+DIS_CO+DIS_BAT2+DIS_BAT3 
    DISPLAY_MARK2 = 0;         // DIS_O2+DIS_BAT1+DIS_BAT 
    DISPLAY_MARK3 = 0;         // DIS_H2S+DIS_2H 
    DISPLAY_MARK4 = 0;         // DIS_3H+DIS_PPM+DIS_LO+DIS_SPEAKER+DIS_ZERO 
    DISPLAY_MARK5 = 0;         // DIS_PERCENT+DIS_HI+DIS_CAL   
}

/*! \fn void  HMI_FreshDisplay(void)
    \brief Refesh LCM segments by segments pins data.
*/
void  HMI_FreshDisplay(void)
{
    static INT8U LCD_PULSE = 1;

    switch (LCD_PULSE)
        {
        case 1:
         P2DIR &=~0x80;                      //SELECT P2.7(COM0) TO INPUT(0)
        //       P2OUT =   M                         //SET P3.0 (COM1) TO MID(M) 
          
         P3DIR |= 0x01;                      //SELECT P3.0(COM1) TO OUTPUT(1) 
         P3OUT &=~0x01;                      //SET P3.0(COM1) TO LOW(0)
         
         P3OUT = 0xFE & LCD_P3_COM1_SEG;
         P4OUT =        LCD_P4_COM1_SEG;
         P5OUT =        LCD_P5_COM1_SEG;
         
         LCD_PULSE++;
         break;
        
        case 2:
         P2DIR &=~0x80;                      //SELECT P2.7(COM0) TO INPUT(0)   
        //       P2OUT =   M                         //SET P3.0 (COM1) TO MID(M) 
         
         P3DIR |= 0x01;                      //SELECT P3.0(COM1) TO OUTPUT(1)
         P3OUT |= 0x01;                      //SET P3.0(COM1) TO HIGH(1)
         
         P3OUT = 0x01 | (~LCD_P3_COM1_SEG);
         P4OUT =         ~LCD_P4_COM1_SEG;
         P5OUT =         ~LCD_P5_COM1_SEG;
         LCD_PULSE++;         
         break;
        
        case 3:
         P2DIR |= 0x80;                      //SELECT P2.7(COM0) TO OUTPUT(1)
         P2OUT &=~0x80;                      //SET P2.7(COM0) TO LOW(0)
         
         P3DIR =0xFE;                        //SELECT P3.0(COM1) TO INPUT(0)
        //       P3OUT  =  M                         //SET P3.0 (COM1) TO MID(M) 
         
         P3OUT = LCD_P3_COM0_SEG;
         P4OUT = LCD_P4_COM0_SEG;
         P5OUT = LCD_P5_COM0_SEG;
         LCD_PULSE++;           
         break;
        
        case 4:
         P2DIR |= 0x80;                      //SELECT P2.7(COM0) TO OUTPUT(1)
         P2OUT |= 0x80;                      //SET P2.7(COM0) TO HIGH(1)
         
         P3DIR =0xFE;                        //SELECT P3.0(COM1) TO INPUT(0)
        //       P3OUT  =  M                         //SET P3.0 (COM1) TO MID(M) 
         
         P3OUT = ~LCD_P3_COM0_SEG;
         P4OUT = ~LCD_P4_COM0_SEG;
         P5OUT = ~LCD_P5_COM0_SEG;
         LCD_PULSE = 1;         
         break;
        default:
         LCD_PULSE = 1;
         break;  
        } // End of switch
}

/*! \fn void  HMI_DisplayAll(void)
	\brief enable all segments pins data.
*/
void  HMI_DisplayAll(void)
{
    LCD_P3_COM0_SEG  = 0xFF;   // LCD DISPLAY PORT3 & COMO SEGMENT
    LCD_P3_COM1_SEG  = 0xFF;   // LCD DISPLAY PORT3 & COM1 SEGMENT
    LCD_P4_COM0_SEG  = 0xFF;   // LCD DISPLAY PORT4 & COM0 SEGMENT
    LCD_P4_COM1_SEG  = 0xFF;   // LCD DISPLAY PORT4 & COM1 SEGMENT
    LCD_P5_COM0_SEG  = 0xFF;   // LCD DISPLAY PORT5 & COM0 SEGMENT
    LCD_P5_COM1_SEG  = 0xFF;   // LCD DISPLAY PORT5 & COM1 SEGMENT
}

/*! \fn void  HMI_DisplayClear(void)
	\brief set all LCM segment pins to zero.
*/
void  HMI_DisplayClear(void)
{
    P2SEL &=~ 0X80;
    P2DIR &=~ 0X80;
   
    P3SEL  = 0;
    P3DIR  = 0;
    P3OUT  = 0;
    
    P4OUT  = 0;
    P5OUT  = 0;
}

/*! \fn void  HMI_FloatToSegmentWithOneDec(float data)
	\brief show a float value with one dec.
*/
void  HMI_FloatToSegmentWithOneDec(float data)
{
    int  d;
    char d1,d2,d3,d4;
    unsigned long LCD_BUFFER0;
    unsigned long LCD_BUFFER1;
    
    data += ROUNDUP_ONE_DEC_VAL;
    d = (int)(data*10);
    
    if (d < -999) d = -999;
    if (d > 9999) d = 9999;
    if (d < 0)
    {
       d  = -d;
       d1 = 10;
    }
    else
    {
       d1 = (char)(d/1000);
       d  = d - d1*1000;
    } 
    d2 = (char)(d/100);
    d  = d - d2*100;
    d3 = (char)(d/10);
    d  = d - d3*10;
    d4 = (char)d;
    
    d1 = DIS[d1];
    d2 = DIS[d2];
    d3 = DIS[d3];
    d4 = DIS[d4]; 
     
    LCD_BUFFER0 = 0x07 & d4;
    LCD_BUFFER1 = 0x0F & (d4>>4);
    
    LCD_BUFFER0 = (LCD_BUFFER0 <<5 ) + (0x07 & d3);
    LCD_BUFFER1 = (LCD_BUFFER1 <<5 ) + (0x0F & (d3 >> 4));   
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) +(0x07&d2);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) +(0x0F&(d2>>4));    
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) +(0x07&d1);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) +(0x0F&(d1>>4));    
    
    LCD_BUFFER0 = LCD_BUFFER0 << 5;
    LCD_BUFFER1 = LCD_BUFFER1 << 5;      
    
    _DINT();
    
    LCD_P3_COM0_SEG = (char)(LCD_BUFFER0 & 0x0000FF)+DISPLAY_MARK1;   //P3 PORT COM0,  P31:PEAK,P32:CO,P33:S2,P34:S3
    LCD_P3_COM1_SEG = (char)(LCD_BUFFER1 & 0x0000FF)+DISPLAY_MARK2;   //P3 PORT COM1,  P32:O2,P33:S1,P34:S4
    
    LCD_BUFFER0     = LCD_BUFFER0 >> 8;
    LCD_BUFFER1     = LCD_BUFFER1 >> 8;
    
    LCD_P4_COM0_SEG = (char)(LCD_BUFFER0 & 0x00FF)+DISPLAY_MARK3;      //P4 PORT COM0,  P40:H2S,P44:2H
    LCD_P4_COM1_SEG = (char)(LCD_BUFFER1 & 0x00FF);
    
    LCD_BUFFER0     = LCD_BUFFER0>>8;
    LCD_BUFFER1     = LCD_BUFFER1>>8;   
    
    LCD_P5_COM0_SEG = (char)(LCD_BUFFER0&0xFF)+DISPLAY_MARK4;   //P5 PORT COM0,  P50:3H,P51:PPM,P55:LO,P56:S5,P57:ZERO
    LCD_P5_COM1_SEG = (char)(LCD_BUFFER1&0xFF)+DISPLAY_MARK5;   //P5 PORT COM1,  P51:%,P56:HI,P57:CAL
    
    _EINT();
}

/*! \fn void  HMI_FloatToSegment(float data)
	\brief show a float value to LCM.
	\param data the data to show.
*/
void  HMI_FloatToSegment(float data)
{
    int  d;
    char d1,d2,d3,d4;
    unsigned long LCD_BUFFER0;
    unsigned long LCD_BUFFER1;
    
    d = (int)data;
    if (d < -999) d = -999;
    if (d > 9999) d = 9999;
    if (d < 0)
    {
       d  = -d;
       d1 = 10;
    }
    else
    {
       d1 = (char)(d/1000);
       d  = d - d1*1000;
    } 
    d2 = (char)(d/100);
    d  = d - d2*100;
    d3 = (char)(d/10);
    d  = d - d3*10;
    d4 = (char)d;
    
    d1 = DIS[d1];
    d2 = DIS[d2];
    d3 = DIS[d3];
    d4 = DIS[d4];   
    
    LCD_BUFFER0 = 0x07 & d4;
    LCD_BUFFER1 = 0x0F & (d4>>4);
    
    LCD_BUFFER0 = (LCD_BUFFER0 <<5 ) + (0x07 & d3);
    LCD_BUFFER1 = (LCD_BUFFER1 <<5 ) + (0x0F & (d3 >> 4));   
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) +(0x07&d2);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) +(0x0F&(d2>>4));    
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) +(0x07&d1);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) +(0x0F&(d1>>4));    
    
    LCD_BUFFER0 = LCD_BUFFER0 << 5;
    LCD_BUFFER1 = LCD_BUFFER1 << 5;      
    
    _DINT();
    
    LCD_P3_COM0_SEG = (char)(LCD_BUFFER0 & 0x0000FF)+DISPLAY_MARK1;   //P3 PORT COM0,  P31:PEAK,P32:CO,P33:S2,P34:S3
    LCD_P3_COM1_SEG = (char)(LCD_BUFFER1 & 0x0000FF)+DISPLAY_MARK2;   //P3 PORT COM1,  P32:O2,P33:S1,P34:S4
    
    LCD_BUFFER0     = LCD_BUFFER0 >> 8;
    LCD_BUFFER1     = LCD_BUFFER1 >> 8;
    
    LCD_P4_COM0_SEG = (char)(LCD_BUFFER0 & 0x00FF)+DISPLAY_MARK3;      //P4 PORT COM0,  P40:H2S,P44:2H
    LCD_P4_COM1_SEG = (char)(LCD_BUFFER1 & 0x00FF);
    
    LCD_BUFFER0     = LCD_BUFFER0>>8;
    LCD_BUFFER1     = LCD_BUFFER1>>8;   
    
    LCD_P5_COM0_SEG = (char)(LCD_BUFFER0&0xFF)+DISPLAY_MARK4;   //P5 PORT COM0,  P50:3H,P51:PPM,P55:LO,P56:S5,P57:ZERO
    LCD_P5_COM1_SEG = (char)(LCD_BUFFER1&0xFF)+DISPLAY_MARK5;   //P5 PORT COM1,  P51:%,P56:HI,P57:CAL
    
    _EINT();
}

/*! \fn void  HMI_DisplayPeak(void)
	\brief show the gas peak frame, to display peak value and flag.
*/
void  HMI_DisplayPeak(void)
{
    DISPLAY_MARK1 |=  DIS_PEAK;         
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_SPEAKER + DIS_ZERO + DIS_3H); 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);
    
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S 
    DISPLAY_MARK4 |=  DIS_PPM;
    HMI_FloatToSegment(g_uiGasPeak);   
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |= DIS_PERCENT;
    DISPLAY_MARK4 |= DIS_3H;
    HMI_FloatToSegmentWithOneDec(g_uiGasPeak);
#endif   
}

/*	\fn void  HMI_DisplayCode(char d1,char d2,char d3,char d4);
	\brief show the four characters in LCM.
	\param d1 is the first character defined in header file.
	\param d2 is the second character defined in header file.
	\param d3 is the third character defined in header file.
	\param d4 is the fourth character defined in header file.
*/
void  HMI_DisplayCode(char d1,char d2,char d3,char d4)
{
    unsigned long LCD_BUFFER0;
    unsigned long LCD_BUFFER1;
    
    LCD_BUFFER0 = 0x0F & d4;
    LCD_BUFFER1 = 0x0F & (d4 >> 4);
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 5) + (0x0F & d3);
    LCD_BUFFER1 = (LCD_BUFFER1 << 5) + (0x0F & (d3 >> 4));   
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) + (0x0F & d2);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) + (0x0F & (d2 >> 4));    
    
    LCD_BUFFER0 = (LCD_BUFFER0 << 4) + (0x0F & d1);
    LCD_BUFFER1 = (LCD_BUFFER1 << 4) + (0x0F & (d1 >> 4));    
    
    LCD_BUFFER0 = LCD_BUFFER0 << 5;
    LCD_BUFFER1 = LCD_BUFFER1 << 5;      
    
    _DINT();
    
    LCD_P3_COM0_SEG = (INT8U)(LCD_BUFFER0 & 0x0000FF); //P3 PORT COM0,  P31:PEAK,P32:CO,P33:S2,P34:S3
    LCD_P3_COM1_SEG = (INT8U)(LCD_BUFFER1 & 0x0000FF); //P3 PORT COM1,  P32:O2,P33:S1,P34:S4
    LCD_BUFFER0     = LCD_BUFFER0 >> 8;
    LCD_BUFFER1     = LCD_BUFFER1 >> 8;
    
    LCD_P4_COM0_SEG = (INT8U)(LCD_BUFFER0 & 0x00FF);   //P4 PORT COM0,  P40:H2S,P44:2H
    LCD_P4_COM1_SEG = (INT8U)(LCD_BUFFER1 & 0x00FF);
    LCD_BUFFER0     = LCD_BUFFER0 >> 8;
    LCD_BUFFER1     = LCD_BUFFER1 >> 8;   
    
    LCD_P5_COM0_SEG = (INT8U)(LCD_BUFFER0 & 0xFF);     //P5 PORT COM0,  P50:3H,P51:PPM,P55:LO,P56:S5,P57:ZERO
    LCD_P5_COM1_SEG = (INT8U)(LCD_BUFFER1 & 0xFF);     //P5 PORT COM1,  P51:%,P56:HI,P57:CAL
    
    _EINT();    
}

/*	\fn void  HMI_DisplayCodeAndMark(char d1,char d2,char d3,char d4)
	\brief show the four characters and flags in LCM.
	\param d1 is the first character defined in header file.
	\param d2 is the second character defined in header file.
	\param d3 is the third character defined in header file.
	\param d4 is the fourth character defined in header file.
*/
void  HMI_DisplayCodeAndMark(char d1,char d2,char d3,char d4)
{
    unsigned long LCD_BUFFER0;
    unsigned long LCD_BUFFER1;
    
    LCD_BUFFER0 = 0x0F&d4;
    LCD_BUFFER1 = 0x0F&(d4>>4);
    
    LCD_BUFFER0 = (LCD_BUFFER0<<5)+(0x0F&d3);
    LCD_BUFFER1 = (LCD_BUFFER1<<5)+(0x0F&(d3>>4));   
    
    LCD_BUFFER0 = (LCD_BUFFER0<<4)+(0x0F&d2);
    LCD_BUFFER1 = (LCD_BUFFER1<<4)+(0x0F&(d2>>4));    
    
    LCD_BUFFER0 = (LCD_BUFFER0<<4)+(0x0F&d1);
    LCD_BUFFER1 = (LCD_BUFFER1<<4)+(0x0F&(d1>>4));    
    
    LCD_BUFFER0 = LCD_BUFFER0<<5;
    LCD_BUFFER1 = LCD_BUFFER1<<5;      
    
    _DINT();
    
    LCD_P3_COM0_SEG = (char)(LCD_BUFFER0&0x0000FF)+DISPLAY_MARK1;  //P3 PORT COM0,  P31:PEAK,P32:CO,P33:S2,P34:S3
    LCD_P3_COM1_SEG = (char)(LCD_BUFFER1&0x0000FF)+DISPLAY_MARK2;  //P3 PORT COM1,  P32:O2,P33:S1,P34:S4
    LCD_BUFFER0     = LCD_BUFFER0>>8;
    LCD_BUFFER1     = LCD_BUFFER1>>8;
    
    LCD_P4_COM0_SEG = (char)(LCD_BUFFER0&0x00FF)+DISPLAY_MARK3;    //P4 PORT COM0,  P40:H2S,P44:2H
    LCD_P4_COM1_SEG = (char)(LCD_BUFFER1&0x00FF);
    LCD_BUFFER0     = LCD_BUFFER0>>8;
    LCD_BUFFER1     = LCD_BUFFER1>>8;   
    
    LCD_P5_COM0_SEG = (char)(LCD_BUFFER0&0xFF)+DISPLAY_MARK4;      //P5 PORT COM0,  P50:3H,P51:PPM,P55:LO,P56:S5,P57:ZERO
    LCD_P5_COM1_SEG = (char)(LCD_BUFFER1&0xFF)+DISPLAY_MARK5;      //P5 PORT COM1,  P51:%,P56:HI,P57:CAL 
    
    _EINT();
}

/*	\fn void  HMI_DisplayCodePowerOff(void)
	\brief show the "OFF" in LCM.
*/
void  HMI_DisplayCodePowerOff(void)
{
    HMI_DisplayCode(DIS_SPACE,DIS_0,DIS_F,DIS_F);
}

/*	\fn void  HMI_DisplayCodeFail(void)
	\brief show the "FAIL" in LCM.
*/
void  HMI_DisplayCodeFail(void)
{
}

/*	\fn void  HMI_DisplayCodeCal(INT8U ucFlash)
	\brief show the Calibration flag, and flash "CAL" in LCM.
	\param ucFlash decides whether show "CAL" string or no string, makes the string flash..
*/
void  HMI_DisplayCodeCal(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_LO + DIS_3H);    
    DISPLAY_MARK4 &=~ DIS_SPEAKER;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S    
    DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |= DIS_PERCENT;
#endif   
    DISPLAY_MARK5 &=~ DIS_HI;
    
    if (ucFlash)
    {   
        DISPLAY_MARK5 |=  DIS_CAL;
        HMI_DisplayCodeAndMark(DIS_C,DIS_A,DIS_L,0);  
    }
    else
    {
        DISPLAY_MARK5 |=  DIS_CAL;
        HMI_DisplayCodeAndMark(0,0,0,0);   
    }
}

void  HMI_DisplayDoCal(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_LO + DIS_3H);    
    DISPLAY_MARK4 &=~ DIS_SPEAKER;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S    
    DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |= DIS_PERCENT;
#endif     
    DISPLAY_MARK5 &=~ DIS_HI;
    
    if (ucFlash)
    {   
        DISPLAY_MARK5 |=  DIS_CAL;
        HMI_DisplayCodeAndMark(DIS_C,DIS_A,DIS_L,0);  
    }
    else
    {
        DISPLAY_MARK5 |=  DIS_CAL;
        HMI_DisplayCodeAndMark(DIS_d,DIS_o,0,0);   
    }
}

/*	\fn void  HMI_DisplayCodeDisp(INT8U ucFlash)
	\brief show the "DISP" string in LCM and flash.
	\param ucFlash decides whether show "DISP" string or no string, makes the string flash..
*/
void  HMI_DisplayCodeDisp(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
   
    DISPLAY_MARK4 &=~ DIS_ZERO;
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_3H); 
    DISPLAY_MARK4 &=~ DIS_PPM;
    DISPLAY_MARK4 &=~ DIS_SPEAKER;   
    
    DISPLAY_MARK5 &=~ DIS_CAL;        
    DISPLAY_MARK5 &=~ DIS_HI;
    DISPLAY_MARK5 &=~ DIS_PERCENT; 
    if (ucFlash)
    {  
      HMI_DisplayCodeAndMark(DIS_d,DIS_I,DIS_5,DIS_P);  
    }
    else
    {
      HMI_DisplayCodeAndMark(0,0,0,0);
    }
}

/*	\fn void  HMI_DisplayCodeLo(INT8U ucFlash)
	\brief show LOW alarm flags and flash "LOW" string.
	\param ucFlash decides whether show "LOW" string or no string, makes the string flash.
*/
void  HMI_DisplayCodeLo(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_3H); 
    DISPLAY_MARK5 &=~ DIS_HI;
    DISPLAY_MARK5 &=~ DIS_CAL;
#if SENSOR_TYPE == SENSOR_CO ||  SENSOR_TYPE == SENSOR_H2S    
    DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |=  DIS_PERCENT;
#endif    
    
    if (ucFlash)
    {
        DISPLAY_MARK4 |=  DIS_SPEAKER;
        DISPLAY_MARK4 |=  DIS_LO;   
        HMI_DisplayCodeAndMark(DIS_L,DIS_0,0,0);  
    }
    else
    {
        DISPLAY_MARK4 &=~ DIS_SPEAKER;
        DISPLAY_MARK4 &=~ DIS_LO;    
        HMI_DisplayCodeAndMark(0,0,0,0);  
    }   
}

/*	\fn void  HMI_DisplayCodeHi(INT8U ucFlash)
	\brief show High alarm flags and flash "Hi" string.
	\param ucFlash decides whether show "Hi" string or no string, makes the string flash.
*/
void  HMI_DisplayCodeHi(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ DIS_ZERO; 
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_3H);
#if SENSOR_TYPE == SENSOR_CO ||  SENSOR_TYPE == SENSOR_H2S 
    DISPLAY_MARK4 |=  DIS_PPM;   
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |=  DIS_PERCENT;
#endif   
    DISPLAY_MARK5 &=~ DIS_CAL;        
    
    if (ucFlash)
    {
        DISPLAY_MARK5 |=  DIS_HI;
        DISPLAY_MARK4 |=  DIS_SPEAKER;      
        HMI_DisplayCodeAndMark(DIS_H,DIS_1,0,0); 
    }
    else
    {
        DISPLAY_MARK5 &=~ DIS_HI;
        DISPLAY_MARK4 &=~ DIS_SPEAKER;        
        HMI_DisplayCodeAndMark(0,0,0,0);  
    }     
}

/*	\fn void  HMI_DisplayCodeZeroPrepare(INT8U ucFlash)
	\brief show Zero flag and current Gas value, and flash them.
	\param ucFlash decides whether show Zero flag and current value, makes them flash.
*/
void  HMI_DisplayCodeZeroPrepare(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK5 &=~ DIS_HI;
    DISPLAY_MARK5 &=~ DIS_CAL;        
    DISPLAY_MARK4 &=~ (DIS_SPEAKER + DIS_3H);
    DISPLAY_MARK4 &=~ DIS_LO;     
      
    if (ucFlash)
    {
      DISPLAY_MARK4 |=  DIS_ZERO;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S       
      HMI_FloatToSegment(g_fGasCurVal);       
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(g_fGasCurVal);
#endif      
    }
    else
    {
      DISPLAY_MARK4 &=~ DIS_ZERO;      
      HMI_DisplayCodeAndMark(0,0,0,0);
    } 
}

/*	\fn void  HMI_DisplayCodeZeroRunning(INT8U ucFlash)
	\brief show "0000" string and zero flag and flash it.
	\param ucFlash decides whether show "0000" string, makes them flash.
*/
void  HMI_DisplayCodeZeroRunning(INT8U ucFlash)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK5 &=~ DIS_HI;
    DISPLAY_MARK5 &=~ DIS_CAL;        
    DISPLAY_MARK4 &=~ (DIS_SPEAKER + DIS_3H);
    DISPLAY_MARK4 &=~ DIS_LO;     
      
    if (ucFlash)
    {
      DISPLAY_MARK4 |=  DIS_ZERO;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S       
      HMI_FloatToSegment(g_fGasCurVal);       
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(g_fGasCurVal);
#endif      
    }
    else
    {
      DISPLAY_MARK4 &=~ DIS_ZERO;      
      HMI_DisplayCodeAndMark(0,0,0,0);
    }       
}

/*	\fn void  HMI_DisplayCodeZeroFinish(INT8U ucFlash)
	\brief show "0000" string and zero flag and flash it.
	\param ucFlash decides whether show "0000" string, makes them flash.
*/
void  HMI_DisplayCodeZeroFinish(INT8U ucFlash)
{
}

/*	\fn void HMI_DisplayCodeCalPrepare(INT8U ucFlash)
	\brief show Calibration flag and gas reading value.
	\param ucFlash decides whether show Calibration flag and gas reading value, makes them flash.
*/
void  HMI_DisplayCodeCalPrepare(INT8U ucFlash)
{
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_3H);
    if (ucFlash)
    {
      DISPLAY_MARK5 &=~ DIS_CAL;
      HMI_DisplayCodeAndMark(DIS_SPACE,DIS_SPACE,DIS_SPACE,DIS_SPACE);
    }
    else
    {
      DISPLAY_MARK5 |= DIS_CAL;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
      HMI_FloatToSegment(g_fGasCal); 
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(g_fGasCal);
#endif      
    }   
}

/*	\fn void  HMI_DisplayCodeCalRunning(INT8U ucFlash)
	\brief show Calibration flag and gas reading value.
	\param ucFlash decides whether show gas reading value, makes them flash.
*/
void  HMI_DisplayCodeCalRunning(INT8U ucFlash)
{
    DISPLAY_MARK4 &=~ (DIS_3H + DIS_ZERO);
    DISPLAY_MARK5 |= DIS_CAL;
    
    if (ucFlash)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
    HMI_FloatToSegment(g_fGasCalSpan);   
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK4 |= DIS_3H;
    HMI_FloatToSegmentWithOneDec(g_fGasCalSpan);
#endif      
    }
    else
    {
        HMI_DisplayCodeAndMark(0,0,0,0);
    } 
}

/*	\fn void  HMI_DisplayCodeCalFinish(INT8U ucFlash)
	\brief show Sensor status or sensor SPAN
	\param ucFlash decides show status or SPAN.
*/
void  HMI_DisplayCodeCalFinish(INT8U ucFlash)
{
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_3H);
    DISPLAY_MARK5 |= DIS_CAL;
    if (ucFlash)
    {
        if (g_ucSensorStatus == SENSOR_GOOD)
        {
            HMI_DisplayCodeAndMark(DIS_9,DIS_o,DIS_o,DIS_d);
        }
        else
        {
            HMI_DisplayCodeAndMark(DIS_F,DIS_A,DIS_I,DIS_L);
        }
    }
    else
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
    HMI_FloatToSegment(g_fGasCalSpan); 
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK4 |= DIS_3H;
    HMI_FloatToSegmentWithOneDec(g_fGasCalSpan);
#endif      
    }  
}

/*	\fn void HMI_DisplayNormal(void)
	\brief show gas reading value or "SAFE" string, according system display mode.
*/
void  HMI_DisplayNormal(void)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_SPEAKER + DIS_ZERO+ DIS_3H); 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);      
    if (g_ucDisplayMod)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S     
      DISPLAY_MARK4 &=~ DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK5 &=~ DIS_PERCENT;
#endif        
      HMI_DisplayCodeAndMark(DIS_5,DIS_A,DIS_F,DIS_E);
    }
    else
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S 
      DISPLAY_MARK4 |= DIS_PPM;
      DISP_FloatToSegment(g_fGasCurVal); 
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK5 |= DIS_PERCENT;
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(g_fGasCurVal);
#endif
    }
}

/*	\fn void  HMI_DisplayNormalOverLo(void)
	\brief show "-OR" string, if PPM mode, show "PPM" flag.
*/
void  HMI_DisplayNormalOverLo(void)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_SPEAKER + DIS_ZERO + DIS_3H); 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);       
    if (g_ucDisplayMod)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
        DISPLAY_MARK4 &=~ DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 &=~ DIS_PERCENT;
#endif      
        HMI_DisplayCodeAndMark(DIS_Minus,DIS_0D,DIS_rD,DIS_SPACE);
    }
    else
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
        DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 |= DIS_PERCENT;
#endif      
        HMI_DisplayCodeAndMark(DIS_Minus,DIS_0D,DIS_rD,DIS_SPACE); 
    }
}

/*	\fn void  HMI_DisplayNormalOverHi(void)
	\brief show "OR" string, if PPM mode, show "PPM" flag.
*/
void  HMI_DisplayNormalOverHi(void)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_SPEAKER + DIS_ZERO + DIS_3H); 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);       
    if (g_ucDisplayMod)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S     
        DISPLAY_MARK4 &=~ DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 &=~ DIS_PERCENT;
#endif      
        HMI_DisplayCodeAndMark(DIS_SPACE,DIS_0D,DIS_rD,DIS_SPACE);
    }
    else
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S     
        DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 |= DIS_PERCENT;
#endif      
        HMI_DisplayCodeAndMark(DIS_SPACE,DIS_0D,DIS_rD,DIS_SPACE);
    }
}

/*	\fn void  HMI_DisplayNormalSpan(void)
	\brief show Zero and Calibration flag, and SPAN value.
*/
void  HMI_DisplayNormalSpan(void)
{
}

/*	\fn void  HMI_DisplayNormalHi(void)
	\brief show High alarm, and "Hi" string or gas value according system display mode.
*/
void  HMI_DisplayNormalHi(void)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_ZERO + DIS_3H);
    DISPLAY_MARK4 |=  DIS_SPEAKER; 
    DISPLAY_MARK5 &=~ DIS_CAL;
    DISPLAY_MARK5 |=  DIS_HI;        
    if (g_ucDisplayMod)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
        DISPLAY_MARK4 &=~ DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 &=~ DIS_PERCENT;
#endif      
        HMI_DisplayCodeAndMark(DIS_SPACE,DIS_H,DIS_I,DIS_SPACE);
    }
    else
    {      
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S 
        DISPLAY_MARK4 |=  DIS_PPM;
        HMI_FloatToSegment(g_fGasCurVal); 
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 |= DIS_PERCENT;
        DISPLAY_MARK4 |= DIS_3H;
        HMI_FloatToSegmentWithOneDec(g_fGasCurVal);
#endif        
    }
}

/*	\fn void  HMI_DisplayNormalLo(void)
	\brief show Low alarm, and "Lo" string or gas value according system display mode.
*/
void  HMI_DisplayNormalLo(void)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_ZERO + DIS_3H);
    DISPLAY_MARK4 |=  DIS_LO + DIS_SPEAKER; 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);        
    if (g_ucDisplayMod)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
        DISPLAY_MARK4 &=~ DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 &=~ DIS_PERCENT;
#endif        
        HMI_DisplayCodeAndMark(DIS_SPACE,DIS_L,DIS_0,DIS_SPACE);
    }
    else
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
        DISPLAY_MARK4 |= DIS_PPM;
        HMI_FloatToSegment(g_fGasCurVal); 
#elif SENSOR_TYPE == SENSOR_O2
        DISPLAY_MARK5 |= DIS_PERCENT;
        DISPLAY_MARK4 |= DIS_3H;
        HMI_FloatToSegmentWithOneDec(g_fGasCurVal);
#endif      
    }
}

/*	\fn void  HMI_DisplayPresetHi((INT8U ucFlash, float fDispData).
	\brief show pre-set gas High alarm threshold value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetHi(INT8U ucFlash, float fDispData)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    
    DISPLAY_MARK4 &=~ (DIS_ZERO+DIS_3H); 
    DISPLAY_MARK4 &=~ DIS_LO;   
    
    DISPLAY_MARK4 |=  DIS_SPEAKER;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S    
    DISPLAY_MARK4 |=  DIS_PPM;   
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |= DIS_PERCENT;
#endif   
    DISPLAY_MARK5 |=  DIS_HI;
    DISPLAY_MARK5 &=~ DIS_CAL;        
    
    if (ucFlash)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S      
      DISP_FloatToSegment(fDispData);  
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(fDispData);
#endif
    }
    else
    {
      HMI_DisplayCodeAndMark(0,0,0,0);  
    }
}

/*	\fn void  HMI_DisplayPresetLo(void)
	\brief show pre-set gas Low alarm threshold value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetLo(INT8U ucFlash, float fDispData)
{
    DISPLAY_MARK1 &=~ DIS_PEAK; 
    DISPLAY_MARK4 &=~ (DIS_ZERO+DIS_3H); 
    DISPLAY_MARK5 &=~ (DIS_HI + DIS_CAL);  
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S   
    DISPLAY_MARK4 |=  (DIS_SPEAKER + DIS_LO + DIS_PPM);
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK4 |= (DIS_SPEAKER + DIS_LO);
    DISPLAY_MARK5 |= DIS_PERCENT;
#endif   
    if (ucFlash)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S           
      HMI_FloatToSegment(fDispData); 
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(fDispData);
#endif      
    }
    else
    {
      HMI_DisplayCodeAndMark(0,0,0,0);  
    }
}

/*	\fn void   HMI_DisplayPresetCal(INT8U ucFlash, float fDispData)
	\brief show pre-set gas Calibration value.
	\param ucFlash makes the value flash.
	\param fDispData the value is to be displayed.
*/
void  HMI_DisplayPresetCal(INT8U ucFlash, float fDispData)
{
    DISPLAY_MARK1 &=~ DIS_PEAK;        
    DISPLAY_MARK4 &=~ (DIS_LO + DIS_ZERO + DIS_3H);       
    DISPLAY_MARK5 |=  DIS_CAL;           
        
    DISPLAY_MARK4 &=~ DIS_SPEAKER;
    DISPLAY_MARK5 &=~ DIS_HI;
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S    
    DISPLAY_MARK4 |=  DIS_PPM;
#elif SENSOR_TYPE == SENSOR_O2
    DISPLAY_MARK5 |=  DIS_PERCENT;   
#endif
    
    if (ucFlash)
    {
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S     
      HMI_FloatToSegment(fDispData);  
#elif SENSOR_TYPE == SENSOR_O2
      DISPLAY_MARK4 |= DIS_3H;
      HMI_FloatToSegmentWithOneDec(fDispData);
#endif       
    }
    else
    {
      HMI_DisplayCodeAndMark(0,0,0,0);       
    }
}

/*	\fn void  HMI_DisplayPresetDispMod(INT8U ucFlash, INT8U ucDisMode)
	\brief show system pre-set display mode as "OFF" or "ON"
	\param ucFlash makes the value flash.
	\param ucDisMode decides the "OFF" or "ON" string.
*/
void  HMI_DisplayPresetDispMod(INT8U ucFlash, INT8U ucDisMode)
{
    DISPLAY_MARK1 &=~ DIS_PEAK;        
    DISPLAY_MARK4 &=~ DIS_LO;
    DISPLAY_MARK4 &=~ DIS_ZERO;       
    DISPLAY_MARK4 &=~ DIS_PPM;      
    DISPLAY_MARK4 &=~ (DIS_SPEAKER + DIS_3H);   
    DISPLAY_MARK5 &=~ DIS_CAL;           
    DISPLAY_MARK5 &=~ DIS_HI;
    DISPLAY_MARK5 &=~ DIS_PERCENT; 
    
    if(ucFlash)
    {
        if (ucDisMode)
        {
            HMI_DisplayCodeAndMark(0,DIS_0,DIS_F,DIS_F);
        }
        else
        {
            HMI_DisplayCodeAndMark(0,0,DIS_0,DIS_n); 
        }
    }  
    else
    {
        HMI_DisplayCodeAndMark(0,0,0,0);       
    } 
}