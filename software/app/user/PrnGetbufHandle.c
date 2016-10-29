#include "includes.h"

uint32_t ICACHE_FLASH_ATTR
GetRecvDatSize(void)
{
	return 0;//USART1_GetRxBufSize();
}

uint8_t ICACHE_FLASH_ATTR
GetRecvByte(void)
{
	return 0;//GetChar();
}

uint32_t ICACHE_FLASH_ATTR
WaitPrnBufBytes(uint32_t len, uint8_t *buffer, uint32_t timeout,u8 bZeroExit)
{
    uint32_t dat_cnt;
	uint8_t temp;

	dat_cnt = 0;
    TimerDelayReset();	// tqy
    while(1)
    {
        //pollint();
		if (GetRecvDatSize())
		{
			temp = GetRecvByte();
			buffer[dat_cnt++] = temp;

			if( bZeroExit && (temp==0x00) )
            {
				return dat_cnt;
			}
		}
		if ( dat_cnt == len)				
		{
			return dat_cnt;
		}
        else if (TimerDelay(timeout))	//tqy
        {
		    return dat_cnt;
        }
    }
}

