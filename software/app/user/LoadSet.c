#include "includes.h"

#ifndef MUL_LANG
// 0=Ӣ��, 1=����, 2=����, 3=����
uint8_t PrintFontType = 0;	// ����/����/����
#else
uint8_t CodePage = 0;
#endif

u8 Density = 0;		// ��ӡŨ��
u32 MachineNo;
u8 MachineIdStr[14];

#if INCLUDE_SETTING

//����  �����趨��������������xsetting(�����ڴ��ڳ�ʼ��֮ǰ���á�)
//����  ��
//���  ��
//����  ��
void ICACHE_FLASH_ATTR
LoadSetting(void)
{
	uint8_t xsetting[0x100];
	ReadNByteFromSst(xsetting,0x100,SETTING);

	os_memset(&strprnprop, 0, sizeof(STRPRNPROP));
    /* �жϲ����Ƿ�Ϸ���������Ϸ�������������趨 */
    if ((xsetting[254] != 0x55) || (xsetting[255] != 0x55))	/* ����������ò��Ϸ� */
    {
        os_memset(xsetting,0,256);
        xsetting[SDENSITY] = 0x04;
        os_memset(&xsetting[ECRIDADDR], 0x3A, 4);
		os_memset(&xsetting[MachineIdAddr], 0x3A, 13);

    }

	Density = xsetting[SDENSITY];
	if( Density > 4 ) Density = 4;

#ifdef MUL_LANG
	CodePage = xsetting[CodePageAddr];
#else
	PrintFontType = 0;
    if (!xsetting[ChinessTypeAddr]) 			PrintFontType = 1;	/* �ü��庺�ֱ�־ */
    else if (xsetting[ChinessTypeAddr] == 1) 	PrintFontType = 2;	/* �÷��庺�ֱ�־ */
	else if (xsetting[ChinessTypeAddr] == 2) 	PrintFontType = 3;	/* �����������־ */
	else										PrintFontType = 1;	// ƽʱĬ��Ϊ����
#endif

	MachineNo = BigEndingBuf_To_U32(&xsetting[ECRIDADDR]);
	os_memcpy(MachineIdStr,&xsetting[MachineIdAddr],13);
	MachineIdStr[13] = 0;

	os_memset(xsetting,0,256);
}
#endif


