#include "includes.h"

u8 HzAreaF = 0;
u8 NoHzF = 0;

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
    {VK_FF,0,EscPrnPageData},         // ҳģʽ�´�ӡ
    {'!',1,EscSetPrintmode},          // ! 21 ѡ���ӡģʽ
    {'-',1,EscSetUnderLine},          // - 2D ѡ��/ȡ���»���ģʽ
    {'E',1,EscSetEmphasize},          // E 45 ѡ��/ȡ���Ӵ�ģʽ
    {'G',1,EscSetDoubleStrike},       // G 47 ѡ��/ȡ��˫�ش�ӡģʽ
    {'{',1,EscSetUpDown},             // { 7B ѡ��/ȡ�����ô�ӡģʽ
    {'V',1,EscSetRotate90},           // V 56 ѡ��/ȡ��˳ʱ����ת90��
    {'3',1,EscSetLineSpace},          // 3 33 �����м��
    {' ',1,EscSetCharSpace},          //   20 �����ַ��Ҽ��
    {'%',1,EscSetCancelUserCharSet},  // % 25 ѡ��/ȡ���û��Զ����ַ�
    {'&',0,EscSetUserCharPattern},    // & 26 �����û��Զ����ַ��ֿ�
    {'?',1,EscCancelUserChar},        // ? 3F ȡ�������û��Զ����ַ��ֿ�
    {'R',1,EscSelectCharsSet},        // R 52 ѡ������ַ���
    {'t',1,EscSelectCharsTbl},        // t 74 ѡ���ַ������(codepage)
    {'D',0,EscSetTabsPos},            // D 44 ���ú�������λ��
    {'J',1,EscPrnBufferFeed},         // J 4A ��ֽn����
    {'d',1,EscPrnBuffer},             // d 64 ��ӡ n �ַ���
    {'p',3,EscOutDevice},             // p 70 ����Ǯ���������
    {'q',3,GPOutDevice},              // q 71 ����Ǯ���������(�Ǳ�׼EPSON����,��Ϊ�Ѳ���ӡ��ר��)
    {'c',3,EscSetKeyAvailable},       // c 63 ѡ���ӡֽ�����������ȱֽ�ź�
    {'$',2,EscSetAbsPosition},        // $ 24 �趨ˮƽ��ʼ��ӡ����λ��((n1+n2*256)��)
    {'\\',2,EscSetRelPosition},       // \ 5C ������Ժ����ӡλ��
    {'W',8,EscSetPageSetting},        // W 57 ����ҳģʽ�´�ӡ����
    {'T',1,EscSetPrnDirection},       // T 54 ����ҳģʽ�´�ӡ������
    {'u',1,EscReadOutdeviceStatus},   // u 75
    {'v',0,EscReadPaperStatus},       // v 76
    {'i',0,EscCutPaper},              // i 69 ����ֽ
    {'@',0,EscInitPrinter},           // @ 40 ��ʼ����ӡ��
    {'=',1,EscSetPrinterOnLine},       // = 3D ѡ������豸( = 1 ��ӡ�� = 2 ��ʾ��)
    {'L',0,EscSelectPageMode},        // L 4C ѡ��ҳģʽ
    {'S',0,EscSelectStandardMode},    // S 53 ѡ���׼ģʽ
    {'2',0,EscSetDefLineSpace},       // 2 32 ����Ĭ���м��
    {'*',3,EscSetBitimageMode},       // * 2A ѡ���ӡλͼģʽ
    {'r',0,EscLQ1600KNoUseCmd},       // r 72 ѡ���ӡ��ɫ
    {'U',1,EscLQ1600KNoUseCmd},       // U 55 ;;n=0,1 ѡ���ӡ����,0:˫�� 1: ����
    {'x',1,EscLQ1600KNoUseCmd},       // x 78 ;;ѡ���ӡƷ��
    {0x19, 1,EscLQ1600KNoUseCmd},     // 19  ;;�趨�Զ�װֽװ��
    {'6',0,EscLQ1600KNoUseCmd},       // 6    ;;ȡ��128~159�Ŀ����빦��
    {'C',3,GpOpenDrawer},         // Ϊ��Ѳ���ӡ�����ݶ���
    {'a',1,EscSetPageLineMode},       // ����ģʽ (��/��/����)
    {'M',1,EscSetFontSize},         	// ���������С
    {'l',9,EscSetLine},               // 1b 6c ��ֱ��
    {'7',3,EscSettingPrinter},		  /* ���ô�ӡ���� */
    {VK_SO,0,EscSetDoubleWidth},
	{VK_DC4,0,EscClearDoubleWidth},
	{'8',2,EscSetSystemSleep},		  /*����ϵͳ˯��*/
    {0,0,NULL}
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              FsCmd table                                    */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD FsCmdTbl[] =
{
    {'a',1,FSChangeBaudTate},	// �ı䲨����
    {'b',1,FSSetPrintDensity},	// �ı��ӡŨ��
    {'!',1,FsSetPrintmode},		//�趨���ִ�ӡģʽ���
    {'S',2,FsSetWordInt},		//�趨�����ּ��
    {'W',1,FsSelectHzBhw},		//ѡ��/ȡ�����ֱ��߱���
    {'-',1,FsSelectHzULine},	//ѡ��/ȡ�������»���

	{'p',2,FsLogoPrn},      // logo ͼ�����ݴ�ӡ
    {'q',1,FsLogoDownLoad},		// logo ͼ����������
    {0,0,NULL}
};
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              GsCmd table                                    */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD GsCmdTbl[] =
{
//====== �ڱ����������� ======
    {VK_FF,0,GscBlankMark},      // �ҵ��ڱ꣬��ǰ����˺ֽλ��

    //*********** ������ά���ӡ�������� *************
#ifdef INCLUDE_QR_CODE
    {'(',1,GsExpCmd},             // ��չ���� (����ά��/OPOS/�ڱ���������)
#endif
    {'!',1,GsSetFontEnlarge},     // ѡ���ַ���С
    {'$',2,GsSetAbsVertPosition}, // Ҳģʽ�������������λ��
    {'\\',2,GsSetRelVertPosition},// ҳģʽ���趨��ֱ�����λ��
    {'/',1,GsPrnBitimage},        // ��ӡ����λͼ
    {':',0,GsStartEndMacro},      // ��ʼ/�����궨��
    {'^',3,GsExeMacro},           // ִ�к�����
    {'V',1,GsCutPaper},           // ģʽ��ֽģʽ(�ɰ�/ȫ��)
    {'P',2,GsSetMotionUnit},      // ���ú���ʹ�ֱ
    {'I',1,GsReadPrintID},        // ����ӡ�� ID
    {'B',1,GsSetBlackWhite},      // ѡ��/ȡ���ڰ׷��Դ�ӡģʽ
    {'b',1,GsSetSmoothMode},      //
    {'H',1,GsSetHRIPosition},     // ѡ�� HRI�ַ��Ĵ�ӡλ�� (����ӡ/�����Ϸ�/�����·�/�������·�����ӡ)
    {'f',1,GsSetHRIFont},         // ѡ�� HRI ʹ������
    {'h',1,GsSetBarcodeHeight},   // ѡ������߶�
    {'w',1,GsSetBarcodeWidth},    // ����������
    {'k',1,GsPrintBarcode},       // ��ӡ����
    {'x',1,GsPrinterBarcodeLeft}, /* ��ӡ������߾� */
    {'a',1,GsSetAutoAckStatus},   // ��/�ر��Զ�״̬��������
    {'r',1,GsReadStatus},         // ����״̬
    {'L',2,GsSetLeftMargin},      // ������߾�
    {'W',2,GsSetPageWidth},       // ��������ˮƽ�ߴ�
    {'v',6,GsPrintImage},         // ��ӡ��դλͼ
    {'S',0,GPExterntCut},         // �Ѳ���չ����
    {'U',3,GPExterntFilter},      // �Ѳ���չ����
    {'T',0,GPExterntCmd},         /* �Ѳ���չ���� */
    {0,0,NULL}
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                              Dc2 Cmd table                                  */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
const STRPRNCMD Dc2CmdTbl[] =
{
	{'T',0,Dc2PrintTestPage},	/* ��ӡ����ҳ	*/
	{'#',1,Dc2SetPrintDark},	/* ���ô�ӡŨ��	*/
	{'*',2,Dc2PrintImage},		/* ��ӡͼ��1	*/
	{'V',2,Dc2PrintMsbImage},	/* ��ӡMSBλͼ	*/
	{'v',2,Dc2PrintLsbImage},	/* ��ӡLSBλͼ	*/
	{0,0,NULL}
};

//==================================================================================================
//����  ����VK_ESC������
//����  ��
//���  ��
//����  ��
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
*��������: ����ʵʱ״̬��������
*�������: ch    �����ַ�ֵ
*�������: ��
*����ֵ  : ��
*�򵥹�������:DEL EOT  n  ʵʱ״̬���� (�����������)
*             DEL ENQ  n  ʵʱǿ��ִ������ (�����������)
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
					USART_To_USB_Send_Byte(temp);
					break;
				case 2:
#if INCLUDE_CUTPAPER
		#if INCLUDE_PLATENSW
					temp =	((PaperStatus & NOPRESSSW) <<2 ) | (GET_FEEDKEY_STATUS()<<3) | ((PaperStatus & NOPAPER) <<5 ) | ( CutPaperF<<6 ) | (1<<1) | (1<<4);
		#else
					temp =	((PaperStatus & NOPAPER) <<2 ) | (GET_FEEDKEY_STATUS()<<3) | ((PaperStatus & NOPAPER) <<5 ) | ( CutPaperF<<6 ) | (1<<1) | (1<<4);
		#endif
#else
		#if INCLUDE_PLATENSW
					temp =	((PaperStatus & NOPRESSSW) <<2 ) | (GET_FEEDKEY_STATUS()<<3) | ((PaperStatus & NOPAPER) <<5 ) | (1<<1) | (1<<4);
		#else
					temp =	((PaperStatus & NOPAPER) <<2 ) | (GET_FEEDKEY_STATUS()<<3) | ((PaperStatus & NOPAPER) <<5 ) | (1<<1) | (1<<4);
		#endif
#endif
					USART_To_USB_Send_Byte(temp);
					break;
				case 3:
#if INCLUDE_CUTPAPER
					temp =	( CutPaperF<<3 ) | ((_PRINTER_OVERHEAT()||_PRINTER_OVERPOWER())<<6) | (1<<1) | (1<<4);
#else
					temp =	((_PRINTER_OVERHEAT()) | (1<<1) | (1<<4);
#endif
					USART_To_USB_Send_Byte(temp);
					break;
				case 4:
					temp =
#if INCLUDE_PAPER_SCROLL
						((PaperStatus & NOSCROLL)?0x0C:0)
#else
						((PaperStatus & NOPAPER)?0x0C:0 )
#endif
						| ((PaperStatus & NOPAPER)?0x60:0) | (1<<1) | (1<<4);

					USART_To_USB_Send_Byte(temp);
					break;
				default:
					break;
			}
		}
	}
}


