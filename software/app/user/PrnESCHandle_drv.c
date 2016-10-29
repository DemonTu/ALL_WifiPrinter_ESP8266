#include "includes.h"
//==============================================================================
static uint8_t GPDrvFlag = 0;
static uint8_t GPAfterOut = 0;
static u8 DrawerNunber; // 钱箱号
static u8 DrawerPulseHighTime;   // 钱箱驱动高电平时间
static u8 DrawerPulseLowTime;    // 钱箱驱动低电平时间

const STR_FONT_SIZE FONT_MAP[FONT_MAX]=
{
    {12, 24},
    {8,  16},
    {5,  7 }, /* 这个字库比较特别，数据第一列和第二列都是空白，最后一列也是空白，使用时注意左移一位 */
    {8,  8 },
    {16, 32},
    {9,	 24},
};

STRPRNPROP strprnprop;

/*
步进点数的基本算法
i = (((Uint)buffer[1])<<8) + buffer[0];  // (n1+256*n2)
i = 203 * i + (i+2)/5;                    // 25.4mm*基本单位/x/0.125
if(strprnprop.pagedir %2){
    if(strprnprop.xunit) xdotcnt = i/strprnprop.xunit;
    else xdotcnt = i/200;
}else{
    if(strprnprop.yunit) ydotcnt = i/strprnprop.yunit;
    else ydotcnt = i/200;
}
*/
//==============================================================================
/*
*函数功能: 输入0x【buf1】【buf0】 和基本单位，算出对应的步进值
*输入参数: buf0， buf1， basic_uint (strprnprop.xunit or strprnprop.yunit)(0<=buf0<=255,0<=buf1<=255)
*输出参数: 无
*返回值  : 步进值
*/
//==============================================================================
uint32_t ICACHE_FLASH_ATTR
return_dot_step(uint8_t buf0, uint8_t buf1, uint8_t basic_unit)
{
	return (((uint16_t)buf1<<8) + buf0);
}

//==============================================================================
//函数  设定打印模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetPrintmode(u8 *bufferin)
{
#if 0
	strprnprop.prop = bufferin[0];
	if( strprnprop.prop & _PRNUNDERLINE )
	{
		/* 全局模式默认为2行 */
		strprnprop.underline_para = 2;
	}
#else
	uint8_t temp;
	temp = bufferin[0];

	if (temp & BIT(0))
	{
		strprnprop.font = FONT9X24;
	}
	else
	{
		strprnprop.font = FONT12X24;
	}

	if (temp & BIT(3))
	{
		strprnprop.prop |= _PRNEMPHASIZE;
	}
	else
	{
		strprnprop.prop &= ~_PRNEMPHASIZE;
	}

	if (temp & BIT(4))
	{
		strprnprop.prop |= _PRNENHEIGHT;
	}
	else
	{
		strprnprop.prop &= ~_PRNENHEIGHT;
	}

	if (temp & BIT(5))
	{
		strprnprop.prop |= _PRNENWIDTH;
	}
	else
	{
		strprnprop.prop &= ~_PRNENWIDTH;
	}

	if (temp & BIT(7))
	{
		strprnprop.prop |= _PRNUNDERLINE;
		strprnprop.underline_para = 1;	// 下划线粗细
	}
	else
	{
		strprnprop.prop &= ~_PRNUNDERLINE;
	}


#endif
	return (0);
}


//函数  设定下划线
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetUnderLine(u8 *buffer)
{
	if (buffer[0] >= '0')
	{
		buffer[0] -= '0';
	}

	if (buffer[0] == 0)
	{
		strprnprop.prop &= ~_PRNUNDERLINE;
	}
	else if (buffer[0] == 1)
	{
		strprnprop.prop |= _PRNUNDERLINE;
		strprnprop.underline_para = 1;
	}
	else if (buffer[0] == 2)
	{
		strprnprop.prop |= _PRNUNDERLINE;
		strprnprop.underline_para = 2;
	}
	return (0);
}

//函数  设定倍宽打印
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetDoubleWidth(u8 *buffer)
{
	strprnprop.prop |= _PRNENWIDTH;
	return 0;
}

//函数  设定倍宽打印
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscClearDoubleWidth(u8 *buffer)
{
	strprnprop.prop &= ~_PRNENWIDTH;
	return 0;
}


//函数  设定加重模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetEmphasize(u8 *buffer)
{
	if (buffer[0] & 0x0f)
	{
		strprnprop.prop |= _PRNEMPHASIZE;
	}
	else
	{
		strprnprop.prop &= ~_PRNEMPHASIZE;
	}
	return (0);
}


