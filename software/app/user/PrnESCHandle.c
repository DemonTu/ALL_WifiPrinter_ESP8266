#include "includes.h"

u8 HzAreaF = 0;

#ifdef MUL_LANG
static const uint8_t VietnamTBL[]=
{
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0XC5, 0xc2, 0XCA, 0XD4, 0XB4, 0XBF, 0XD0, 0XE5, 0XE2, 0XEA, 0XF4, 0XBD, 0XDF, 0XF0, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0xE4, 0xE3, 0xE1, 0xD5, 0x20, 0xA2, 0xC6, 0xC7, 0xA1, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA3, 0xA5, 0xA6, 0xE7, 0xA4, 0xA7, 0XE8, 0X20, 0XEB, 0XA8,
    0Xe9, 0Xa9, 0Xab, 0XAC, 0XAD, 0XAA, 0XAe, 0XEC, 0XEF, 0x20, 0x20, 0x20, 0XEE, 0XED, 0XB8, 0XF2,
    0x20, 0XF6, 0Xf5, 0XF3, 0XF7, 0XB0, 0XB1, 0XB2, 0XAF, 0XB5, 0XB6, 0XB7, 0XDE, 0XBE, 0XFE, 0XF9,
    0x20, 0XFC, 0XFB, 0XFA, 0XF8, 0XD7, 0XD8, 0Xe6, 0Xd1, 0XF1, 0XCF, 0XD6, 0XDB, 0XFD, 0XDC, 0x20
};
#define VietnamChange(c)	(VietnamTBL[c-0x80])
#endif

