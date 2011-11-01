#include "Alarm.h"

void Alarm_Init(void)
{
    P1SEL &=~ SOUND_ALARM;
    P1OUT &=~ SOUND_ALARM;     
    P1DIR |=  SOUND_ALARM;
   
    P1SEL &=~ LED_ALARM;             // SET P1.6 TO I/O PIN FOR LED ALARM
    P1OUT &=~ LED_ALARM;             // SET LED ALARM START STATUS
    P1DIR |=  LED_ALARM;             // LED ALARM DISABLE
   
    P2SEL &=~ VIBRATION_ALARM;       // SET P2.5 TO I/O PIN FOR VIBRATION ALARM
    P2OUT &=~ VIBRATION_ALARM;       // SET VIBRATION ALARM START STATUS
    P2DIR |=  VIBRATION_ALARM;       // VIBRATION ALARM DISABLE 
}

void Alarm_BuzzerOn(unsigned char flag)
{
    P1DIR |= 0x0C;
    P1SEL |= 0x0C;
    if(flag != 0) // the highest sound
    {
        TACCR0 = 0x0C;      //10ms
        CCTL1 = OUTMOD_7;
        CCR1 = 0x05;

        CCTL2 = OUTMOD_7;
        CCR2 = 0x06;    
        TACTL = TASSEL_1 + MC_1; 
    }
    else
    {
        TACCR0 = 0x9;      //10ms

        CCTL1 = OUTMOD_7;
        CCR1 = 0x04;

        CCTL2 = OUTMOD_7;
        CCR2 = 0x05;    
        TACTL = TASSEL_1 + MC_1;
    }
}

void Alarm_BuzzerOff(void)
{
    TACTL = TASSEL_1 + MC_0;
    P1SEL &=~ SOUND_ALARM;
    P1OUT &=~ SOUND_ALARM;    
    P1DIR |=  SOUND_ALARM;
}

void Alarm_LedOn(void)
{
    P1SEL &=~ LED_ALARM;
    P1OUT |=  LED_ALARM;    
    P1DIR |=  LED_ALARM; 
}

void Alarm_LedOff(void)
{
    P1SEL &=~ LED_ALARM;
    P1OUT &=~ LED_ALARM;    
    P1DIR |=  LED_ALARM;   
}

void Alarm_VibOn(void)
{
    P2SEL &=~ VIBRATION_ALARM;
    P2OUT |=  VIBRATION_ALARM;    
    P2DIR |=  VIBRATION_ALARM;  
}

void Alarm_VibOff(void)
{
    P2SEL &=~ VIBRATION_ALARM;
    P2OUT &=~ VIBRATION_ALARM;    
    P2DIR |=  VIBRATION_ALARM; 
}

void Alarm_Off(void)
{
    Alarm_LedOff();
    Alarm_VibOff();
    Alarm_BuzzerOff();
}

void Alarm_Beep(void)
{
}