//函数  设定双倍黑度模式,效果与加重模式一样
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetDoubleStrike(u8 *buffer)
{
    return (EscSetEmphasize(buffer));
}

//函数  设定字符上下倒置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetUpDown(u8 * buffer)
{
    if (buffer[0] & 0x0f)
    {
        strprnprop.prop |= _PRNUPDOWN;
    }
    else
    {
        strprnprop.prop &= ~_PRNUPDOWN;
    }
    return (0);
}

//函数  字符顺时钟旋转90度
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PRG_OK */
u8 ICACHE_FLASH_ATTR
EscSetRotate90(u8 *buffer)
{
    if (buffer[0] >= '0')
    {
        buffer[0] -= '0';
    }
    if (0 == buffer[0])
    {
        strprnprop.prop &=~ _PRNRollRight90;
    }
    else if (1 == buffer[0])
    {
        strprnprop.prop |=  _PRNRollRight90;
    }
    return (0);
}

//函数  设定缺省的行高
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetDefLineSpace(u8 *buffer)
{
	/* 标准模式这里是转换成行间距 */
	strprnprop.linespace = (DEFAULT_LINE_SPACE-FONT_MAP[FONT12X24].h);
	return (0);
}


//函数  设定行间距
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetLineSpace(u8 *buffer)
{
	strprnprop.linespace = buffer[0];
#if 0
	/* 标准模式行高 */
	temp = return_dot_step(buffer[0], 0, strprnprop.yunit);
	if( temp>= FONT_MAP[strprnprop.font].h )
	{
		strprnprop.linespace = temp - FONT_MAP[strprnprop.font].h;
	}
	else
	{
		strprnprop.linespace = 0;
	}
#endif
	return (0);
}


//函数  设定字符间距
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetCharSpace(u8 *buffer)
{
	strprnprop.charspace = return_dot_step(buffer[0], 0, strprnprop.xunit);
	return (0);
}

//函数  设定使用的国际字符集
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSelectCharsSet(u8 *buffer)
{
    strprnprop.charsset = (ENUM_LANGUAGE)buffer[0];
    return (0);
}
// 'M'设置字体大小
#if (SET_FONT_SIZE_EN)
u8 ICACHE_FLASH_ATTR
EscSetFontSize(u8 *buffer)
{
	/* 一致化参数 */
	if (buffer[0] >= '0' )
	{
		buffer[0] -= '0';
	}
	/* 判断字体 */
	if ( buffer[0] < FONT_MAX )
	{
		strprnprop.font = (ENUM_FONT_SIZE)buffer[0];
	}
	return 0;
}

#endif
//函数  设定使用的字符表
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSelectCharsTbl(u8 *buffer)
{
	strprnprop.codetbl = buffer[0];
	return (0);
}


//函数  设定水平TAB位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PG_OK */
u8 ICACHE_FLASH_ATTR
EscSetTabsPos(u8 *buffer)
{
	uint8_t i;
	uint8_t temp;
	/* 清除以往跳格参数*/
	os_memset(strprnprop.htabpos, 0, sizeof(strprnprop.htabpos));
	temp = 0;
	for (i=0; i<TAB_NUM; i++)
	{
		/* 下一个预读的数是空则退出 */
		if (0 == WaitPrnBufBytes(1, buffer, 500,_WAIT_NBYTE_EXIT))
		{
			return i;
		}
		/* 下一个预读的数小于前一个数则停止 */
		if( buffer[0] <= temp )
		{
			return i;
		}
		temp = buffer[0];
#if 1		/* added by hzh */
		if( temp > PrnDotOfLine/FONT_MAP[strprnprop.font].w )
		{
			temp = PrnDotOfLine / FONT_MAP[strprnprop.font].w;
		}
#endif
		strprnprop.htabpos[i] = temp;
	}
	return OK;
}


//函数  打印缓冲区的数据 并走纸n字符行
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscPrnBuffer(u8 * buffer)
{
    PutLineDotBufToPrnDotBuf(); // 打印缓冲区数据
    PutBlankLineToPrintDotBuf(buffer[0]*(FONT_MAP[strprnprop.font].h)); // 走纸n点行
    return (0);
}

//函数  打印缓冲区的数据,并指定打印完进纸的长度
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscPrnBufferFeed(u8 *buffer)
{
	if((0x18==buffer[0]) && GPDrvFlag)
	{
	   //兼容佳伯驱动打印
	}
	else
	{
	    PutLineDotBufToPrnDotBuf();
	    PutBlankLineToPrintDotBuf(return_dot_step(buffer[0], 0, strprnprop.yunit)); // 走纸n点行
	}
    return (0);
}

