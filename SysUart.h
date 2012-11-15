#ifndef SYSUART_H
#define SYSUART_H

#include "SysConfig.h"

#define USART_BUFFER_LENGTH         24      //!< Length of USART buffer
#define USART_BAUD_RATE96           9600    //!< Baud rate of USART
#define UART0                       0
#define UART1                       1

typedef struct
{
    int counter;                            //!< This structure member is used as index of transmit and receive buffers
    int size;                               //!< This structure member checks if transmit buffer is empty or not
    char bytesBuffer[USART_BUFFER_LENGTH];  //!< This structure member is used to store received data and data to be transmitted
} USART_STRUCT;

void InitUart(void);

void MSP430USARTEnableRx(unsigned char UartNum);
void MSP430USARTEnableTx(unsigned char UartNum);
void MSP430USARTDisableRx(unsigned char UartNum);
void MSP430USARTResetRxData(unsigned char UartNum);
void MSP430USARTResetTxData(unsigned char UartNum);
int MSP430UARTSendData(unsigned char UartNum, unsigned char *buf, unsigned char len);

//extern USART_STRUCT transmit0BufferUSART;
extern USART_STRUCT receive0BufferUSART;

//extern USART_STRUCT transmit1BufferUSART;
extern USART_STRUCT receive1BufferUSART;
#endif