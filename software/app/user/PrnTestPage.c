#include "includes.h"


#define SPACE_STRING			""

/*************************************************
功能: 测试字符
参数: 字符串
返回: 无
*************************************************/
void ICACHE_FLASH_ATTR
PrinterTestString(u8 *str)
{
	uint8_t ch;

	while ((ch=*str++) != 0)
	{
		put_prn(ch);
	}
	put_prn(0x0A);
}


// 打印测试页
#if 1
//函数      将LONG数据送入
//输入      无
//输出      无
//返回      无
void ICACHE_FLASH_ATTR
PutLongData(u8 *buf)
{
    u8 i,j;
	u32 LongData;
    u8 buffer[0x20];

	LongData = MachineNo;
    for(i=10; i<20; i++)  // 将LONG型数据转换成BCD码
    {
        j=LongData % 10;
        LongData /= 10;
        buffer[i] = (u8)(j+0x30);
    }

    for(i=19; i>9; i--)
    {
        buffer[19-i] = buffer[i];
    }

    for (j=0; j<10; j++)
    {
        if  (buffer[j] != 0x30) break;
    }

    if (j == 10) i = 1 ;
    else i = 10-j;

	os_memcpy(buf,&buffer[j],i);
	buf[i] = 0;
}

void ICACHE_FLASH_ATTR
PrintMachineNo(u8 *buf)
{

	os_sprintf((char *)buf,"Serial Number:    %s%s", SPACE_STRING, MachineIdStr);

	PrinterTestString(buf);
}

void ICACHE_FLASH_ATTR
PrintInterfaceMessage(u8 *buf)
{
	os_sprintf((char *)buf,"Interface:        %swifi",SPACE_STRING);
	PrinterTestString(buf);

	os_sprintf((char *)buf,"Data bits:        %s8 bit",SPACE_STRING);
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Parity Check:     %sNone",SPACE_STRING);
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Stop Bit:         %s1 bit",SPACE_STRING);
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Handshaking:      %sDTR/DSR",SPACE_STRING);
	PrinterTestString(buf);

}

