#include "PowerClockControl.h"

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
    TBCCTL0 = CCIE;
    TBCTL  &=~ 0x30;
    //TBCCR0  =  327;      // SYSTEM CLOCK INTERRUPT PERIOD 10 ms( (1000*328)/32768  )
    //TBCTL   =  0x0116;   // ACLK

    TBCCR0 = 327;//0xA4;      //10ms
    TBCTL = TBSSEL_1 + MC_1 + TAIE;

    //Init Power control pin
    P1SEL &=~POWER_SWITCH;           // SET P1.5 FOR POWER SWITCH PIN
    P1DIR |= POWER_SWITCH;           // SET P1.5 TO OUPUT DIRECTION
    P1OUT &=~POWER_SWITCH;           // SET POWER SWITCH TO OFF STATUS
    
    // Init LCD SEGMENT
    P2SEL &=~0x80;             // SELECT P2.7 FOR I/O  (COM0) 
    P3SEL &=~0x00;             // SELECT P3.0(COM1) TO P3.7 FOR I/O

    P4SEL  = 0x00;             // SELECT P4.0 TO P4.7 FOR I/O
    P4DIR  = 0xFF;             // SET P4 TO OUTPUT 
}