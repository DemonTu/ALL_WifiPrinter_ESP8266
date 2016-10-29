#include "includes.h"

void ICACHE_FLASH_ATTR
PrnPutOneLineToPrintBuf(uint8_t *buf)
{
#if HEAT_2TIMES
	uint8_t tmp_buf[2][PrnDotOfByte];
	uint8_t temp;
	uint8_t i = 0;
	uint8_t m = 0;
	uint8_t c = 0;
	memset(tmp_buf, 0x00, sizeof(tmp_buf));
	for (i=0; i<PrnDotOfByte; i++)
	{
		temp = buf[i];
		if (temp)
		{
			for (m=0; m<8; m++)
			{
				if (temp & bitmap_H[m])
				{
					tmp_buf[c&1][i] |= bitmap_H[m];
					c ++;
				}
			}
		}
	}
	#if MOTOR_STEP_COUNT == 4
	CopyOneDotLineToPrn(tmp_buf[0]);
	CopyOneDotLineToPrn(tmp_buf[0]);
	CopyOneDotLineToPrn(tmp_buf[1]);
	CopyOneDotLineToPrn(tmp_buf[1]);
	#else
	CopyOneDotLineToPrn(tmp_buf[0]);
	CopyOneDotLineToPrn(tmp_buf[1]);
	#endif
#else
	CopyOneDotLineToPrn(buf);
	CopyOneDotLineToPrn(buf);
	#if MOTOR_STEP_COUNT == 4
	CopyOneDotLineToPrn(buf);
	CopyOneDotLineToPrn(buf);
	#endif
#endif
}

