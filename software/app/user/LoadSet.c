#include "includes.h"

#ifndef MUL_LANG
// 0=英文, 1=简体, 2=繁体, 3=日文
uint8_t PrintFontType = 0;	// 简体/繁体/日文
#else
uint8_t CodePage = 0;
#endif

u8 Density = 0;		// 打印浓度
u32 MachineNo;
u8 MachineIdStr[14];

#if INCLUDE_SETTING

//函数  载入设定参数到缓冲区到xsetting(必须在串口初始化之前调用。)
//输入  无
//输出  无
//返回  无
void ICACHE_FLASH_ATTR
LoadSetting(void)
{
	uint8_t xsetting[0x100];
	ReadNByteFromSst(xsetting,0x100,SETTING);

	os_memset(&strprnprop, 0, sizeof(STRPRNPROP));
    /* 判断参数是否合法，如果不合法，则清除参数设定 */
    if ((xsetting[254] != 0x55) || (xsetting[255] != 0x55))	/* 如果数据设置不合法 */
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
    if (!xsetting[ChinessTypeAddr]) 			PrintFontType = 1;	/* 置简体汉字标志 */
    else if (xsetting[ChinessTypeAddr] == 1) 	PrintFontType = 2;	/* 置繁体汉字标志 */
	else if (xsetting[ChinessTypeAddr] == 2) 	PrintFontType = 3;	/* 置日文字体标志 */
	else										PrintFontType = 1;	// 平时默认为简体
#endif

	MachineNo = BigEndingBuf_To_U32(&xsetting[ECRIDADDR]);
	os_memcpy(MachineIdStr,&xsetting[MachineIdAddr],13);
	MachineIdStr[13] = 0;

	os_memset(xsetting,0,256);
}
#endif


