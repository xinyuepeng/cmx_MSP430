#include "KeyInput.h"

struct KeyInputQueue g_sKey;
INT16U   KEY_TIMER;
INT16U   BACKLIGHT_TIMER;

void Key_Init(void)
{
    // Init_COMPARATOR_A
    /*
     compareA init for shutdown. ?? check it
    */
    CACTL1 = CARSEL + CAREF1 + CAON; 
    CACTL2 = P2CA0 + CAF;
    
    P2SEL |= COMPARATOR_IN;          // SET P2.3 FOR COMPARATOR INPUT PIN 
    P2DIR &=~COMPARATOR_IN;          // SET P2.3 TO INPUT DIRECTION. 
    
    
    g_sKey.ScanState = KEY_STATE_UP;            // Keyboard should not have a key pressed
    g_sKey.Number    = 0;                       // Clear the number of keys read 
    g_sKey.BufInIx   = 0;                       // Key codes inserted at  the beginning of the buffer
    g_sKey.BufOutIx  = 0;                       // Key codes removed from the beginning of the buffer
    g_sKey.Time      = 0;
    g_sKey.Pressed   = 0;
    KEY_TIMER        = 0;
}

BOOLEAN  Key_Hit(void)
{
    BOOLEAN hit;

    hit = (BOOLEAN)(g_sKey.Number > 0) ? TRUE : FALSE;

    return (hit);
}

INT8U  Key_Decode (void)
{
   INT8U ucKeyCode;
   
   ucKeyCode  = (P2IN & SELECT_SWITCH) + (!(CACTL2 & 0x01));
   return (ucKeyCode);                          // Return scan code
}

INT16U Key_PressedTime(void)
{
   return (g_sKey.Time);  
}

BOOLEAN  Key_Pressed (void)
{
   if (Key_Decode())                            // Key not pressed if 0
   {                                              
      return (1);
   }
   else
   {
      return (0);
   }
}

void  Key_PushKey (INT8U ucCode)
{
    if (g_sKey.Number < KEY_BUF_SIZE)
    {                                           // Make sure that we don't overflow the buffer    
        g_sKey.Number++;                        // Increment the number of keys read  
        g_sKey.Buf[g_sKey.BufInIx++] = ucCode;  // Store the scan code into the buffer 
        if (g_sKey.BufInIx >= KEY_BUF_SIZE)
        {                                       // Adjust index to the next scan code to put in buffer
            g_sKey.BufInIx = 0;
        }
    }
}

INT8U  Key_PopKey (void)
{
   INT8U ucCode = 0;

   if (g_sKey.Number > 0)
   {                                            // See if we have keys in the buffer 
      g_sKey.Number--;                          // Decrement the number of keys read 
      ucCode = g_sKey.Buf[g_sKey.BufOutIx];     // Get scan code from the buffer     
      
      g_sKey.BufOutIx++;
      if (g_sKey.BufOutIx >= KEY_BUF_SIZE)
      {                                         // Adjust index into the keyboard buffer        
         g_sKey.BufOutIx = 0;
      }
   }   
   
   return (ucCode);                             // Return the scan code of the key pressed  
}

INT8U Key_ReadKey(void)
{
    INT8U ucCode = 0;

    if (g_sKey.Number > 0)
    {                                            // See if we have keys in the buffer 
      ucCode = g_sKey.Buf[g_sKey.BufOutIx];     // Get scan code from the buffer     
    }   
    
    return (ucCode);      
}

void  Key_ClearBuf (void)
{
    while (Key_Hit())
    {                                           // While there are keys in the buffer...
        Key_PopKey();                           // ... extract the next key from the buffer 
    }
}

void Key_ClearTime(void)
{
   g_sKey.Time = 0;   
}

void  Task_KeyScan (void)
{
   static INT8U ucKeyCodeHolder = 0X00;
          INT8U ucKeyCode       = 0X00;
   
   CACTL1 = CARSEL + CAREF1 + CAON; 
   CACTL2 = P2CA0 + CAF;
   
   switch (g_sKey.ScanState)
    {
    case KEY_STATE_UP:                                    // See if need to look for a key pressed
        if (Key_Pressed())
        {			                                          // See if key is pressed 
            g_sKey.ScanState = KEY_STATE_DEBOUNCE;          // Next call we will have debounced the key
            ucKeyCodeHolder  = 0X00;
            }
        g_sKey.Time      = 0;         
        break;
    
    case KEY_STATE_DEBOUNCE:                              // Key pressed, get scan code and buffer
        if (Key_Pressed())
        {		                                             // See if key is pressed 
            ucKeyCode = Key_Decode();                       // Determine the key scan code 
            if (ucKeyCode != ucKeyCodeHolder)
            {   
                Key_PushKey(ucKeyCode);                      // Input scan code in buffer   
                ucKeyCodeHolder = ucKeyCode;
            }
            g_sKey.Time    += KEY_SCAN_ALT;
            g_sKey.Pressed  = 1;
        }
        else
        {
            g_sKey.ScanState  = KEY_STATE_UP;            // Key was not pressed after all! 
            g_sKey.Pressed    = 0;
        }
        break;
    default:
    Key_Init();
    break;   
    }
   CACTL1 &=~ (CAREF1 + CAREF0);//CAON);
   if (Key_Hit()) 
     EnableBackLight(30);
}

void EnableBackLight(unsigned char time)
{
    P1SEL &=~ BACK_LIGHT;       
    P1DIR |=  BACK_LIGHT;        
    P1OUT |=  BACK_LIGHT;
    
    BACKLIGHT_TIMER = time;
}

void DisableBackLight(void)
{
    P1SEL &=~ BACK_LIGHT;        
    P1DIR |=  BACK_LIGHT;        
    P1OUT &=~ BACK_LIGHT;
    BACKLIGHT_TIMER = 0;
}