//函数  设置钱箱开关(port=0/1; 电平 = 0 关）
//输出
//返回
static void ICACHE_FLASH_ATTR
SetDrawerLev (u8 port, u8 value)
{
#if (INCLUDE_DRAWER2 || INCLUDE_DRAWER1 )
    if (value)
    {
        if (port)	OpenDrawer2();
        else		OpenDrawer1();
    }
    else
    {
        if (port)	CloseDrawer2();
        else 		CloseDrawer1();
    }
#endif
}

//函数  控制外设的输出
//输入  buffer  命令缓冲区数据
//      注：命令格式为ESC p m n1,n2
//          其中m 为钱箱号（0，1）
//          其中n1 * 2ms 为钱箱开启时间
//          其中n2 * 2ms 为钱箱关闭时间
//          0 < n1 < n2 < 255
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscOutDevice(u8 *buffer)	// tqy
{
	if ((buffer[0] == 48) || (buffer[0] == 49))
	{
		buffer[0] -= 48;
	}
	if (buffer[0] < 2)
	{
		SetDrawerLev(buffer[0],0x01);	// 开钱钱箱
		MSctCnt = 0;		  // 开钱箱延时
		while(MSctCnt < buffer[1]*2)
		{
			//pollint();
		}
		SetDrawerLev(buffer[0],0x00);// 关钱钱箱

		if (buffer[1] > buffer[2])
		{
			buffer[2] = buffer[1];
		}
		MSctCnt = 0;		  // 关钱箱延时
		while(MSctCnt < buffer[2]*2)
		{
			pollint();
			//PrnEscHandle();
		}
	}
	return 0;
}


//函数  设定按键缺纸等信息
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetKeyAvailable(u8 *buffer)
{
    if (buffer[0] == '0')
    {
        if (buffer[1] > 2)
        {
            return (0);
        }
        strprnprop.paper_type = (ENUM_PAPER_TYPE)buffer[1];
    }
    else if (buffer[0] == '3')
    {
    }
    else if (buffer[0] == '4')
    {
    }
    else if (buffer[0] == '5')
    {
    }

    return (0);
}

//函数  设定打印的绝对位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetAbsPosition(u8 * buffer)
{
    uint32_t temp;
    temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
    if ((temp>strprnprop.current_row)&&(temp<=PrnDotOfLine))
    {
        strprnprop.current_row = temp;
    }
    return (0);
}

//函数  设定打印的相对位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PR_OK */
u8 ICACHE_FLASH_ATTR
EscSetRelPosition(u8 * buffer)
{
    uint32_t temp;
    temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit)+strprnprop.current_row;
    if ((temp>strprnprop.current_row)&&(temp<=PrnDotOfLine))
    {
        strprnprop.current_row = temp;
    }
    return (0);
}

//函数  输出外设状态
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscReadOutdeviceStatus(u8 * buffer)
{
    USART_To_USB_Send_Byte(0);	// tqy
    return (0);
}

//函数  输出纸张状态
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscReadPaperStatus(u8 * buffer)
{
	SendPrinterStatus();	//
    return (0);
}
//函数  裁纸命令
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscCutPaper(u8 * buffer)
{
    return (0);
}

