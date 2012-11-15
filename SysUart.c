#include "SysUart.h"

USART_STRUCT receive0BufferUSART;

USART_STRUCT receive1BufferUSART;

void InitUart(void)
{
    //
    //transmit0BufferUSART.counter = transmit0BufferUSART.size = 0;
    receive0BufferUSART.counter = receive0BufferUSART.size = 0;
    //transmit1BufferUSART.counter = transmit1BufferUSART.size = 0;
    receive1BufferUSART.counter = receive1BufferUSART.size = 0;
    
    // Init hardware  
    P3SEL |= 0x30;                           // P3.4,5 = USART0 TXD/RXD
    P3DIR |= BIT4;
  
    P3SEL |= 0xC0;                           // P3.6,7 = USART1 TXD/RXD
    P3DIR |= BIT6;
    
    P4DIR = 0X03;
    P4OUT = 0XFF;

    ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.4
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4a;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;    
    
    ME2 |= UTXE1 + URXE1;                      // Enable USART1 TXD/RXD
    UCTL1 |= CHAR;                            // 8-bit character
    UTCTL1 |= SSEL0;                          // UCLK = ACLK
    UBR01 = 0x03;                             // 32k/9600 - 3.4
    UBR11 = 0x00;                             //
    UMCTL1 = 0x4a;                            // Modulation
    UCTL1 &= ~SWRST;                          // Initialize USART state machine
    IE2 |= URXIE1;
    
    _BIS_SR(GIE);                             // Enter interrupt
}


/****************************************************************************** 
函数名称：usart0_rx
功    能：串行接收中断
参    数：无
           
返回值  ：无  
******************************************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void usart0_rx (void)
{
     //U0TXBUF=RX_DATA;                          //把接收到的数据返回
     if ( receive0BufferUSART.counter < USART_BUFFER_LENGTH )
     {
        receive0BufferUSART.bytesBuffer[receive0BufferUSART.counter] = RXBUF0;
        receive0BufferUSART.counter++;
        receive0BufferUSART.size++;
     }
}

/****************************************************************************** 
函数名称：usart1_rx
功    能：串行接收中断
参    数：无
           
返回值  ：无  
******************************************************************************/
#pragma vector=UART1RX_VECTOR
__interrupt void usart1_rx (void)
{
     //unsigned char RX_DATA;
     //RX_DATA=U1RXBUF;
     //U1TXBUF=RX_DATA;                          //把接收到的数据返回
    if ( receive1BufferUSART.counter < USART_BUFFER_LENGTH )
     {
        receive1BufferUSART.bytesBuffer[receive1BufferUSART.counter] = RXBUF1;
        receive1BufferUSART.counter++;
        receive1BufferUSART.size++;
     }
}

//------------------------------------------------------------------------------
//   AT91USARTEnableRx(void)
//
//   Author:   Sufwan Sadiq
//   Date:     2011/01/01
//
//!  This function enables USART0 for receiving data  
//
//------------------------------------------------------------------------------

void MSP430USARTEnableRx(unsigned char UartNum)
{
#if 0  
    // Enable the 485 in input mode
    MSP430GPIOOutputLow((unsigned int)MSP430_PORT_3,(unsigned int)PIN_2);
    // Disable TX interrupt
    IE1 &= (unsigned char)(~UTXIE0);
    // Enable USART receive module and receive interrupt
    IE1 |= (unsigned char)URXIE0;
#endif    
    if( UART0 == UartNum )
    {
        // Disable TX interrupt
        IE1 &= (unsigned char)(~UTXIE0);
        // Enable USART receive module and receive interrupt
        IE1 |= (unsigned char)URXIE0;
    }
    else if( UART1 == UartNum )
    {
        // Disable TX interrupt
        IE2 &= (unsigned char)(~UTXIE1);
        // Enable USART receive module and receive interrupt
        IE2 |= (unsigned char)URXIE1;
    }
}

//------------------------------------------------------------------------------
//   AT91USARTEnableTx(void)
//
//   Author:  Sufwan Sadiq
//   Date:    2011/01/01
//
//!  This function enable USART0 for transmitting data from transmit buffer
//
//------------------------------------------------------------------------------

