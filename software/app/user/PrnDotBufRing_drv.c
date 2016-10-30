#include "includes.h"

ALSTR_PRN_INITARG PrnDrv;			/* ��ӡͷ��ѹ */


//����һ��������
void ICACHE_FLASH_ATTR
CopyOneDotLineToPrn(u8 *buffer)
{
	uint8_t next_dat_p;

	if(					
		(!PaperStatus) &&
		(!_PRINTER_OVERHEAT())
	)
	{
		next_dat_p = RBUF_NEXT_PT(LDotWritePtr,1,DotBufHigh);
		system_soft_wdt_feed();
		while( next_dat_p==LDotReadPtr )
		{
			/* �ȴ���ӡ�������пռ� */
			//pollint();
		}
#if INCLUDE_FENDUAN
		FillDotBufToPrnDotBuf(buffer);
#else
		os_memcpy(&DotBuffer[LDotWritePtr][0], buffer, PrnDotOfByte);
#endif
	    LDotWritePtr = next_dat_p;
	}

}

