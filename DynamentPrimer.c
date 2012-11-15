#include "DynamentPrimer.h"

unsigned char current_cmd;
unsigned char current_id;
float current_reading;


unsigned char PremierReadValueCmd(unsigned char *FrameStr, unsigned char id)
{
    unsigned short sum = 0;
    FrameStr[0] = CD_DLE;
    current_cmd = FrameStr[1] = CD_RD;
    current_id = FrameStr[2] = id;
    FrameStr[3] = CD_DLE;
    FrameStr[4] = CD_EOF;
    
    sum = FrameStr[0] + FrameStr[1] + FrameStr[2] + FrameStr[3] + FrameStr[4];
    FrameStr[5] = (sum >> 8) & 0xFF;
    FrameStr[6] = sum&0xFF;

    return 7;
}

void GetCurrentReadingValue(unsigned char *ieee754)
{
    current_reading = *((float *)ieee754);
}

void PaserPremierResponse(unsigned char *ResponseStr, unsigned short len)
{
    unsigned char tmp1, tmp2;
    
    if(ResponseStr[0] != CD_DLE)
    {
      MSP430USARTResetTxData(UART0);
      return;
    }
    
    switch(current_cmd)
    {
    case CD_RD:
      if(ResponseStr[1] == CD_DAT)
      {
        switch(current_id)
        {
        case CONFIG_DATA_ID:
          break;
        case LIVE_DATA_ID:
          break;
        case ZERO_SENSOR_ID:
          break;
        case SPAN_SENSOR_ID:
          break;
        case LIVE_DATA_SIMPLE_ID:
          MSP430UARTSendData(UART0, "0", 1);
          if(ResponseStr[2] == 0x08 && ResponseStr[5] == 0x00 && ResponseStr[6] == 0x00)
          {
            MSP430UARTSendData(UART0, "1", 1);
            GetCurrentReadingValue(ResponseStr+7);
            if(current_reading > 0.2)
              ;
          }
          break;
        case USER_DATA_ID:
          break;
        default:
          break;
        }
      }
      else
      {
        // Should be a NAK, get the reason
      }
      break;
    case CD_WR:
      
      break;
    }
    
    MSP430USARTResetTxData(UART0);
    return;
}