////////////////////////////////////////////////////////////////////////////////
void ICACHE_FLASH_ATTR
PrnEscInit(void)
{
    EscInitPrinter(0);
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              EscCmd table                                   */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD EscCmdTbl[] =
{
    {VK_FF,0,EscPrnPageData},         // 页模式下打印
    {'!',1,EscSetPrintmode},          // ! 21 选择打印模式
    {'-',1,EscSetUnderLine},          // - 2D 选择/取消下滑线模式
    {'E',1,EscSetEmphasize},          // E 45 选择/取消加粗模式
    {'G',1,EscSetDoubleStrike},       // G 47 选择/取消双重打印模式
    {'{',1,EscSetUpDown},             // { 7B 选择/取消倒置打印模式
    {'V',1,EscSetRotate90},           // V 56 选择/取消顺时针旋转90度
    {'3',1,EscSetLineSpace},          // 3 33 设置行间距
    {' ',1,EscSetCharSpace},          //   20 设置字符右间距
    {'%',1,EscSetCancelUserCharSet},  // % 25 选择/取消用户自定义字符
    {'&',0,EscSetUserCharPattern},    // & 26 下载用户自定义字符字库
    {'?',1,EscCancelUserChar},        // ? 3F 取消下载用户自定义字符字库
    {'R',1,EscSelectCharsSet},        // R 52 选择国际字符集
    {'t',1,EscSelectCharsTbl},        // t 74 选择字符代码表(codepage)
    {'D',0,EscSetTabsPos},            // D 44 设置横向跳格位置
    {'J',1,EscPrnBufferFeed},         // J 4A 走纸n点行
    {'d',1,EscPrnBuffer},             // d 64 打印 n 字符行
    {'p',3,EscOutDevice},             // p 70 产生钱箱控制脉冲
    {'q',3,GPOutDevice},              // q 71 产生钱箱控制脉冲(非标准EPSON命令,此为佳伯打印机专用)
    {'c',3,EscSetKeyAvailable},       // c 63 选择打印纸传感器以输出缺纸信号
    {'$',2,EscSetAbsPosition},        // $ 24 设定水平起始打印绝对位置((n1+n2*256)点)
    {'\\',2,EscSetRelPosition},       // \ 5C 设置相对横向打印位置
    {'W',8,EscSetPageSetting},        // W 57 设置页模式下打印区域
    {'T',1,EscSetPrnDirection},       // T 54 设置页模式下打印区域方向
    {'u',1,EscReadOutdeviceStatus},   // u 75
    {'v',0,EscReadPaperStatus},       // v 76
    {'i',0,EscCutPaper},              // i 69 半切纸
    {'@',0,EscInitPrinter},           // @ 40 初始化打印机
    {'=',1,EscSetPrinterOnLine},       // = 3D 选择输出设备( = 1 打印机 = 2 显示器)
    {'L',0,EscSelectPageMode},        // L 4C 选择页模式
    {'S',0,EscSelectStandardMode},    // S 53 选择标准模式
    {'2',0,EscSetDefLineSpace},       // 2 32 设置默认行间距
    {'*',3,EscSetBitimageMode},       // * 2A 选择打印位图模式
    {'r',0,EscLQ1600KNoUseCmd},       // r 72 选择打印颜色
    {'U',1,EscLQ1600KNoUseCmd},       // U 55 ;;n=0,1 选择打印方向,0:双向 1: 单向
    {'x',1,EscLQ1600KNoUseCmd},       // x 78 ;;选择打印品质
    {0x19, 1,EscLQ1600KNoUseCmd},     // 19  ;;设定自动装纸装置
    {'6',0,EscLQ1600KNoUseCmd},       // 6    ;;取消128~159的控制码功能
    {'C',3,GpOpenDrawer},         // 为与佳伯打印机兼容而加
    {'a',1,EscSetPageLineMode},       // 对齐模式 (左/右/居中)
    {'M',1,EscSetFontSize},         	// 设置字体大小
    {'l',9,EscSetLine},               // 1b 6c 画直线
    {'7',3,EscSettingPrinter},		  /* 设置打印参数 */
    {VK_SO,0,EscSetDoubleWidth},
	{VK_DC4,0,EscClearDoubleWidth},
	{'8',2,EscSetSystemSleep},		  /*设置系统睡眠*/
    {0,0,NULL}
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              FsCmd table                                    */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD FsCmdTbl[] =
{
    {'a',1,FSChangeBaudTate},	// 改变波特率
    {'b',1,FSSetPrintDensity},	// 改变打印浓度
    {'!',1,FsSetPrintmode},		//设定汉字打印模式组合
    {'S',2,FsSetWordInt},		//设定汉字字间距
    {'W',1,FsSelectHzBhw},		//选择/取消汉字倍高倍宽
    {'-',1,FsSelectHzULine},	//选择/取消汉字下划线

	{'p',2,FsLogoPrn},      // logo 图形数据打印
    {'q',1,FsLogoDownLoad},		// logo 图形数据下载
    {0,0,NULL}
};
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              GsCmd table                                    */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD GsCmdTbl[] =
{
//====== 黑标侦测相关命令 ======
    {VK_FF,0,GscBlankMark},      // 找到黑标，并前进到撕纸位置

    //*********** 新增二维码打印控制命令 *************
#ifdef INCLUDE_QR_CODE
    {'(',1,GsExpCmd},             // 扩展命令 (含二维码/OPOS/黑标侦测等命令)
#endif
    {'!',1,GsSetFontEnlarge},     // 选择字符大小
    {'$',2,GsSetAbsVertPosition}, // 也模式下设置枞向绝对位置
    {'\\',2,GsSetRelVertPosition},// 页模式中设定垂直的相对位置
    {'/',1,GsPrnBitimage},        // 打印下载位图
    {':',0,GsStartEndMacro},      // 开始/结束宏定义
    {'^',3,GsExeMacro},           // 执行宏命令
    {'V',1,GsCutPaper},           // 模式切纸模式(可半/全切)
    {'P',2,GsSetMotionUnit},      // 设置横向和垂直
    {'I',1,GsReadPrintID},        // 读打印机 ID
    {'B',1,GsSetBlackWhite},      // 选择/取消黑白反显打印模式
    {'b',1,GsSetSmoothMode},      //
    {'H',1,GsSetHRIPosition},     // 选择 HRI字符的打印位置 (不打印/条码上方/条码下方/条码上下方都打印)
    {'f',1,GsSetHRIFont},         // 选择 HRI 使用字体
    {'h',1,GsSetBarcodeHeight},   // 选择条码高度
    {'w',1,GsSetBarcodeWidth},    // 设置条码宽度
    {'k',1,GsPrintBarcode},       // 打印条码
    {'x',1,GsPrinterBarcodeLeft}, /* 打印条码左边距 */
    {'a',1,GsSetAutoAckStatus},   // 打开/关闭自动状态返传功能
    {'r',1,GsReadStatus},         // 返回状态
    {'L',2,GsSetLeftMargin},      // 设置左边距
    {'W',2,GsSetPageWidth},       // 设置条码水平尺寸
    {'v',6,GsPrintImage},         // 打印光栅位图
    {'S',0,GPExterntCut},         // 佳伯扩展命令
    {'U',3,GPExterntFilter},      // 佳伯扩展命令
    {'T',0,GPExterntCmd},         /* 佳伯扩展命令 */
    {0,0,NULL}
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              Dc2 Cmd table                                  */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD Dc2CmdTbl[] =
{
	{'T',0,Dc2PrintTestPage},	/* 打印测试页	*/
	{'#',1,Dc2SetPrintDark},	/* 设置打印浓度	*/
	{'*',2,Dc2PrintImage},		/* 打印图像1	*/
	{'V',2,Dc2PrintMsbImage},	/* 打印MSB位图	*/
	{'v',2,Dc2PrintLsbImage},	/* 打印LSB位图	*/
	{0,0,NULL}
};

//==================================================================================================
//函数  处理VK_ESC的命令
//输入  无
//输出  无
//返回  无
static u16 ICACHE_FLASH_ATTR
HandleCmd(u8 cmd)
{
	u8 i,result;
	u16 cmdCnt;
	uint8_t PrnCmd[32];
	STRPRNCMD *pFunc;

	if( cmd==VK_ESC )
	{
		pFunc = (STRPRNCMD *)EscCmdTbl;
		cmdCnt = sizeof(EscCmdTbl)/sizeof(STRPRNCMD);
	}
	else if( cmd==VK_GS )
	{
		pFunc = (STRPRNCMD *)GsCmdTbl;
		cmdCnt = sizeof(GsCmdTbl)/sizeof(STRPRNCMD);
	}
	else if( cmd==VK_FS )
	{
		pFunc = (STRPRNCMD *)FsCmdTbl;
		cmdCnt = sizeof(FsCmdTbl)/sizeof(STRPRNCMD);
	}
	else if( cmd==VK_DC2 )
	{
		pFunc = (STRPRNCMD *)Dc2CmdTbl;
		cmdCnt = sizeof(Dc2CmdTbl)/sizeof(STRPRNCMD);
	}
	else
	{
		return 0;
	}

	result = 0;
	if (WaitPrnBufBytes(1, PrnCmd, 500, _WAIT_NBYTE_EXIT) == 1)// tqy
	{
		for (i=0; i<cmdCnt; i++)
		{
			if (PrnCmd[0] == pFunc[i].codevalue)
			{
				result = 1;
				if (pFunc[i].waitcount > 0)
				{
					if (WaitPrnBufBytes(pFunc[i].waitcount,PrnCmd, 500,_WAIT_NBYTE_EXIT) != pFunc[i].waitcount)
					{
						result = 0;
					}
				}
				if (result)
				{
					if (pFunc[i].codefunc != NULL)
					{
						pFunc[i].codefunc(PrnCmd);
					}
				}
				break;
			}
		}
		if(i==cmdCnt)
		{
			return(0x100+PrnCmd[0]);
		}
	}
	return(0);
}

//==============================================================================
/*
*函数功能: 处理实时状态传送命令
*输入参数: ch    控制字符值
*输出参数: 无
*返回值  : 无
*简单功能描述:DEL EOT  n  实时状态传送 (不管联机与否)
*             DEL ENQ  n  实时强制执行命令 (不管联机与否)
*/
//==============================================================================
void ICACHE_FLASH_ATTR
HandleDleCom(void)
{
	u8 buf[4];
	u8 temp;
	if (WaitPrnBufBytes(2, buf, 500, _WAIT_NBYTE_EXIT))   // tqy
	{
		if (buf[0] == VK_EOT)
		{
			switch( buf[1] )
			{
				case 1:
					temp =	((strprnprop.printenable==0)<<3) | (1<<1) | (1<<4);
				//	USART_To_USB_Send_Byte(temp);
					break;
				case 2:

		#if INCLUDE_PLATENSW
					temp =	((PaperStatus & NOPRESSSW) <<2 ) | (GET_FEEDKEY_STATUS()<<3) | ((PaperStatus & NOPAPER) <<5 ) | (1<<1) | (1<<4);
		#else
					temp =	((PaperStatus & NOPAPER) <<2 ) | ((PaperStatus & NOPAPER) <<5 ) | (1<<1) | (1<<4);
		#endif
				//	USART_To_USB_Send_Byte(temp);
					break;
				case 3:
					temp =	((_PRINTER_OVERHEAT()) | (1<<1) | (1<<4));
				//	USART_To_USB_Send_Byte(temp);
					break;
				case 4:
					temp =
#if INCLUDE_PAPER_SCROLL
						((PaperStatus & NOSCROLL)?0x0C:0)
#else
						((PaperStatus & NOPAPER)?0x0C:0 )
#endif
						| ((PaperStatus & NOPAPER)?0x60:0) | (1<<1) | (1<<4);

				//	USART_To_USB_Send_Byte(temp);
					break;
				default:
					break;
			}
		}
	}
}


void ICACHE_FLASH_ATTR
VK_HECR_PROC(void)	// tqy	没用的
{
#if 0
	uint8_t *rxbuf;
	uint16_t Len;
	STR_PECRPORTOCOL lan;

	rxbuf = common_ram;
	Len = WaitPrnBufBytes(301,rxbuf,1000,_WAIT_NBYTE_EXIT);
	if ( (rxbuf[299]==LAN_ETX) && (Len >= 300))
	{
		if (Decode_OK(rxbuf, &lan) == TRUE)
		{
			switch (LAN_FUN_DoPackage(&lan))
			{
                case NET_ACK:
                    USART_To_USB_Send_Byte(LAN_ACK);
                    break;
                case NET_LOAD:
					if (lan.para2 == 0)  /* 上传ROM */
					{
						ReadNByteFromSst(&lan.buf[0],0x100,SETTING);
						sprintf((char *)&lan.buf[0xb0], "PP7M3");	/* 表示新机型，上位机将只下载三个参数包 */
					}
                    Encode(&lan);
                    break;
                case NET_DISACK:
					USART_To_USB_Send_Byte(LAN_ACK);
                    //SetBaudRate(_B9600);
                    break;
                case NET_DISNET:
                    break;
                case NET_CHGBT:
					USART_To_USB_Send_Byte(LAN_ACK);
                    SetBaudRate(lan.buf[10]);
                    break;
                case NET_DownPrg:
					USART_To_USB_Send_Byte(LAN_ACK);
                    ProgramSwitch();
                    break;
                case NET_ERR:
                default:
                    break;
            }
		}
	}
#endif	
}



//==============================================================================
/*
*函数功能:处理控制字符
*输入参数:ch控制字符值
*输出参数:无
*返回值  :无
*/
//==============================================================================
void ICACHE_FLASH_ATTR
HandleCtrlChar(uint8_t ch)
{
    switch(ch)
    {
#ifdef DEBUG_MODE	// 测试
		case 0x02:
		{
			u8 buf[8];
			u16 speed = 0;
			// 02 heattime maxspeed
			if (WaitPrnBufBytes(2, buf, 100, _WAIT_NBYTE_EXIT) == 2)
			{
				if (buf[0])
				{
					BASEHEATTIME = buf[0] * 10;
				}
				if (buf[1])
				{
					PrnDrv.Prn_MaxSpeed = buf[1];
				}
				if (BASEHEATTIME < 100)
				{
					BASEHEATTIME = 100;
				}
				else if (BASEHEATTIME >= 600)
				{
					BASEHEATTIME = 600;
				}
				speed = 125000/MOTOR_STEP_COUNT/(BASEHEATTIME+10);
				if (speed > 150)
				{
					speed = 150;
				}
				if (PrnDrv.Prn_MaxSpeed > 150)
				{
					PrnDrv.Prn_MaxSpeed = 150;
				}
				else if (PrnDrv.Prn_MaxSpeed < 60)
				{
					PrnDrv.Prn_MaxSpeed = 60;
				}
				os_printf("heat=%dus, speed=%dmm/s, maxspeed=%dmm/s\r\n", BASEHEATTIME, speed, PrnDrv.Prn_MaxSpeed);
				os_printf("\r\n");
			}
		}
		break;
#endif
        case VK_PPECR:          //接收到PPECR的数据包标志，等待数据包
#if (PAGE_MODE_EN)
            PageCircle(224, 160, 50);
#endif
            VK_HECR_PROC();
            break;
            /* 页模式打印缓冲区的数据并返回标准模式 */
        case VK_FF:
            /* 如果连续值 */
            if (strprnprop.paper_type == SERIES_PAPER)
            {
                /* 标准模式相当于LF(0x0a)*/
                PutLineDotBufToPrnDotBuf();
                strprnprop.current_row = strprnprop.left_space;
            }
            break;
        case VK_LF:             //打印换行(0x0a)
            if( !P_STRLineBuf->max_high  )
			{
	        	P_STRLineBuf->max_high = FONT_MAP[strprnprop.font].h;
				os_memset(&P_STRLineBuf->LineDotBuf[0][0], 0, sizeof(P_STRLineBuf->LineDotBuf));
			}
			if (!bBitMapFlag) 
			{
				PutLineDotBufToPrnDotBuf();
			}
			bBitMapFlag = 0;
			strprnprop.htabcnt = 0;
            break;
        case VK_CR:             //光标返回,回车(0x0d)
            break;
        case VK_BACKSPACE:      //退格,
            break;
        case VK_NUL:            //清除当前打印缓冲区数据
            break;
        case VK_WIDTH:          //宽度放大
            break;
        //case VK_HEIGHT:         //高度放大
        //    break;
        case VK_LARGE:          //高度宽度放大
            break;
        case VK_DELMARK:        //删除线
            break;
        case VK_UNDERLINE:      //下划线
            break;
        case VK_REVERSE:        //反白
            break;
        case VK_HT:             //TAB键
            CursorToNextTabPosition();
            break;
        case VK_SO:
            break;
        case VK_DC4:
            break;
        case VK_SI:
            break;
        case VK_DC2:
            os_printf("\r\nhaha!");
            break;
        case VK_CAN: //在页模式下，删除当前打印区域中所有打印数据
#if (PAGE_MODE_EN)
            PrnPageInit();
#endif
			break;
		default:
            break;
    }
}

uint32_t ICACHE_FLASH_ATTR
PrnEscHandle(void)
{
    u8  ch;
    u16 dovalue = 0;
    u8 buf[32];

    if (WaitPrnBufBytes(1, buf, 0, _WAIT_NBYTE_EXIT) == 0)
    {
        return 0;						/* 没数据 */
    }
    ch = buf[0];
    if (ch < 0x20)						/* 如果是控制字符 */
    {
        if(ch==VK_DLE) 					/* 强制打印机动作(不管打印机是否ONLINE) */
        {
            HandleDleCom();
            return 0;
        }
        else if(ch==VK_ESC)    			/* 如果接收到 GS 则处理相应命令 */
        {
            //处理ESC命令
            dovalue = HandleCmd(VK_ESC);
        }
		else if( strprnprop.printenable==0 )
		{
			os_printf("打印机离线\r");
			/**打印机离线**/
		}
        else if (ch==VK_GS)        		/* 如果接收到 GS 则处理相应命令 */
        {
            /* 处理GS命令 */
            dovalue = HandleCmd(VK_GS);
        }
        else if (ch==VK_FS)      		/* 如果接收到 FS 则处理相应命令 */
        {
            /* 处理FS命令 */
            dovalue = HandleCmd(VK_FS);
        }
		else if( ch==VK_DC2 )
		{
			/* 处理DC2命令 */
            dovalue = HandleCmd(VK_DC2);
		}
#ifdef MUL_LANG  						/* 如果是越南文 */
        else if((27==strprnprop.codetbl) && (0x02==ch) || (0x05==ch) || (0x06==ch) || (0x14==ch) || (0x19==ch) || (0x1e==ch)) // 越南文处理
        {
            CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
        }
#endif
        else
        {
            /* 处理其它打印控制字符 */
            HandleCtrlChar(ch);
        }
    }
    else
    {
		if (strprnprop.printenable == 0)
		{
			os_printf("打印机离线\r");
			/**打印机离线**/
			return 0;
		}
#ifdef MUL_LANG
		else if((27==strprnprop.codetbl) && (ch >=0x80))	/* 越南文 */
		{
			buf[0] = VietnamChange(ch);
		}
		CharacterToCharacterDotBuf(buf[0], SINGLE_CHAR);		/* 按单字节处理 */
#else
		else if ((!PrintFontType) || (ch < 0x80))	// 英文字符
		{
			/* 单字节 */
			CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
		}
		else if ((PrintFontType == FONT_JAPANESE) && (ch >= 0xa0) &&  (ch <= 0xdf))	// 如果是日文半角字符
		{
			CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
		}
		else
		{
			/* 双字节 */
			if( WaitPrnBufBytes(1, &buf[1], 500,_WAIT_NBYTE_EXIT) )
			{
				if (PrintFontType == FONT_JCHINESE)	// 简体字
				{
					// 采用GB2312字库
        			{
						if (((ch >= 0xb0) && (ch <= 0xf7)) && (buf[1] >= 0xa1))
		                {
		                    CharacterToCharacterDotBuf((ch-0xa1)*94+(buf[1]-0xa1), DOUBLE_CHAR);
		                }
		                else if ((ch >= 0xa1) && (ch <= 0xa3) && (buf[1] >= 0xa1)) // 全角符号区
		                {
		                    CharacterToCharacterDotBuf((ch-0xa1)*94+(buf[1]-0xa1), DOUBLE_CHAR);
		                }
		                else
		                {
		                    CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                    CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                }
					}

				}
            }
			else
			{
				/* 单字节 */
				CharacterToCharacterDotBuf(ch, SINGLE_CHAR);			/* 字体内码到字符点阵的转换 */
			}
		}
#endif
    }
    return (dovalue);
}

