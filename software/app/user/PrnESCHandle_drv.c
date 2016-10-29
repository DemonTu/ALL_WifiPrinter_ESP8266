#include "includes.h"
//==============================================================================
static uint8_t GPDrvFlag = 0;
static uint8_t GPAfterOut = 0;
static u8 DrawerNunber; // Ǯ���
static u8 DrawerPulseHighTime;   // Ǯ�������ߵ�ƽʱ��
static u8 DrawerPulseLowTime;    // Ǯ�������͵�ƽʱ��

const STR_FONT_SIZE FONT_MAP[FONT_MAX]=
{
    {12, 24},
    {8,  16},
    {5,  7 }, /* ����ֿ�Ƚ��ر����ݵ�һ�к͵ڶ��ж��ǿհף����һ��Ҳ�ǿհף�ʹ��ʱע������һλ */
    {8,  8 },
    {16, 32},
    {9,	 24},
};

STRPRNPROP strprnprop;

/*
���������Ļ����㷨
i = (((Uint)buffer[1])<<8) + buffer[0];  // (n1+256*n2)
i = 203 * i + (i+2)/5;                    // 25.4mm*������λ/x/0.125
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
*��������: ����0x��buf1����buf0�� �ͻ�����λ�������Ӧ�Ĳ���ֵ
*�������: buf0�� buf1�� basic_uint (strprnprop.xunit or strprnprop.yunit)(0<=buf0<=255,0<=buf1<=255)
*�������: ��
*����ֵ  : ����ֵ
*/
//==============================================================================
uint32_t ICACHE_FLASH_ATTR
return_dot_step(uint8_t buf0, uint8_t buf1, uint8_t basic_unit)
{
	return (((uint16_t)buf1<<8) + buf0);
}

//==============================================================================
//����  �趨��ӡģʽ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetPrintmode(u8 *bufferin)
{
#if 0
	strprnprop.prop = bufferin[0];
	if( strprnprop.prop & _PRNUNDERLINE )
	{
		/* ȫ��ģʽĬ��Ϊ2�� */
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
		strprnprop.underline_para = 1;	// �»��ߴ�ϸ
	}
	else
	{
		strprnprop.prop &= ~_PRNUNDERLINE;
	}


#endif
	return (0);
}


//����  �趨�»���
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �趨�����ӡ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetDoubleWidth(u8 *buffer)
{
	strprnprop.prop |= _PRNENWIDTH;
	return 0;
}

//����  �趨�����ӡ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscClearDoubleWidth(u8 *buffer)
{
	strprnprop.prop &= ~_PRNENWIDTH;
	return 0;
}


//����  �趨����ģʽ
//����  buffer  �����������
//���  ��
//����  ����
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


//����  �趨˫���ڶ�ģʽ,Ч�������ģʽһ��
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetDoubleStrike(u8 *buffer)
{
    return (EscSetEmphasize(buffer));
}

//����  �趨�ַ����µ���
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �ַ�˳ʱ����ת90��
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �趨ȱʡ���и�
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetDefLineSpace(u8 *buffer)
{
	/* ��׼ģʽ������ת�����м�� */
	strprnprop.linespace = (DEFAULT_LINE_SPACE-FONT_MAP[FONT12X24].h);
	return (0);
}