void ICACHE_FLASH_ATTR
PrintCharater(u8 *buf)
{
	u8 i,j;
	u8 count = 0;
	
	count = 32;
	count /= 2;
	if (PrintFontType)
	{
		u8 m;
		for (i=0; i<5; i++) // 打印中文测试字符
		{
			m = 0;
			for (j=0;j<count;j++)
			{
				buf[m++] = 0xb1;
				buf[m++] = 0xa1 + j + i;
			}
			buf[m++] = 0;
			PrinterTestString(buf);
		}
	}
}
/*******************************************************************************
* Function Name  : PrinterTestPage
* Description    : 打印测试页
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrinterTestPage(void)
{
	u8 buf[256];
	u32 i;
	PrinterTestString(" ");

	PrinterTestString("    ===== Selftest Page =====");

	PrinterTestString(" ");
	os_sprintf((char *)buf,"Version:          %s%s", SPACE_STRING, __SOFTVER);
	PrinterTestString(buf);

	PrintMachineNo(buf);

	os_sprintf((char *)buf,"Modify date:      %s%s", SPACE_STRING, MODIFY_DATE);
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Command mode:     %sEPSON(ESC/POS)", SPACE_STRING);
	PrinterTestString(buf);

	PrintInterfaceMessage(buf);

	os_sprintf((char *)buf, "Cutter:           %s%s",SPACE_STRING,INCLUDE_CUTPAPER?"YES":"NO");
	PrinterTestString(buf);

	i = os_sprintf((char *)buf,"Print Density:    %s", SPACE_STRING);
    switch (Density)
    {
        case 0:
            os_sprintf((char *)&buf[i],"Light");
            break;
        case 1:
			os_sprintf((char *)&buf[i],"Middle Light");
            break;
		case 2:
			os_sprintf((char *)&buf[i],"Middle Dark");
			break;
        case 3:
            os_sprintf((char *)&buf[i],"Dark");
            break;
		default:
			os_sprintf((char *)&buf[i],"Dark");
			break;
    }
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Print Width:      %s%dmm",SPACE_STRING,PrnDotOfByte);
	PrinterTestString(buf);
	os_sprintf((char *)buf,"Print Speed:      %s%dmm/sec(Max)",SPACE_STRING, PrnDrv.Prn_MaxSpeed > 100 ? 150:80);
	PrinterTestString(buf);

	PrinterTestString(" ");

	os_sprintf((char *)buf,"Resident Character:");
	PrinterTestString(buf);
	os_sprintf((char *)buf,"  Alphanumeric");
	PrinterTestString(buf);

#ifndef MUL_LANG
	if (PrintFontType == FONT_JCHINESE) // 简体字
    {
        // 采用GB2312字库
		{
			os_sprintf((char *)buf, "  Chinese GB2312 Simplified Chinese");
		}

		PrinterTestString(buf);
    }
	else
	{
		os_sprintf((char *)buf,"  Big 5 ");
		PrinterTestString(buf);
	}
#else
  #if TP34_HEAD_EN
	PrinterTestString("Code Page:");
	PrinterTestString("  0:PC437(U.S.A)        2:PC850(Multilingual)");
	PrinterTestString("  3:PC860(Portuguese)   4:PC863(Canada-French)");
	PrinterTestString("  5:PC865(Nordic)       7:(Spianish)");
	PrinterTestString("  8:PC857(Turkey)       10:(Farsi)");
	PrinterTestString("  14:PC864(Arabic)      18:PC852(Latin2)");
	PrinterTestString("  20:PC737(Greek)       25:WPC1254(Turkey)");
	PrinterTestString("  27:(Vietnam)          32:WPC1255(Israel)");
	PrinterTestString("  33:Rumania            40:WPC1256");
	PrinterTestString("  59:PC866(Cyrillic #2) 68:(Armenia)");
  #else
  	PrinterTestString(" ");
    PrinterTestString("Code Page:");
    PrinterTestString("  0:PC437(U.S.A))");
    PrinterTestString("  2:PC850(Multilingual)");
    PrinterTestString("  3:PC860(Portuguese)");
    PrinterTestString("  4:PC863(Canada-French)");
    PrinterTestString("  5:PC865(Nordic)");
    PrinterTestString("  7:(Spianish)");
    PrinterTestString("  8:PC857(Turkey)");
    PrinterTestString("  10:(Farsi)");
    PrinterTestString("  14:PC864(Arabic)");
    PrinterTestString("  18:PC852(Latin2)");
    PrinterTestString("  20:PC737(Greek)");
    PrinterTestString("  25:WPC1254(Turkey)");
    PrinterTestString("  27:(Vietnam)         ");
    PrinterTestString("  32:WPC1255(Israel)");
    PrinterTestString("  33:Rumania ");
    PrinterTestString("  40:WPC1256 ");
    PrinterTestString("  59:PC866(Cyrillic #2)");
	PrinterTestString("  68:(Armenia)");
  #endif
#endif

	PrinterTestString(" ");

	i=os_sprintf((char *)buf, "Barcode Type:");
	PrinterTestString(buf);

	i=os_sprintf((char *)buf, "       UPC-A          UPC-E");
	PrinterTestString(buf);

	i=os_sprintf((char *)buf, "       EAN(JAN)13     EAN(JAN)8");
	PrinterTestString(buf);

	i=os_sprintf((char *)buf, "       CODE39         CODABAR");
	PrinterTestString(buf);

	i=os_sprintf((char *)buf, "       ITF            CODE93");
	PrinterTestString(buf);

	i=os_sprintf((char *)buf, "       CODE128");
	PrinterTestString(buf);

	PrinterTestString(" ");
	//===============================
	PrintCharater(buf);

	PrinterTestString(" ");

	os_sprintf((char *)buf,"   ===== Completed =====");

	PrinterTestString(buf);


	PrinterTestString(" ");
	PrinterTestString(" ");
#if MACHINE_TYPE != MACHINE_TP27M3
	PrinterTestString(" ");
	PrinterTestString(" ");
#endif
}


// 打印麻点图
#endif

#if 1

/*************************************************
功能: 测试点阵图0
参数: 高度
返回: 无
*************************************************/
void ICACHE_FLASH_ATTR
PrinterTest0(uint16_t line)
{
	uint8_t buf[PrnDotOfByte];
	uint8_t buf1[PrnDotOfByte];
	uint16_t i,m;
	///////////////////////////////////////////////////
	os_memset(buf,0xCC,sizeof(buf));
	os_memset(buf1,0x33,sizeof(buf1));
	for(i=0;i<line/2;i++)
	{
		for(m=0;m<2;m++)
		{
			PrnPutOneLineToPrintBuf( (i&0x1)? buf : buf1 );
		}
	}
}

/*************************************************
功能: 测试点阵图1
参数: 高度
返回: 无
*************************************************/
void ICACHE_FLASH_ATTR
PrinterTest1(uint16_t line)
{
	uint8_t buf[PrnDotOfByte];
	uint8_t buf1[PrnDotOfByte];
	uint16_t i,m;
	///////////////////////////////////////////////////
	for(i=0;i<sizeof(buf);i++)
	{
		if( i%2 )
		{
			buf[i] = 0xFF;
			buf1[i] = 0x00;
		}
		else
		{
			buf[i] = 0x00;
			buf1[i] = 0xFF;
		}
	}
	/////////////////////////////////////////////////
	for(i=0;i<line/8;i++)
	{
		for(m=0;m<8;m++)
		{
			PrnPutOneLineToPrintBuf( (i&0x1)? buf : buf1 );
		}
	}
}

/*************************************************
功能: 测试直线(居中对齐)
参数: 长度
返回: 无
*************************************************/
void ICACHE_FLASH_ATTR
PrinterTestLine(uint16_t len)
{
	uint8_t m;
	uint8_t buf[PrnDotOfByte];
	uint32_t i;
	os_memset(buf,0x00,PrnDotOfByte);

	len /= 8;
	///////////////////////////////////////////////////
	i = (PrnDotOfByte-len)/2;
	os_memset(&buf[i],0xFF,len);
	/////////////////////////////////////////////////
	for(m=0;m<3;m++)	{ PrnPutOneLineToPrintBuf(buf); }
}

/*******************************************************************************
* Function Name  : PrinterTestPage
* Description    : 打印测试页
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrinterTestPage2(void)
{
	PrinterTestString(" ");
	PrinterTestString("  testing page");
	PrinterTestLine(200);
	PutBlackLine(24);
	PrinterTest0(250);
	PrinterTest1(250);
	PutBlackLine(120);
}

#endif