//函数  初始化打印机
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 EscInitPrinter(u8 * buffer)
{
    uint8_t i;

	os_memset(&strprnprop, 0, sizeof(STRPRNPROP));
	strprnprop.density = Density;
    /* 定义字符集 */
    strprnprop.charsset = USA;

    /* 设置默认字体的宽度和高度*/
#ifdef MUL_LANG
	strprnprop.font = FONT9X24;
	//strprnprop.font = FONT12X24;
#else
	//strprnprop.font = FONT8X16;
	//strprnprop.font = FONT9X24;
	strprnprop.font = FONT12X24;
#endif
#if TP34_HEAD_EN
	strprnprop.font = FONT12X24;
#else
	strprnprop.font = FONT12X24;
#endif
    /* 打印条码的初始化 */
    strprnprop.barcodewidth = MAXMORENWEIGH;		/* 设置条码打印缺省宽度 (EPSON缺省值) */
    strprnprop.barcodeleft = 0x00;					/* 设置条码水平打印缺省位置 */
    strprnprop.barcodeheight = MAXMORENHIGH;		/* 设置条码高度 */
    /* 设置GS P默认情况的基本单位 */
    strprnprop.yunit = Y_DEFAULT_BASIC_MOVE_UNIT;	/* 默认一步0.125 */
    strprnprop.xunit = X_DEFAULT_BASIC_MOVE_UNIT;	/* 默认一步0.125 */
    /* 标准模式参数 */
    strprnprop.linespace = (DEFAULT_LINE_SPACE-24);
    strprnprop.charspace = DEFAULT_CHAR_SPACE;
    /* 设置TAB跳格的默认值 */
#define TABJUMP   8
    for (i=0; i<TAB_NUM; i++)
    {
        strprnprop.htabpos[i]=TABJUMP*(i+1);
    }
    /* bitmap */
    BitmapInit();
	TogglePrintOnLine(1);			/* 打印机缺省是连线状态 */
    /*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
#if (PAGE_MODE_EN)
    /* 页模式初始化 */
    PrnPageInit();
#endif
#ifdef MUL_LANG
	Internationalcode(CodePage);	/* 预设国家码 */
#endif

    /*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
    return (*buffer);
}
/*
**函数功能: 波特率设置
**输入参数:
**输出参数:
**返回值  :
*/
u8 ICACHE_FLASH_ATTR
FSChangeBaudTate(u8 *buffer)
{
    if (buffer[0] <= 4)
    {
        strprnprop.baudrate = buffer[0];
        SetBaudRate(strprnprop.baudrate);	// tqy
    }
    return (0);
}

/*
* 设置浓度
*/
u8 ICACHE_FLASH_ATTR
FSSetPrintDensity(u8 *buffer)
{
    if (buffer[0] <= 7)
    {
        strprnprop.density = buffer[0];
    }
    return (0);
}

//函数  选择外围输出设备
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetPrinterOnLine(u8 * buffer)
{
	TogglePrintOnLine(buffer[0]);
	return (0);
}


//函数  切换打印机进入页模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PG_OK */
u8 ICACHE_FLASH_ATTR
EscSelectPageMode(u8 * buffer)
{
#if PAGE_MODE_EN
//    EscInitPrinter(0);
    strprnprop.pagemode = PAGE_MODE;
    PrnPageInit();
#endif
    return (0);
}

// 设定页高为 buffer[0] 英寸
u8 ICACHE_FLASH_ATTR
GpOpenDrawer(u8 * buffer)
{
	if (GPDrvFlag)
	{
		// 驱动打印。
		_MOTOR_FEED_MM(15);
	}
    GPDrvFlag = 0;

    //打印完成后是否开钱箱
    if (GPAfterOut)
    {
        GPAfterOut = 0;

		buffer[1] = DrawerPulseHighTime;   // 钱箱驱动高电平时间
		buffer[2] = DrawerPulseLowTime;    // 钱箱驱动低电平时间
		if (DrawerNunber & 0x01)
		{
			buffer[0] = 0;          // 钱箱号
            EscOutDevice(buffer);
		}

		if (DrawerNunber & 0x02)
        {
			buffer[0] = 1;			// 钱箱号
			EscOutDevice(buffer);
		}
		DrawerNunber = 0;
    }
    return(0);
}
//函数  需识别的LQ1600k ,但不需执行的命令
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscLQ1600KNoUseCmd(u8 * buffer)
{
    return (*buffer);
}

// 设定对齐模式
// buffer[0] = 0 左对齐; = 1 居中; = 2 右对齐;
/* PR_OK */
u8 ICACHE_FLASH_ATTR
EscSetPageLineMode(u8 * buffer)
{
    if (buffer[0] < 3)
    {
        strprnprop.LineModeM = buffer[0];
    }
    else if ((buffer[0] >= 48) && (buffer[0] < 51))
    {
        strprnprop.LineModeM = buffer[0] - 48;
    }
    return(0);
}

//函数  设定汉字打印模式组合
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
FsSetPrintmode(u8 *bufferin)
{
    u8 * buffer;
    buffer=bufferin;
    strprnprop.prop = 0;

    if (buffer[0] & 0x80)
    {
        strprnprop.prop |= _PRNUNDERLINE;
		strprnprop.underline_para = 1;	// 下划线粗细
    }
    if (buffer[0] & 0x08)
    {
        strprnprop.prop |= _PRNENHEIGHT;
    }
    if ((buffer[0] & 0x04))
    {
        strprnprop.prop |= _PRNENWIDTH;
    }
    return (0);
}


//函数  设定汉字字间距
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
FsSetWordInt(u8 *buffer)
{
    return (0);
}

//函数  选择/取消汉字倍高倍宽
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
FsSelectHzBhw(u8 *buffer)
{
	strprnprop.prop &= ~(_PRNENHEIGHT + _PRNENWIDTH);

	if (buffer[0] & 0x01)
    {
        strprnprop.prop |= _PRNENHEIGHT;
        strprnprop.prop |= _PRNENWIDTH;
    }

    return (0);
}


//函数  选择/取消汉字下划线
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
FsSelectHzULine(u8 *buffer)
{
	if ((buffer[0] < 3) || ((buffer[0] > 0x2f) && (buffer[0] < 0x33)))
    {
		strprnprop.underline_para = 0;	// 下划线粗细

        buffer[0] &= 0x0f;

        if (buffer[0] == 0)
        {
            strprnprop.prop &= ~_PRNUNDERLINE;
        }
        else
        {
            strprnprop.prop |= _PRNUNDERLINE;
            //buffer[0] --;
			strprnprop.underline_para = buffer[0];	// 下划线粗细
        }
     }
    return (0);
}



//函数  前进纸至黑标位置 （如果已在黑标位置则不动作）
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GscBlankMark(u8 *buffer)
{
#if (PAPER_MARK_EN)
    if (PAGE_MODE == strprnprop.pagemode)
    {
        ClrNoPaperFlag();
        ResetLabel();
    }
    else if (STANDERD_MODE == strprnprop.pagemode)
    {
        AppendPrnBuffer(HEAT_PARA_PaperHead, 0);
    }
#endif
    return (*buffer);
}


// 命令格式 GS ( F PL PH a m nL nH
// (pL + pH*256) * 0.125 mm = 设置撕纸位置相对于黑标检测的偏移量
// a = 1 设置起始打印位置相对与黑标检测位置的偏移两

//函数  设置黑标定位偏移量
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
PrintTestPage(u8 *buffer)
{
#if (PRN_TEST_EN)
    print_test_page();	// tqy
#endif
    return (0);
}

//函数  设定打印机字体的放大倍数
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetFontEnlarge(u8 *buffer)
{
	if( buffer[0] & 0x0f )
	{
		strprnprop.prop |= _PRNENHEIGHT;
	}
	else
	{
		strprnprop.prop &= ~_PRNENHEIGHT;
	}

	if ((buffer[0]>>4))
	{
		strprnprop.prop |= _PRNENWIDTH;
	}
	else
	{
		strprnprop.prop &= ~_PRNENWIDTH;
	}
	return (0);
}

//函数  开始/结束宏定义
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsStartEndMacro(u8 *buffer)
{
    return (0);
}

//函数  设定/取消反白
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetBlackWhite(u8 *buffer)
{
    if (buffer[0] & 0x01)
    {
        strprnprop.prop |= _PRNREVERSE;
    }
    else
    {
        strprnprop.prop &= ~_PRNREVERSE;
    }
    return (0);
}

//函数  发送打印机ID
//输入  buffer  命令缓冲区数据 1d 49 n
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsReadPrintID(u8 *buffer)
{
	u8 ch;
	ch = buffer[0];
	switch( ch )
	{
		case 0: 	// 版本号
			os_printf("%s\r\n", _MACHINE_TYPE_STRING);
			break;
		case 1: 	// 系统电压
			os_printf("volt=%dmv\r\n", 5000);
			break;
		case 2: 	// 打印头类型
			os_printf("PrnHead=%d\r\n", 0);
			break;
		case 3:
			os_printf("flash_id=%04x\r\n", spi_flash_get_id());
			break;
		case 4:
			#ifndef MUL_LANG
			os_printf("font=%d\r\n", PrintFontType);
			#endif
			break;
		default:
			break;
	}
	return (0);
}


//函数  设定左边距
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetLeftMargin(u8 *buffer)
{
	uint32_t temp;
	temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
	if (temp <= PrnDotOfLine)
	{
		strprnprop.left_space = temp;
	}
	if (strprnprop.left_space+strprnprop.right_space >= PrnDotOfLine)
	{
		strprnprop.right_space = PrnDotOfLine - strprnprop.left_space;
	}
	return (0);
}


//函数  设定水平垂直单位
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetMotionUnit(u8 *buffer)
{
    strprnprop.xunit = buffer[0];
    strprnprop.yunit = buffer[1];
    if (strprnprop.xunit == 0)
    {
        strprnprop.xunit = X_DEFAULT_BASIC_MOVE_UNIT;
    }
    if (strprnprop.yunit == 0)
    {
        strprnprop.yunit = Y_DEFAULT_BASIC_MOVE_UNIT;
    }
    return (0);
}

//函数  切纸(有三种切纸方式：全切，半切，走纸后半切)
//命令格式: GS V m n
//注: m = 0,48 为半切纸
//注: m = 1,49 为全切纸
//注: m = 65 走纸 n 点行后,全切纸
//注: m = 66 走纸 n 点行后,半切纸
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsCutPaper(u8 *buffer)
{
    return (0);
}

//函数  设定条码打印宽度
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetPageWidth(u8 *buffer)
{
    uint32_t temp;
    temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
    if (temp + strprnprop.left_space <= PrnDotOfLine)
    {
        strprnprop.right_space = PrnDotOfLine - temp - strprnprop.left_space;
    }
    return (0);
}
//函数  执行宏
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsExeMacro(u8 *buffer)
{
    return (0);
}

//函数  允许/关闭自动状态反馈
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetAutoAckStatus(u8 *buffer)
{
    strprnprop.autostatus = buffer[0];
    return (0);
}

//函数  设置连续模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetSmoothMode(u8 *buffer)
{
    return (0);
}
//函数  发送状态
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsReadStatus(u8 *buffer)
{
    return(0);
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////below define barcode ///////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/*
u8 GsSetHRIPosition(u8 *buffer)
{  return(*buffer++); }
u8 GsSetHRIFont(u8 *buffer)
{  return(*buffer++); }
extern u8 GsSetBarcodeHeight(u8 *buffer)
{  return(*buffer++); }
u8 GsSetBarcodeWidth(u8 *buffer)
{  return(*buffer++); }
static void PrintHriFont(u8 *buffer,u8 barcodetype)
{  *buffer++=barcodetype; }
static void PrintBarcode(u8 *buffer,u8 barcodetype)
{  *buffer++=barcodetype; }
u8 GsPrintBarcode(u8 *buffer)
{  return(*buffer++); }
*/








//佳柏扩展命令
u8 ICACHE_FLASH_ATTR
GPExterntCmd(u8 *buffer)
{
	return 0;
}

//佳柏扩展命令
u8 ICACHE_FLASH_ATTR
GPExterntCut(u8 *buffer)
{
    return 0;
}

u8 ICACHE_FLASH_ATTR
GPOutDevice(u8 *buffer)
{
	if (buffer[0] == 1) DrawerNunber += 0x2;
	else DrawerNunber += 0x1;		  // 钱箱号
	DrawerPulseHighTime = buffer[1] ; // 钱箱驱动高电平时间
	DrawerPulseLowTime	= buffer[2] ; // 钱箱驱动低电平时间
	GPAfterOut = 1;
	return 0;
}

u8 ICACHE_FLASH_ATTR
GPExterntFilter(u8 *buffer)
{
    GPDrvFlag = 1;
    return 0;
}


//==============================================================================
/*
*函数功能: 将光标定位到下一个TAB位置,特别注意的是如果当前位置+一个字符宽度大于下一个
*          制表位，则下移动到下下个制表位
*输入参数: 无
*输出参数: 无
*返回值  : 无
*/
//==============================================================================
void ICACHE_FLASH_ATTR
CursorToNextTabPosition(void)
{
	u8 tabvalue;
	u8 largecnt;

	largecnt = 0;

	if (strprnprop.enwidth > 0)
	{
		largecnt = strprnprop.enwidth;
	}
	else if (strprnprop.prop & _PRNENWIDTH)
	{
		largecnt = 1;

	}

	largecnt ++;

	tabvalue = (FONT_MAP[strprnprop.font].w+strprnprop.charspace) * largecnt;

	if ((strprnprop.htabcnt < 32 ) && (strprnprop.htabpos[strprnprop.htabcnt] != 0))
	{
		if ((strprnprop.htabpos[strprnprop.htabcnt] * tabvalue) > strprnprop.current_row)
		{
			strprnprop.current_row = (strprnprop.htabpos[strprnprop.htabcnt] * tabvalue );
			strprnprop.current_row %= PrnDotOfLine;
			if (strprnprop.current_row == 00) strprnprop.current_row = PrnDotOfLine;
			strprnprop.htabcnt ++ ;
		}
	}
}


/*******************************************************************************
*
*                      条码打印使用函数
*
*******************************************************************************/
#if (BARCODE_EN)
//函数  设定fri的字体
//输入  buffer  命令缓冲区数据
//      0   12x24
//      1    9x24
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetHRIFont(u8 *buffer)
{
	return (0);
}

//函数  选择HRI字符的打印位置
//      0   不打印
//      1   在条码上面
//      2   在条码下面
//      3   条码上下两个位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetHRIPosition(u8 *buffer)
{
	if(buffer[0] <= 3)
	{
		strprnprop.barcodeHRIpos = buffer[0];
	}
	else if((buffer[0] >= 48)&&(buffer[0] <= 51))
	{
		strprnprop.barcodeHRIpos = buffer[0] - 48;
	}
	else
	{
		strprnprop.barcodeHRIpos = 0;				 // 设置成默认
	}
	return (0);
}

//函数  设置条码线条的宽度
//      设定    基本单位    窄线条  宽线条
//      2       2           2       5
//      3       3           3       8
//      4       4           4       10
//      5       5           5       13
//      6       6           6       16
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetBarcodeWidth(u8 *buffer)
{
	if((buffer[0] < 2)||(buffer[0] > 6))	return 0;
	strprnprop.barcodewidth = buffer[0];
	return (0);
}

//函数  设置条码的高度
//      缺省条码高度是162
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsSetBarcodeHeight(u8 *buffer)
{
    strprnprop.barcodeheight = buffer[0];
    return (0);
}

//函数  设置条码的左边距
//      缺省条码左边距是0
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsPrinterBarcodeLeft(u8 *buffer)
{
	uint8_t temp;
	temp = buffer[0];
	if( temp> PrnDotOfByte )
	{
		temp = PrnDotOfByte;
	}
	strprnprop.barcodeleft = temp;
	return 0;
}

//函数  打印条码
//      条码类型    条码长度    条码值范围
//      0 UPC-A     11-12       48-57
//      1 UPC-E     11-12       48-57
//      2 EAN13     12-13       48-57
//      3 EAN8       7- 8       48-57
//      4 CODE39     1-         48-57,65-90,32,36,37,43,45,46,47
//      5 ITF        1-偶数     48-57(程序用交错式25码打印)
//      6 CODABAR    1-         48-57,65-68,36,43,45,46,47,58
//      7 CODE93     1-255      0-127
//      8 CODE128    2-255      0-127
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
GsPrintBarcode(u8 *buffer)
{
    u8 barlen;               //bartype记录要打印得条码类型，barlen存储要打印条码的长度
    u8 buf[256];

    /* 判断条码高度是否合法，不合法则载入缺省的高度 */
    if (strprnprop.barcodeheight == 0)
    {
        strprnprop.barcodeheight = MAXMORENHIGH;
    }
    /* 判断条码宽度是否合法，不合法则载入缺省的宽度 */
    if (strprnprop.barcodewidth == 0)
    {
        strprnprop.barcodewidth = MAXMORENWEIGH;
    }
    /* 获取barcode打印类型 */
    strprnprop.bartype = buffer[0];

	buffer = &buf[0];
    /* 对应命令<GS> 'k' m d1 d2 d3 d4 ... dk <NUL> */
    if (strprnprop.bartype <= 0x08)
    {
        barlen = WaitPrnBufBytes(255,buffer,1000,_WAIT_ZERO_EXIT);
        barlen--;
    }
    /* 对应命令<GS> 'k' m n d1 d2 d3 d4 ... dn */
    else if ((strprnprop.bartype >= 65) && (strprnprop.bartype <= 73))
    {
        strprnprop.bartype -= 65;
        if (WaitPrnBufBytes(1,buffer,500,_WAIT_NBYTE_EXIT) == 0)
        {
            return (0);
        }
        /* 获取长度 */
        barlen = buffer[0];
        /* 判断长度 */
        if (barlen == 0)
        {
            return (0);
        }
        /* 接收数据 */
        if (WaitPrnBufBytes(barlen,buffer,500,_WAIT_NBYTE_EXIT) != barlen)
        {
            return (0);
        }
    }
    /* 不支持的条码类型 */
    else
    {
        return (0);
    }
    if (barlen == 0)
    {
        return(0);
    }
    /* 条码长度限制 */
    if (barlen > MAXBARCODELEN - 4)
    {
        return (0);
    }
    /* 最后补0 */
    buffer[barlen] = '\0';
    PrnOneBarcodeSet(buffer);
    return (0);
}
#endif
/*******************************************************************************
*
*                             页模式所用函数
*
*******************************************************************************/
#if (PAGE_MODE_EN)
//函数  换页 打印页模式的数据
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 EscPrnPageData(u8 *buffer)
{
    StartPrnPage();
    return (0);
}
//函数  切换打印机进入标准模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 EscSelectStandardMode(u8 * buffer)
{
    strprnprop.pagemode = STANDERD_MODE;
    return (0);
}
//函数  设定打印方向
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PR_OK */
u8 EscSetPrnDirection(u8 * buffer)
{
    uint8_t dir;
    switch(buffer[0])
    {
        case 0:
        case 1:
        case 2:
        case 3:
            dir = buffer[0];
            break;
        case 48:
        case 49:
        case 50:
        case 51:
            dir = (buffer[0] - 48);
            break;
        default:
            dir = 0;           //默认A
            break;
    }
    PageSetDir((ENUM_PAGE_PRN_DIR)dir);
    return (0);
}
//函数  设定
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
/* PR_OK */
u8 EscSetPageSetting(u8 *buffer)
{
    uint32_t page_startx,page_starty,page_width,page_high;
    page_startx = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
    page_starty = return_dot_step(buffer[2], buffer[3], strprnprop.yunit);
    page_width  = return_dot_step(buffer[4], buffer[5], strprnprop.xunit);
    page_high   = return_dot_step(buffer[6], buffer[7], strprnprop.yunit);
    PageSetPrnArea(page_startx, page_starty, page_width, page_high);
    return (0);
}
//函数  页模式选择垂直位置的绝对位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 GsSetAbsVertPosition(u8 *buffer)
{
    uint32_t temp;
    if (0 == (PageGetDir()%2))    // 横向
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.yunit);
        if (PAGE_OK == PageSideCheck(Y_CURSOR, temp))
        {
            PageSetCursor(Y_CURSOR, temp);
        }
    }
    else                              // 纵向
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
        if (PAGE_OK == PageSideCheck(X_CURSOR, temp))
        {
            PageSetCursor(X_CURSOR, temp);
        }
    }
    return (0);
}
//函数  页模式中设定垂直的相对位置
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 GsSetRelVertPosition(u8 *buffer)
{
    uint32_t temp = 0;
    if (0 == (PageGetDir()%2))    // 横向
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.yunit)+PageGetCursor(Y_CURSOR);
        if (PAGE_OK == PageSideCheck(Y_CURSOR, temp))
        {
            PageSetCursor(Y_CURSOR, temp);
        }
    }
    else                              // 纵向
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit)+PageGetCursor(X_CURSOR);
        if (PAGE_OK == PageSideCheck(X_CURSOR, temp))
        {
            PageSetCursor(X_CURSOR, temp);
        }
    }
    return (0);
}
//==============================================================================
/*
**函数功能:
**输入参数:
**输出参数:
**返回值  :
*/
u8 EscSetLine(uint8_t *buffer)
{
    uint32_t x0,x1,y0,y1,w;
    w  = buffer[0];
    x0 = buffer[1]+buffer[2]*0xFF;
    y0 = buffer[3]+buffer[4]*0xFF;
    x1 = buffer[5]+buffer[6]*0xFF;
    y1 = buffer[7]+buffer[8]*0xFF;
    PageLineWith(x0, y0, x1, y1, w);
    return 0;
}
#endif