//����  �趨�м��
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetLineSpace(u8 *buffer)
{
	strprnprop.linespace = buffer[0];
#if 0
	/* ��׼ģʽ�и� */
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


//����  �趨�ַ����
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetCharSpace(u8 *buffer)
{
	strprnprop.charspace = return_dot_step(buffer[0], 0, strprnprop.xunit);
	return (0);
}

//����  �趨ʹ�õĹ����ַ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSelectCharsSet(u8 *buffer)
{
    strprnprop.charsset = (ENUM_LANGUAGE)buffer[0];
    return (0);
}
// 'M'���������С
#if (SET_FONT_SIZE_EN)
u8 ICACHE_FLASH_ATTR
EscSetFontSize(u8 *buffer)
{
	/* һ�»����� */
	if (buffer[0] >= '0' )
	{
		buffer[0] -= '0';
	}
	/* �ж����� */
	if ( buffer[0] < FONT_MAX )
	{
		strprnprop.font = (ENUM_FONT_SIZE)buffer[0];
	}
	return 0;
}

#endif
//����  �趨ʹ�õ��ַ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSelectCharsTbl(u8 *buffer)
{
	strprnprop.codetbl = buffer[0];
	return (0);
}


//����  �趨ˮƽTABλ��
//����  buffer  �����������
//���  ��
//����  ����
/* PG_OK */
u8 ICACHE_FLASH_ATTR
EscSetTabsPos(u8 *buffer)
{
	uint8_t i;
	uint8_t temp;
	/* ��������������*/
	os_memset(strprnprop.htabpos, 0, sizeof(strprnprop.htabpos));
	temp = 0;
	for (i=0; i<TAB_NUM; i++)
	{
		/* ��һ��Ԥ�������ǿ����˳� */
		if (0 == WaitPrnBufBytes(1, buffer, 500,_WAIT_NBYTE_EXIT))
		{
			return i;
		}
		/* ��һ��Ԥ������С��ǰһ������ֹͣ */
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


//����  ��ӡ������������ ����ֽn�ַ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscPrnBuffer(u8 * buffer)
{
    PutLineDotBufToPrnDotBuf(); // ��ӡ����������
    PutBlankLineToPrintDotBuf(buffer[0]*(FONT_MAP[strprnprop.font].h)); // ��ֽn����
    return (0);
}

//����  ��ӡ������������,��ָ����ӡ���ֽ�ĳ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscPrnBufferFeed(u8 *buffer)
{
	if((0x18==buffer[0]) && GPDrvFlag)
	{
	   //���ݼѲ�������ӡ
	}
	else
	{
	    PutLineDotBufToPrnDotBuf();
	    PutBlankLineToPrintDotBuf(return_dot_step(buffer[0], 0, strprnprop.yunit)); // ��ֽn����
	}
    return (0);
}

//����  ����Ǯ�俪��(port=0/1; ��ƽ = 0 �أ�
//���
//����
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

//����  ������������
//����  buffer  �����������
//      ע�������ʽΪESC p m n1,n2
//          ����m ΪǮ��ţ�0��1��
//          ����n1 * 2ms ΪǮ�俪��ʱ��
//          ����n2 * 2ms ΪǮ��ر�ʱ��
//          0 < n1 < n2 < 255
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscOutDevice(u8 *buffer)	// tqy
{
	if ((buffer[0] == 48) || (buffer[0] == 49))
	{
		buffer[0] -= 48;
	}
	if (buffer[0] < 2)
	{
		SetDrawerLev(buffer[0],0x01);	// ��ǮǮ��
		MSctCnt = 0;		  // ��Ǯ����ʱ
		while(MSctCnt < buffer[1]*2)
		{
			//pollint();
		}
		SetDrawerLev(buffer[0],0x00);// ��ǮǮ��

		if (buffer[1] > buffer[2])
		{
			buffer[2] = buffer[1];
		}
		MSctCnt = 0;		  // ��Ǯ����ʱ
		while(MSctCnt < buffer[2]*2)
		{
			pollint();
			//PrnEscHandle();
		}
	}
	return 0;
}


//����  �趨����ȱֽ����Ϣ
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �趨��ӡ�ľ���λ��
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �趨��ӡ�����λ��
//����  buffer  �����������
//���  ��
//����  ����
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

//����  �������״̬
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscReadOutdeviceStatus(u8 * buffer)
{
    USART_To_USB_Send_Byte(0);	// tqy
    return (0);
}

//����  ���ֽ��״̬
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscReadPaperStatus(u8 * buffer)
{
	SendPrinterStatus();	//
    return (0);
}
//����  ��ֽ����
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscCutPaper(u8 * buffer)
{
    return (0);
}

//����  ��ʼ����ӡ��
//����  buffer  �����������
//���  ��
//����  ����
u8 EscInitPrinter(u8 * buffer)
{
    uint8_t i;

	os_memset(&strprnprop, 0, sizeof(STRPRNPROP));
	strprnprop.density = Density;
    /* �����ַ��� */
    strprnprop.charsset = USA;

    /* ����Ĭ������Ŀ�Ⱥ͸߶�*/
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
    /* ��ӡ����ĳ�ʼ�� */
    strprnprop.barcodewidth = MAXMORENWEIGH;		/* ���������ӡȱʡ��� (EPSONȱʡֵ) */
    strprnprop.barcodeleft = 0x00;					/* ��������ˮƽ��ӡȱʡλ�� */
    strprnprop.barcodeheight = MAXMORENHIGH;		/* ��������߶� */
    /* ����GS PĬ������Ļ�����λ */
    strprnprop.yunit = Y_DEFAULT_BASIC_MOVE_UNIT;	/* Ĭ��һ��0.125 */
    strprnprop.xunit = X_DEFAULT_BASIC_MOVE_UNIT;	/* Ĭ��һ��0.125 */
    /* ��׼ģʽ���� */
    strprnprop.linespace = (DEFAULT_LINE_SPACE-24);
    strprnprop.charspace = DEFAULT_CHAR_SPACE;
    /* ����TAB�����Ĭ��ֵ */
#define TABJUMP   8
    for (i=0; i<TAB_NUM; i++)
    {
        strprnprop.htabpos[i]=TABJUMP*(i+1);
    }
    /* bitmap */
    BitmapInit();
	TogglePrintOnLine(1);			/* ��ӡ��ȱʡ������״̬ */
    /*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
#if (PAGE_MODE_EN)
    /* ҳģʽ��ʼ�� */
    PrnPageInit();
#endif
#ifdef MUL_LANG
	Internationalcode(CodePage);	/* Ԥ������� */
#endif

    /*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
    return (*buffer);
}
/*
**��������: ����������
**�������:
**�������:
**����ֵ  :
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
* ����Ũ��
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

//����  ѡ����Χ����豸
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetPrinterOnLine(u8 * buffer)
{
	TogglePrintOnLine(buffer[0]);
	return (0);
}


//����  �л���ӡ������ҳģʽ
//����  buffer  �����������
//���  ��
//����  ����
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

// �趨ҳ��Ϊ buffer[0] Ӣ��
u8 ICACHE_FLASH_ATTR
GpOpenDrawer(u8 * buffer)
{
	if (GPDrvFlag)
	{
		// ������ӡ��
		_MOTOR_FEED_MM(15);
	}
    GPDrvFlag = 0;

    //��ӡ��ɺ��Ƿ�Ǯ��
    if (GPAfterOut)
    {
        GPAfterOut = 0;

		buffer[1] = DrawerPulseHighTime;   // Ǯ�������ߵ�ƽʱ��
		buffer[2] = DrawerPulseLowTime;    // Ǯ�������͵�ƽʱ��
		if (DrawerNunber & 0x01)
		{
			buffer[0] = 0;          // Ǯ���
            EscOutDevice(buffer);
		}

		if (DrawerNunber & 0x02)
        {
			buffer[0] = 1;			// Ǯ���
			EscOutDevice(buffer);
		}
		DrawerNunber = 0;
    }
    return(0);
}
//����  ��ʶ���LQ1600k ,������ִ�е�����
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscLQ1600KNoUseCmd(u8 * buffer)
{
    return (*buffer);
}

// �趨����ģʽ
// buffer[0] = 0 �����; = 1 ����; = 2 �Ҷ���;
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

//����  �趨���ִ�ӡģʽ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
FsSetPrintmode(u8 *bufferin)
{
    u8 * buffer;
    buffer=bufferin;
    strprnprop.prop = 0;

    if (buffer[0] & 0x80)
    {
        strprnprop.prop |= _PRNUNDERLINE;
		strprnprop.underline_para = 1;	// �»��ߴ�ϸ
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


//����  �趨�����ּ��
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
FsSetWordInt(u8 *buffer)
{
    return (0);
}

//����  ѡ��/ȡ�����ֱ��߱���
//����  buffer  �����������
//���  ��
//����  ����
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


//����  ѡ��/ȡ�������»���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
FsSelectHzULine(u8 *buffer)
{
	if ((buffer[0] < 3) || ((buffer[0] > 0x2f) && (buffer[0] < 0x33)))
    {
		strprnprop.underline_para = 0;	// �»��ߴ�ϸ

        buffer[0] &= 0x0f;

        if (buffer[0] == 0)
        {
            strprnprop.prop &= ~_PRNUNDERLINE;
        }
        else
        {
            strprnprop.prop |= _PRNUNDERLINE;
            //buffer[0] --;
			strprnprop.underline_para = buffer[0];	// �»��ߴ�ϸ
        }
     }
    return (0);
}



//����  ǰ��ֽ���ڱ�λ�� ��������ںڱ�λ���򲻶�����
//����  buffer  �����������
//���  ��
//����  ����
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


// �����ʽ GS ( F PL PH a m nL nH
// (pL + pH*256) * 0.125 mm = ����˺ֽλ������ںڱ����ƫ����
// a = 1 ������ʼ��ӡλ�������ڱ���λ�õ�ƫ����

//����  ���úڱ궨λƫ����
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
PrintTestPage(u8 *buffer)
{
#if (PRN_TEST_EN)
    print_test_page();	// tqy
#endif
    return (0);
}

//����  �趨��ӡ������ķŴ���
//����  buffer  �����������
//���  ��
//����  ����
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

//����  ��ʼ/�����궨��
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsStartEndMacro(u8 *buffer)
{
    return (0);
}

//����  �趨/ȡ������
//����  buffer  �����������
//���  ��
//����  ����
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

//����  ���ʹ�ӡ��ID
//����  buffer  ����������� 1d 49 n
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsReadPrintID(u8 *buffer)
{
	u8 ch;
	ch = buffer[0];
	switch( ch )
	{
		case 0: 	// �汾��
			os_printf("%s\r\n", _MACHINE_TYPE_STRING);
			break;
		case 1: 	// ϵͳ��ѹ
			os_printf("volt=%dmv\r\n", 5000);
			break;
		case 2: 	// ��ӡͷ����
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


//����  �趨��߾�
//����  buffer  �����������
//���  ��
//����  ����
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


//����  �趨ˮƽ��ֱ��λ
//����  buffer  �����������
//���  ��
//����  ����
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

//����  ��ֽ(��������ֽ��ʽ��ȫ�У����У���ֽ�����)
//�����ʽ: GS V m n
//ע: m = 0,48 Ϊ����ֽ
//ע: m = 1,49 Ϊȫ��ֽ
//ע: m = 65 ��ֽ n ���к�,ȫ��ֽ
//ע: m = 66 ��ֽ n ���к�,����ֽ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsCutPaper(u8 *buffer)
{
    return (0);
}

//����  �趨�����ӡ���
//����  buffer  �����������
//���  ��
//����  ����
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
//����  ִ�к�
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsExeMacro(u8 *buffer)
{
    return (0);
}

//����  ����/�ر��Զ�״̬����
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsSetAutoAckStatus(u8 *buffer)
{
    strprnprop.autostatus = buffer[0];
    return (0);
}

//����  ��������ģʽ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsSetSmoothMode(u8 *buffer)
{
    return (0);
}
//����  ����״̬
//����  buffer  �����������
//���  ��
//����  ����
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








//�Ѱ���չ����
u8 ICACHE_FLASH_ATTR
GPExterntCmd(u8 *buffer)
{
	return 0;
}

//�Ѱ���չ����
u8 ICACHE_FLASH_ATTR
GPExterntCut(u8 *buffer)
{
    return 0;
}

u8 ICACHE_FLASH_ATTR
GPOutDevice(u8 *buffer)
{
	if (buffer[0] == 1) DrawerNunber += 0x2;
	else DrawerNunber += 0x1;		  // Ǯ���
	DrawerPulseHighTime = buffer[1] ; // Ǯ�������ߵ�ƽʱ��
	DrawerPulseLowTime	= buffer[2] ; // Ǯ�������͵�ƽʱ��
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
*��������: ����궨λ����һ��TABλ��,�ر�ע����������ǰλ��+һ���ַ���ȴ�����һ��
*          �Ʊ�λ�������ƶ������¸��Ʊ�λ
*�������: ��
*�������: ��
*����ֵ  : ��
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
*                      �����ӡʹ�ú���
*
*******************************************************************************/
#if (BARCODE_EN)
//����  �趨fri������
//����  buffer  �����������
//      0   12x24
//      1    9x24
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsSetHRIFont(u8 *buffer)
{
	return (0);
}

//����  ѡ��HRI�ַ��Ĵ�ӡλ��
//      0   ����ӡ
//      1   ����������
//      2   ����������
//      3   ������������λ��
//����  buffer  �����������
//���  ��
//����  ����
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
		strprnprop.barcodeHRIpos = 0;				 // ���ó�Ĭ��
	}
	return (0);
}

//����  �������������Ŀ��
//      �趨    ������λ    խ����  ������
//      2       2           2       5
//      3       3           3       8
//      4       4           4       10
//      5       5           5       13
//      6       6           6       16
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsSetBarcodeWidth(u8 *buffer)
{
	if((buffer[0] < 2)||(buffer[0] > 6))	return 0;
	strprnprop.barcodewidth = buffer[0];
	return (0);
}

//����  ��������ĸ߶�
//      ȱʡ����߶���162
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsSetBarcodeHeight(u8 *buffer)
{
    strprnprop.barcodeheight = buffer[0];
    return (0);
}

//����  �����������߾�
//      ȱʡ������߾���0
//����  buffer  �����������
//���  ��
//����  ����
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

//����  ��ӡ����
//      ��������    ���볤��    ����ֵ��Χ
//      0 UPC-A     11-12       48-57
//      1 UPC-E     11-12       48-57
//      2 EAN13     12-13       48-57
//      3 EAN8       7- 8       48-57
//      4 CODE39     1-         48-57,65-90,32,36,37,43,45,46,47
//      5 ITF        1-ż��     48-57(�����ý���ʽ25���ӡ)
//      6 CODABAR    1-         48-57,65-68,36,43,45,46,47,58
//      7 CODE93     1-255      0-127
//      8 CODE128    2-255      0-127
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsPrintBarcode(u8 *buffer)
{
    u8 barlen;               //bartype��¼Ҫ��ӡ���������ͣ�barlen�洢Ҫ��ӡ����ĳ���
    u8 buf[256];

    /* �ж�����߶��Ƿ�Ϸ������Ϸ�������ȱʡ�ĸ߶� */
    if (strprnprop.barcodeheight == 0)
    {
        strprnprop.barcodeheight = MAXMORENHIGH;
    }
    /* �ж��������Ƿ�Ϸ������Ϸ�������ȱʡ�Ŀ�� */
    if (strprnprop.barcodewidth == 0)
    {
        strprnprop.barcodewidth = MAXMORENWEIGH;
    }
    /* ��ȡbarcode��ӡ���� */
    strprnprop.bartype = buffer[0];

	buffer = &buf[0];
    /* ��Ӧ����<GS> 'k' m d1 d2 d3 d4 ... dk <NUL> */
    if (strprnprop.bartype <= 0x08)
    {
        barlen = WaitPrnBufBytes(255,buffer,1000,_WAIT_ZERO_EXIT);
        barlen--;
    }
    /* ��Ӧ����<GS> 'k' m n d1 d2 d3 d4 ... dn */
    else if ((strprnprop.bartype >= 65) && (strprnprop.bartype <= 73))
    {
        strprnprop.bartype -= 65;
        if (WaitPrnBufBytes(1,buffer,500,_WAIT_NBYTE_EXIT) == 0)
        {
            return (0);
        }
        /* ��ȡ���� */
        barlen = buffer[0];
        /* �жϳ��� */
        if (barlen == 0)
        {
            return (0);
        }
        /* �������� */
        if (WaitPrnBufBytes(barlen,buffer,500,_WAIT_NBYTE_EXIT) != barlen)
        {
            return (0);
        }
    }
    /* ��֧�ֵ��������� */
    else
    {
        return (0);
    }
    if (barlen == 0)
    {
        return(0);
    }
    /* ���볤������ */
    if (barlen > MAXBARCODELEN - 4)
    {
        return (0);
    }
    /* ���0 */
    buffer[barlen] = '\0';
    PrnOneBarcodeSet(buffer);
    return (0);
}
#endif
/*******************************************************************************
*
*                             ҳģʽ���ú���
*
*******************************************************************************/
#if (PAGE_MODE_EN)
//����  ��ҳ ��ӡҳģʽ������
//����  buffer  �����������
//���  ��
//����  ����
u8 EscPrnPageData(u8 *buffer)
{
    StartPrnPage();
    return (0);
}
//����  �л���ӡ�������׼ģʽ
//����  buffer  �����������
//���  ��
//����  ����
u8 EscSelectStandardMode(u8 * buffer)
{
    strprnprop.pagemode = STANDERD_MODE;
    return (0);
}
//����  �趨��ӡ����
//����  buffer  �����������
//���  ��
//����  ����
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
            dir = 0;           //Ĭ��A
            break;
    }
    PageSetDir((ENUM_PAGE_PRN_DIR)dir);
    return (0);
}
//����  �趨
//����  buffer  �����������
//���  ��
//����  ����
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
//����  ҳģʽѡ��ֱλ�õľ���λ��
//����  buffer  �����������
//���  ��
//����  ����
u8 GsSetAbsVertPosition(u8 *buffer)
{
    uint32_t temp;
    if (0 == (PageGetDir()%2))    // ����
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.yunit);
        if (PAGE_OK == PageSideCheck(Y_CURSOR, temp))
        {
            PageSetCursor(Y_CURSOR, temp);
        }
    }
    else                              // ����
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.xunit);
        if (PAGE_OK == PageSideCheck(X_CURSOR, temp))
        {
            PageSetCursor(X_CURSOR, temp);
        }
    }
    return (0);
}
//����  ҳģʽ���趨��ֱ�����λ��
//����  buffer  �����������
//���  ��
//����  ����
u8 GsSetRelVertPosition(u8 *buffer)
{
    uint32_t temp = 0;
    if (0 == (PageGetDir()%2))    // ����
    {
        temp = return_dot_step(buffer[0], buffer[1], strprnprop.yunit)+PageGetCursor(Y_CURSOR);
        if (PAGE_OK == PageSideCheck(Y_CURSOR, temp))
        {
            PageSetCursor(Y_CURSOR, temp);
        }
    }
    else                              // ����
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
**��������:
**�������:
**�������:
**����ֵ  :
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
**��������: ���ô�ӡ����
**�������:
**�������:
**����ֵ  :
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
		// ���ֻ֧�ַ�DUP_HEAT_CNT��
		if( temp < (TOTALBIT/DUP_HEAT_CNT) )	temp = (TOTALBIT/DUP_HEAT_CNT);
		else if (temp > TOTALBIT) temp = TOTALBIT;
		MaxDotOfArea = temp;
	}
#endif
	return 0;
}

/*
**��������: ��ӡ����ҳ
**�������: ��
**�������:
**����ֵ  :
*/
u8 ICACHE_FLASH_ATTR
Dc2PrintTestPage(uint8_t *buffer)
{
	PrinterTestPage();	// tqy
	return 0;
}
/*
**��������: ���ô�ӡŨ��
**�������: ��
**�������:
**����ֵ  :
*/
u8 ICACHE_FLASH_ATTR
Dc2SetPrintDark(uint8_t *buffer)
{
	strprnprop.density = buffer[0];
	if( strprnprop.density > 4 )	strprnprop.density = 4;
	return (0);
}

/*
**��������: ����ϵͳ˯�߲���
**�������: ��
**�������:
**����ֵ  :
*/
u8 ICACHE_FLASH_ATTR
EscSetSystemSleep(uint8_t *buffer)
{
	uint32_t sleepDlyTm;

	sleepDlyTm = buffer[0]+(uint32_t)buffer[1]*256;
	if( sleepDlyTm )
	{
		if( sleepDlyTm<20 )	sleepDlyTm = 20;		/**��λ10ms**/
	}
	return (0);
}


