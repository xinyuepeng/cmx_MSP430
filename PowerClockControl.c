#include "PowerClockControl.h"
#include "Alarm.h"

static unsigned char bEnableLowPowerMode = 0;
void InitPowerClockController(void)
{  
    //Init system clock registers
    DCOCTL  = 0x3C;                  // DCO=3C   
    /*
     RSEL = 7, select the highest FQ.
    */
    BCSCTL1 = 0X87;                  // XT2=OFF, RSEL=7
    BCSCTL2 = 0X00;                  // MCLK = DCOCLK, SMCLK = DCOCLK, = EXT CRYTAL
  
    //Init timerB as ticks
    TBCTL  &=~ 0x30;
    TBCCR0  =  327;      // SYSTEM CLOCK INTERRUPT PERIOD 10 ms( (1000*328)/32768  )
    TBCTL   =  0x0116;   // ACLK
    //TBCCR0 = 0xA4;      //10ms
    //TBCTL = TBSSEL_1 + MC_1 + TAIE;

    //Init Power control pin
    P1SEL &=~POWER_SWITCH;           // SET P1.5 FOR POWER SWITCH PIN
    P1DIR |= POWER_SWITCH;           // SET P1.5 TO OUPUT DIRECTION
    P1OUT &=~POWER_SWITCH;           // SET POWER SWITCH TO OFF STATUS
    
    // Init LCD SEGMENT
    P2SEL &=~0x80;             // SELECT P2.7 FOR I/O  (COM0) 
    P3SEL &=~0x00;             // SELECT P3.0(COM1) TO P3.7 FOR I/O

    P4SEL  = 0x00;             // SELECT P4.0 TO P4.7 FOR I/O
    P4DIR  = 0xFF;             // SET P4 TO OUTPUT 

    P5SEL  = 0x00;             // SELECT P5.0 TO P5.7 FOR I/O 
    P5DIR  = 0xFF;             // SET P5 TO OUTPUT   
    
    //Key Pin Init
    P1SEL &=~POWER_SWITCH;           // SET P1.5 FOR POWER SWITCH PIN
    P1DIR |= POWER_SWITCH;           // SET P1.5 TO OUPUT DIRECTION
    P1OUT &=~POWER_SWITCH;           // SET POWER SWITCH TO OFF STATUS

#ifndef HW_SPIDER_1_0   
    P1SEL &=~AUTOCAL_SWITCH;         // SET P1.3 FOR AUTOCAL SWITCH INPUT PIN 
    P1DIR &=~AUTOCAL_SWITCH;         // SET P1.3 TO INPUT DIRECTION.
#endif
   
    P2SEL &=~SELECT_SWITCH;          // SET P2.4 FOR SELECT SWITCH INPUT PIN 
    P2DIR &=~SELECT_SWITCH;          // SET P2.4 TO INPUT DIRECTION.
    
    //For ADC Pin configure    
#if SENSOR_TYPE == SENSOR_CO || SENSOR_TYPE == SENSOR_H2S
    P6SEL |= 0X3E;                   // SET P6.1,P6.2,P6.3,P6.4,P6.5 FOR ADC PIN
    P6DIR &=~0X3E;                   // SET P6.1,P6.2,P6.3,P6.4,P6.5 TO INPUT DIRECTION
#elif SENSOR_TYPE == SENSOR_O2
    P6SEL |= 0X33;                   // SET P6.0, P6.1, P6.4, P6.5 FOR ADC PIN 
    P6DIR &=~0X33;                   // SET P6.0, P6.1, P6.4, P6.5 To Input Direction
#endif   
}

void EnablePowerOn(void)
{
    P1OUT  |= POWER_SWITCH; 
}

/*! \fn void SystemPowerOff(void)
    \brief shutdown the system.
 */
void SystemPowerOff(void)
{
   _DINT();                         // GLOBE INTERRUPT DISABLE
   P1DIR &=~ BACK_LIGHT; 
   P1OUT &=~ POWER_SWITCH;          // POWER OFF.  
   while (1) {}   
}

void EnableSysLowPowerMode()
{
    bEnableLowPowerMode = 1;
}

static unsigned char switch_flag = 0;

void SystemLowPowerMode()
{
    switch_flag = !switch_flag;
    if(switch_flag)
        Alarm_LedOn();
    else
        Alarm_LedOff();
}