void MSP430USARTEnableTx(unsigned char UartNum)
{
#if 0  
    //Set 485 in output mode
    MSP430GPIOOutputHigh((unsigned char)MSP430_PORT_3,(unsigned char)PIN_2);

    // Receive function for USART0 is disabled
    MSP430USARTDisableRx();
#endif   
    if( UART0 == UartNum )
    {
      MSP430USARTDisableRx(UART0);
    }
    else if( UART1 == UartNum )
    {
      MSP430USARTDisableRx(UART1);
    }
}

//------------------------------------------------------------------------------
//   MSP430USARTDisableRx(void)
//
//   Author:  Sufwan Sadiq
//   Date:    2011/01/01
//
//!  This function disables USART0 for receiving data
//
//------------------------------------------------------------------------------

void MSP430USARTDisableRx(unsigned char UartNum)
{
    if( UART0 == UartNum )
    {
        // Disable USART  receive interrupt
        IE1 &= (unsigned char)(~URXIE0);
        IFG1 &= (unsigned char)(~URXIFG0);
    }
    else if( UART1 == UartNum )
    {
        // Disable USART  receive interrupt
        IE2 &= (unsigned char)(~URXIE1);
        IFG2 &= (unsigned char)(~URXIFG1);
    }
}

//------------------------------------------------------------------------------
//   MSP430USARTResetRxData(void)
//
//   Author:   Sufwan Sadiq
//   Date:     2011/01/01
//
//!  This function resets variables which are used while data is transferred 
//!  from USART0 to receive buffer
//
//------------------------------------------------------------------------------

void MSP430USARTResetRxData(unsigned char UartNum)
{
    if( UART0 == UartNum )
    {
        // Reset counter value to zero
        receive0BufferUSART.counter = 0;
        // Reset size value to zero
        receive0BufferUSART.size = 0;
    }
    else if( UART1 == UartNum )
    {
        // Reset counter value to zero
        receive1BufferUSART.counter = 0;
        // Reset size value to zero
        receive1BufferUSART.size = 0;
    }
}

//------------------------------------------------------------------------------
//   MSP430USARTResetTxData(void)
//
//   Author: Sufwan Sadiq
//   Date:   2011/01/01
//
//!  This function resets the variables which are used in transmitting
//!  data from USART0 buffer
//
//------------------------------------------------------------------------------

void MSP430USARTResetTxData(unsigned char UartNum)
{
    if( UART0 == UartNum )
    {
        // Reset counter value to zero
        //transmit0BufferUSART.counter = 0;
        // Reset size value zero 
        //transmit0BufferUSART.size = 0;
    }
    else if( UART1 == UartNum )
    {
        // Reset counter value to zero
        //transmit1BufferUSART.counter = 0;
        // Reset size value zero 
        //transmit1BufferUSART.size = 0;
    }
}

int MSP430UARTSendData(unsigned char UartNum, unsigned char *buf, unsigned char len)
{
    unsigned int localCounter;
    
    if(UART0 == UartNum)
    {
        for (localCounter = 0; localCounter < len ;localCounter++ )
        {
            while ((IFG1 & (unsigned int)UTXIFG0) != (unsigned int)UTXIFG0)
            {
              ;
            }
            TXBUF0 = buf[localCounter];
            //Wait usart0 shift out
            while ((UTCTL0 & (unsigned int)TXEPT) != (unsigned int)TXEPT)
            {
              ;
            }
        }
    }
    else if(UART1 == UartNum)
    {
        for (localCounter = 0; localCounter < len ;localCounter++ )
        {
            while ((IFG2 & (unsigned int)UTXIFG1) != (unsigned int)UTXIFG1)
            {
              ;
            }
            TXBUF1 = buf[localCounter];
            //Wait usart0 shift out
            while ((UTCTL1 & (unsigned int)TXEPT) != (unsigned int)TXEPT)
            {
              ;
            }
        }
    }
}