/*
**函数功能: 设置打印参数
**输入参数:
**输出参数:
**返回值  :
*/
u8 ICACHE_FLASH_ATTR
EscSettingPrinter(uint8_t *buffer)
{
#if INCLUDE_FENDUAN
	uint16_t temp;
	temp = buffer[0] + 1;
	if( temp )
	{
		temp = temp * 8;
		// 最大只支持分DUP_HEAT_CNT段
		if( temp < (TOTALBIT/DUP_HEAT_CNT) )	temp = (TOTALBIT/DUP_HEAT_CNT);
		else if (temp > TOTALBIT) temp = TOTALBIT;
		MaxDotOfArea = temp;
	}
#endif
	return 0;
}

/*
**函数功能: 打印测试页
**输入参数: 无
**输出参数:
**返回值  :
*/
u8 ICACHE_FLASH_ATTR
Dc2PrintTestPage(uint8_t *buffer)
{
	PrinterTestPage();	// tqy
	return 0;
}
/*
**函数功能: 设置打印浓度
**输入参数: 无
**输出参数:
**返回值  :
*/
u8 ICACHE_FLASH_ATTR
Dc2SetPrintDark(uint8_t *buffer)
{
	strprnprop.density = buffer[0];
	if( strprnprop.density > 4 )	strprnprop.density = 4;
	return (0);
}

/*
**函数功能: 设置系统睡眠参数
**输入参数: 无
**输出参数:
**返回值  :
*/
u8 ICACHE_FLASH_ATTR
EscSetSystemSleep(uint8_t *buffer)
{
	uint32_t sleepDlyTm;

	sleepDlyTm = buffer[0]+(uint32_t)buffer[1]*256;
	if( sleepDlyTm )
	{
		if( sleepDlyTm<20 )	sleepDlyTm = 20;		/**单位10ms**/
	}
	return (0);
}