void ICACHE_FLASH_ATTR
VK_HECR_PROC(void)	// tqy
{
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
					if (lan.para2 == 0)  /* �ϴ�ROM */
					{
						ReadNByteFromSst(&lan.buf[0],0x100,SETTING);
						sprintf((char *)&lan.buf[0xb0], "PP7M3");	/* ��ʾ�»��ͣ���λ����ֻ�������������� */
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
}



//==============================================================================
/*
*��������:��������ַ�
*�������:ch�����ַ�ֵ
*�������:��
*����ֵ  :��
*/
//==============================================================================
void ICACHE_FLASH_ATTR
HandleCtrlChar(uint8_t ch)
{
    switch(ch)
    {
#ifdef DEBUG_MODE	// ����
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
        case VK_PPECR:          //���յ�PPECR�����ݰ���־���ȴ����ݰ�
#if (PAGE_MODE_EN)
            PageCircle(224, 160, 50);
#endif
            VK_HECR_PROC();
            break;
            /* ҳģʽ��ӡ�����������ݲ����ر�׼ģʽ */
        case VK_FF:
            /* �������ֵ */
            if (strprnprop.paper_type == SERIES_PAPER)
            {
                /* ��׼ģʽ�൱��LF(0x0a)*/
                PutLineDotBufToPrnDotBuf();
                strprnprop.current_row = strprnprop.left_space;
            }
            break;
        case VK_LF:             //��ӡ����(0x0a)
            if( !P_STRLineBuf->max_high  )
			{
	        	P_STRLineBuf->max_high = FONT_MAP[strprnprop.font].h;
				os_memset(&P_STRLineBuf->LineDotBuf[0][0], 0, sizeof(P_STRLineBuf->LineDotBuf));
			}
			if (!bBitMapFlag) // tqy
			{
				PutLineDotBufToPrnDotBuf();
			}
			bBitMapFlag = 0;
			strprnprop.htabcnt = 0;
            break;
        case VK_CR:             //��귵��,�س�(0x0d)
            break;
        case VK_BACKSPACE:      //�˸�,
            break;
        case VK_NUL:            //�����ǰ��ӡ����������
            break;
        case VK_WIDTH:          //��ȷŴ�
            break;
        //case VK_HEIGHT:         //�߶ȷŴ�
        //    break;
        case VK_LARGE:          //�߶ȿ�ȷŴ�
            break;
        case VK_DELMARK:        //ɾ����
            break;
        case VK_UNDERLINE:      //�»���
            break;
        case VK_REVERSE:        //����
            break;
        case VK_HT:             //TAB��
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
        case VK_CAN: //��ҳģʽ�£�ɾ����ǰ��ӡ���������д�ӡ����
#if (PAGE_MODE_EN)
            PrnPageInit();
#endif
			break;
		default:
            break;
    }
}

#ifndef MUL_LANG
//����  �ַ��������
//����  ��
//���  ��
//����  ��
void ICACHE_FLASH_ATTR
GetGB18030HzAddr(u16 hzcode)
{
	u8 ch,ch1;
	u16 hzaddr;
//						  HzAreaF = 0; // ������

	ch	= hzcode/0x100;
	ch1 = hzcode%0x100;

	if ((ch >= 0xa1) && (ch <= 0xa9) && (ch1 >= 0x40)&& (ch1 <= 0xfe)) // ȫ�Ƿ�����(1��)
	{
		HzAreaF = 0; // ������
		if (ch1 > 0xa0) // GB 2312������ A1A1 - A9FE 9x94 = 846 ������
		{
			hzaddr = (ch-0xa1)*94+(ch1-0xa1);
		}
		else  // 5�� (a840-a87e; a880-a8a0; a940-a97e; a980-a9a0; )
		{
			if (ch != 0xa8) // 0xa9
			{
				hzaddr = 846 + 0x60;
			}
			else
			{
				hzaddr = 846;
			}

			if (ch1 > 0x7f)
			{
				hzaddr += 0x3f + ch1 - 0x80;
			}
			else
			{
				hzaddr += ch1 - 0x40;
			}
		}
	}

	else  if (((ch >= 0xB0) && (ch <= 0xf7)) && ((ch1 >= 0xa1) && (ch1 <= 0xfe)))
	{
		HzAreaF = 1; // GB2312

		hzaddr = (ch - 0xb0)*94 + ch1-0xa1;
	}
	else if (((ch >= 0x81) && (ch <= 0xa0)) && (((ch1 >= 0x40) && (ch1 <= 0x7e)) || ((ch1 >= 0x80) && (ch1 <= 0xFe))))
	{
		HzAreaF = 2;  // part 1
		if (ch1 >= 0x80)
		{
			hzaddr = ch1 - 0x41;
		}
		else
		{
			hzaddr = ch1 - 0x40;
		}
		hzaddr += (ch -0x81) * 0xbe;
	}
	else if (((ch >= 0xaa) && (ch <= 0xfe)) && (((ch1 >= 0x40) && (ch1 <= 0x7e)) || ((ch1 >= 0x80) && (ch1 <= 0xa0))))
	{
		HzAreaF = 3;   // part 2
		if (ch1 >= 0x80)
		{
			hzaddr = ch1 - 0x41;
		}
		else
		{
			hzaddr = ch1 - 0x40;
		}
		hzaddr += (ch - 0xaa) * 0x60;
	}
	else
	{
		NoHzF = 1;	 // �Ƿ��ַ�
	}

	if (NoHzF == 0)
	{
		//DCharToLineDotBuf(hzaddr);
		CharacterToCharacterDotBuf(hzaddr, DOUBLE_CHAR);
	}
	else
	{
		NoHzF = 1;	 // �Ƿ��ַ�
	}
}



void ICACHE_FLASH_ATTR
JapanCoversion (u16 hzcode) // ������ת��ΪGBK,����GBK�õ���Ӧ������
{
    u8 ch,ch1;
    u16 index1=0xff, index2=0xff;
    u8 tmpbuf[4];

    ch = hzcode/0x100;
    ch1= hzcode%0x100;

    if ((ch >= 0x81) && (ch <= 0x9f) && ((ch1 >= 0x40) && (ch1 <= 0xfc)))
    {
        index1 = (ch - 0x81)*0xbd ; // ��λ
        index2 = ch1 - 0x40 ;       // ��λ
    }
    else if ((ch >= 0xe0) && (ch <= 0xef) && ((ch1 >= 0x40) && (ch1 <= 0xfc)))
    {
        index1 = (ch - 0xe0)*0xbd + 31*0xbd; // ��λ
        index2 = ch1 - 0x40 ;                // ��λ
    }
    else if ((ch >= 0xfa) && (ch <= 0xfc) && ((ch1 >= 0x40) && (ch1 <= 0xfc)))
    {
        index1 = (ch - 0xfa)*0xbd + 47*0xbd; // ��λ
        index2 = ch1 - 0x40 ;                // ��λ
    }

    if ((index1 != 0xff) || (index2 != 0xff))
    {
        //ReadFromFlashDirect((Big5ToGbkIdx + (index1 + index2)*4), tmpbuf, 4, SST_FLASH); // �õ���Ӧ���������ַ
		ReadNByteFromSst(tmpbuf, 4, (Big5ToGbkIdx + (index1 + index2)*4));

        if (( ch == tmpbuf[0]) && (ch1 ==  tmpbuf[1])) // ��������Ӧ������������
        {
            GetGB18030HzAddr(tmpbuf[2]*0x100+tmpbuf[3]);
        }
        else NoHzF = 1;   // �Ƿ��ַ�
    }
    else NoHzF = 1;   // �Ƿ��ַ�

    if (NoHzF)
    {
		CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		CharacterToCharacterDotBuf('?', SINGLE_CHAR);
    }
}

#endif

uint32_t ICACHE_FLASH_ATTR
PrnEscHandle(void)
{
    u8  ch;
    u16 dovalue = 0;
    u8 buf[32];

    if (WaitPrnBufBytes(1, buf, 0, _WAIT_NBYTE_EXIT) == 0)
    {
        return 0;						/* û���� */
    }
    ch = buf[0];
    if (ch < 0x20)						/* ����ǿ����ַ� */
    {
        if(ch==VK_DLE) 					/* ǿ�ƴ�ӡ������(���ܴ�ӡ���Ƿ�ONLINE) */
        {
            HandleDleCom();
            return 0;
        }
        else if(ch==VK_ESC)    			/* ������յ� GS ������Ӧ���� */
        {
            //����ESC����
            dovalue = HandleCmd(VK_ESC);
        }
		else if( strprnprop.printenable==0 )
		{
			os_printf("��ӡ������\r");
			/**��ӡ������**/
		}
        else if (ch==VK_GS)        		/* ������յ� GS ������Ӧ���� */
        {
            /* ����GS���� */
            dovalue = HandleCmd(VK_GS);
        }
        else if (ch==VK_FS)      		/* ������յ� FS ������Ӧ���� */
        {
            /* ����FS���� */
            dovalue = HandleCmd(VK_FS);
        }
		else if( ch==VK_DC2 )
		{
			/* ����DC2���� */
            dovalue = HandleCmd(VK_DC2);
		}
#ifdef MUL_LANG  						/* �����Խ���� */
        else if((27==strprnprop.codetbl) && (0x02==ch) || (0x05==ch) || (0x06==ch) || (0x14==ch) || (0x19==ch) || (0x1e==ch)) // Խ���Ĵ���
        {
            CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
        }
#endif
        else
        {
            /* ����������ӡ�����ַ� */
            HandleCtrlChar(ch);
        }
    }
    else
    {
		if (strprnprop.printenable == 0)
		{
			os_printf("��ӡ������\r");
			/**��ӡ������**/
			return 0;
		}
#ifdef MUL_LANG
		else if((27==strprnprop.codetbl) && (ch >=0x80))	/* Խ���� */
		{
			buf[0] = VietnamChange(ch);
		}
		CharacterToCharacterDotBuf(buf[0], SINGLE_CHAR);		/* �����ֽڴ��� */
#else
		else if ((!PrintFontType) || (ch < 0x80))	// Ӣ���ַ�
		{
			/* ���ֽ� */
			CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
		}
		else if ((PrintFontType == FONT_JAPANESE) && (ch >= 0xa0) &&  (ch <= 0xdf))	// ��������İ���ַ�
		{
			CharacterToCharacterDotBuf(ch, SINGLE_CHAR);
		}
		else
		{
			/* ˫�ֽ� */
			if( WaitPrnBufBytes(1, &buf[1], 500,_WAIT_NBYTE_EXIT) )
			{
				if (PrintFontType == FONT_JCHINESE)	// ������
				{
					if (SstFlash_ID == Sst_25F040) // ����GB2312�ֿ�
        			{
						if (((ch >= 0xb0) && (ch <= 0xf7)) && (buf[1] >= 0xa1))
		                {
		                    CharacterToCharacterDotBuf((ch-0xa1)*94+(buf[1]-0xa1), DOUBLE_CHAR);
		                }
		                else if ((ch >= 0xa1) && (ch <= 0xa3) && (buf[1] >= 0xa1)) // ȫ�Ƿ�����
		                {
		                    CharacterToCharacterDotBuf((ch-0xa1)*94+(buf[1]-0xa1), DOUBLE_CHAR);
		                }
						#if 0
		                else if ((ch == 0xfe) && ((buf[1] > 0xa0) && (buf[1] < 0xff)) && (strprnprop.userdef_hzzk_allow[buf[1] - 0xa1] == 0xaa)) // �û��Զ��ֿ�
		                {
		                    CharacterToCharacterDotBuf(0xff00 + buf[1]);
		                }
						#endif
		                else
		                {
		                    CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                    CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                }
					}
					else	// ������SST25VVF016 ����GBK�ֿ�
					{
						NoHzF = 0;
		                GetGB18030HzAddr(ch*0x100 + buf[1]);

		                if  (NoHzF)   // �Ƿ��ַ�
		                {
							CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                    CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                }
					}
				}
				else if (PrintFontType == FONT_FCHINESE)        // ������
				{
		            NoHzF = 0;
					// �������ֿ⣬���Դ�ӡ����
		            if ((((ch >= 0xA1) && (ch <= 0xc6)) || ((ch >= 0xc9) && (ch <= 0xf9))) && ((buf[1] >= 0x40) && (buf[1] <= 0x7e) || (buf[1] >= 0xa1) && (buf[1] <= 0xfe) && (ch != 0xc6)))
		            {
		                u16 index1, index2,ch1;
		                u8 tmpbuf[4];

		                if (ch < 0xa4)
		                {
		                    index1 = (ch - 0xa1)*0xbf;
		                    index2 = buf[1] - 0x40;
		                }
		                else
		                {
		                    if (ch < 0xc7)
		                    {
		                        index1 = (ch - 0xa4)*157;
		                    }
		                    else  // C9������ (ע��C7 - C8Ϊ����)
		                    {
		                        index1 = (ch - 0xc9)*157 + 0x1519;
		                    }

		                    if ((buf[1] >= 0x40) && (buf[1] <= 0x7f))
		                    {
		                        index2 = buf[1] - 0x40;
		                    }
		                    else
		                    {
		                        index2 = (buf[1] - 0xa1) + 0x3f;
		                    }
		                }

		                ch1 = buf[1];

	                    if (ch < 0xa4) // ��ʽ�任
	                    {
	                        u8 temp0,temp1,temp2,temp3;

	                        //ReadFromFlashDirect((NewBig5ToGbkIdxFlagAddr + 0x10 + (index1 + index2)*4), tmpbuf, 4, SST_FLASH);
							ReadNByteFromSst(tmpbuf, 4, (NewBig5ToGbkIdxFlagAddr + 0x10 + (index1 + index2)*4));

	                        temp0 = tmpbuf[0];
	                        temp1 = tmpbuf[1];
	                        temp2 = tmpbuf[2];
	                        temp3 = tmpbuf[3];

	                        tmpbuf[0] = temp2;
	                        tmpbuf[1] = temp3;
	                        tmpbuf[2] = temp0;
	                        tmpbuf[3] = temp1;
	                    }
	                    else
	                    {
	                        //ReadFromFlashDirect((Big5ToGbkIdx + (index1 + index2)*4), tmpbuf, 4, SST_FLASH);
							ReadNByteFromSst(tmpbuf, 4, (Big5ToGbkIdx + (index1 + index2)*4));
	                    }

		                if (( ch == tmpbuf[2]) && (ch1 ==  tmpbuf[3])) // ��������Ӧ��BIG5��
		                {
		                    GetGB18030HzAddr(tmpbuf[0]*0x100+tmpbuf[1]);
		                }
		                else NoHzF = 1;   // �Ƿ��ַ�
		            }
		            else
		            {
		                NoHzF = 1;   // �Ƿ��ַ�
		            }

		            if  (NoHzF)   // �Ƿ��ַ�
		            {
						CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		                CharacterToCharacterDotBuf('?', SINGLE_CHAR);
		            }
		        }
				else				   // ���ı�־
				{
					NoHzF = 0;
					JapanCoversion(ch*0x100+buf[1]);//�õ������ֿ�
				}
            }
			else
			{
				/* ���ֽ� */
				CharacterToCharacterDotBuf(ch, SINGLE_CHAR);			/* �������뵽�ַ������ת�� */
			}
		}
#endif
    }
    return (dovalue);
}

