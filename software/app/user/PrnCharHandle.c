#include "includes.h"
//==============================================================================
P_STR_LINE_BUF P_STRLineBuf;

void ICACHE_FLASH_ATTR
PrnCharInit(void)
{
    P_STRLineBuf = (P_STR_LINE_BUF)common_ram;
    os_memset(P_STRLineBuf, 0, sizeof(P_STRLineBuf));
}

/*
*��������: �����������Խ�һ���ַ��������ݷŵ��е��󻺳���
*          �����Զ���ӡ
*�������: buf:�ַ�����ָ�룬w:�ַ���ȣ�h:�ַ��߶�
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
PutCharDot_To_LineDotBuf(uint8_t *buf, uint8_t w, uint8_t h)
{
    uint8_t i,j;                // ѭ���ñ���
    uint8_t bytes_off_width;    // ��¼���ַ��ֽڿ�� w��λ���

    /* ����ʱ�ж� */
    bytes_off_width = ((w+7)/8);
    /* ����� */
    /* ��׼ģʽ���� */
    if ((strprnprop.current_row+w) > (PrnDotOfLine - strprnprop.right_space))
    {
        PutLineDotBufToPrnDotBuf();
    }
    /* ��ʼ�����ַ� */
    for (i=0; i<h; i++)
    {
        for(j=0; j<bytes_off_width; j++)
        {
            FillBarByteBit((uint8_t *)(P_STRLineBuf->LineDotBuf[MAX_LINE_DOT_BUF_HIGH-h+i]),\
                           PrnDotOfByte, \
                           (strprnprop.current_row+(j<<3)),\
                           buf[bytes_off_width*i+j]);
        }
    }
    /* ��λ�ø��� */
    strprnprop.current_row += (w+strprnprop.charspace);
    /* ����и߸��� */
    if (P_STRLineBuf->max_high < h)
    {
        P_STRLineBuf->max_high = h;
    }

#if (PAGE_MODE_EN)
    /* ҳģʽ���� */
    else if (PAGE_MODE == strprnprop.pagemode)
    {
        PagePutbuf(w, h, buf, BUF_IS_CHAR);
    }
#endif
}
/*
*��������: ���ַ�������������ת����ŵ������л�����
*�������: first_char:���ĵ�һ����ĸ��second_char:���ĵڶ�����ĸ
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
CharacterToCharacterDotBuf(uint16_t ch, ENUM_CHAR_NUM char_num)
{
	uint8_t  w,h,temp;

	/* ��ȡ�����С */
	w = FONT_MAP[strprnprop.font].w;
	h = FONT_MAP[strprnprop.font].h;

	//�����ַ�����
	if (SINGLE_CHAR & char_num)
	{
		if (strprnprop.font == FONT12X24)
		{
			ReadASCII_12x24Data(P_STRLineBuf->Charmap, ch);
		}
		else
		{
			w = 9;
        	h = 24;
			ReadASCII_9x17Data(P_STRLineBuf->Charmap, ch);
			
		}
	}
#ifndef MUL_LANG
	else
	{
		if (strprnprop.font == FONT8X16 )	// 8X16��16X16����
		{
			w = 16;
			h = 16;
			ReadCHINESE_16x16Data(P_STRLineBuf->Charmap ,ch);
		}
		else
		{
			w = 24;
			h = 24;
			ReadCHINESE_24x24Data(P_STRLineBuf->Charmap ,ch);
		}
	}
#endif
	/* �ַ�����任 */
	/* ������ת90�� */
	if (strprnprop.prop & _PRNRollRight90)
	{
		RollRight90(P_STRLineBuf->Charmap, w, h);
		temp = h;
		h = w;
		w = temp;
	}
	/* ���巴�� */
	if (strprnprop.prop & _PRNREVERSE)
	{
		ReverseDisplay(P_STRLineBuf->Charmap, w, h);
	}
	/* ����Ӵ� */
	if (strprnprop.prop & _PRNEMPHASIZE)
	{
		Emphasize(P_STRLineBuf->Charmap, w, h);
	}
	/* ����ɾ���� */
	if (strprnprop.prop & _PRNDELMARK)
	{
		Del_Mark(P_STRLineBuf->Charmap, w, h);
	}
	/* �����»��� */
	if(strprnprop.prop & _PRNUNDERLINE)
	{
		Under_Line(P_STRLineBuf->Charmap, w, h, strprnprop.underline_para);
	}
/*�ڴ治�㣬�ݲ�֧�ֱ���*/
#if 0
	/* ���屶�� */
	if (strprnprop.prop & _PRNENHEIGHT)
	{
		DoubleTall(P_STRLineBuf->Charmap, w, h);
		h = h << 1;
	}
#else
	strprnprop.enheight = (strprnprop.prop & _PRNENHEIGHT)?1:0; 	/* ���屶�� */
#endif

	/* ���屶�� */
	if (strprnprop.prop & _PRNENWIDTH)
	{
		DoubleWide(P_STRLineBuf->Charmap, w, h);
		w = w << 1;
	}
	// ���ַ����󵽵�����buf
	PutCharDot_To_LineDotBuf(P_STRLineBuf->Charmap, w, h);
}


/*
*��������: ���ÿհ��н�����
*�������: ��
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
PutBlankLineToPrintDotBuf(u16 LineNum)
{
    uint8_t buf[PrnDotOfByte];
    os_memset(buf, 0, sizeof(buf));
    PutBlockToPrintDotBuf(buf, 1, LineNum);
}

/*
*��������: ���л��������ݷ�����󻺳���������ӡ�м��
*�������: ��
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
PutLineDotBufToPrnDotBuf(void)
{
	uint32_t i,j,k;
	uint8_t buf_temp;
	uint8_t *destbuf;
    /* ���л��������ݷ�����󻺳��� */
///////////////////////////////////////////////////////////////////////////////
    switch (strprnprop.LineModeM)
    {
        case flush_right:
            strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine+strprnprop.left_space-strprnprop.current_row);
            strprnprop.str_buf.src_shift_start_bit = (strprnprop.left_space);
            strprnprop.str_buf.shift_bit_total = (strprnprop.current_row-strprnprop.left_space);
            break;
        case flush_middle:
            strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine+strprnprop.left_space-strprnprop.current_row)>>1);
            strprnprop.str_buf.src_shift_start_bit = (strprnprop.left_space);
            strprnprop.str_buf.shift_bit_total = (strprnprop.current_row-strprnprop.left_space);
            break;
        case flush_left:
        default:
            break;
    }
    strprnprop.str_buf.src_buf_size = PrnDotOfByte;
///////////////////////////////////////////////////////////////////////////////
    if (strprnprop.prop & _PRNUPDOWN)
    {
        destbuf = (uint8_t*)&P_STRLineBuf->LineDotBuf[MAX_LINE_DOT_BUF_HIGH-1][0];// ���һ��
        for(i=0; i<(P_STRLineBuf->max_high); i++) // �������ݽ���180����ת
        {
            /* �����ֽڶԵ� */
            for (j=0; j<(PrnDotOfByte/2); j++)
            {
                buf_temp= destbuf[j];
                destbuf[j] = destbuf[PrnDotOfByte-1-j];
                destbuf[PrnDotOfByte-1-j] = buf_temp;
            }
            /* �����ֽڸߵ�λ�Ե� */
            for (j=0; j<PrnDotOfByte; j++)
            {
                if ((destbuf[j] != 0xFF) && (destbuf[j] != 0x00))
                {
                    buf_temp = 0;
                    for (k=0; k<8; k++)
                    {
                        if (destbuf[j] & bitmap[k])
                        {
                            buf_temp |= bitmap_H[k];
                        }
                    }
                    destbuf[j] = buf_temp;
                }
            }
            PutBlockToPrintDotBuf(destbuf,1,(strprnprop.enheight+1));
            destbuf -= PrnDotOfByte;
        }
    }
    else
    {
        PutBlockToPrintDotBuf((uint8_t *)(P_STRLineBuf->LineDotBuf[MAX_LINE_DOT_BUF_HIGH-P_STRLineBuf->max_high]),P_STRLineBuf->max_high,(strprnprop.enheight+1));
    }
    os_memset(&P_STRLineBuf->LineDotBuf[0][0], 0, sizeof(P_STRLineBuf->LineDotBuf));
    /* ��ӡ�м�� */
	if (!bBitMapFlag)		
	{
    	PutBlankLineToPrintDotBuf(strprnprop.linespace);
	}
    /* ����ʼλ������ */
    strprnprop.current_row = strprnprop.left_space;
    P_STRLineBuf->max_high = 0;
	bBitMapFlag = 0;
}

/*
*��������: ���е��󻺳��������ӡ���󻺳���
*�������: buf:�л�����ָ�룬LineNum:���󻺳�������(һ��Ϊһ���㣬���忴��������߶�)
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
PutBlockToPrintDotBuf(uint8_t *buf, u16 LineNum, uint32_t zoom)
{
    uint16_t i=0,j;
    uint8_t destbuf[PrnDotOfByte];
	if( zoom==0 ) zoom = 1;
///////////////////////////////////////////////////////////////////////////////
    /* ��׼ģʽ */
#if (PAGE_MODE_EN)
    if (STANDERD_MODE == strprnprop.pagemode)
#endif
    {
        while(i < LineNum)
        {
            os_memset(destbuf, 0, sizeof(destbuf));
            strprnprop.str_buf.dest_buf = destbuf;
            strprnprop.str_buf.dest_buf_size = PrnDotOfByte;
            strprnprop.str_buf.src_buf = &buf[i*strprnprop.str_buf.src_buf_size];

            switch (strprnprop.LineModeM)
            {
                case flush_right:
                case flush_middle:
                    BufFillBufByBit(&strprnprop.str_buf);
                    break;
                case flush_left:
                default:
                    strprnprop.str_buf.dest_buf = strprnprop.str_buf.src_buf;
                    break;
            }
            /* �߶ȷŴ� */
            for (j=0; j<zoom; j++)
            {
                PrnPutOneLineToPrintBuf(strprnprop.str_buf.dest_buf);
            }
            /* ��ʼ��һ�и��� */
            i++;
        }
    }
///////////////////////////////////////////////////////////////////////////////
#if (PAGE_MODE_EN)
    /* ҳģʽ */
    else if (PAGE_MODE == strprnprop.pagemode)
    {
        for (i=0; i<LineNum; i++)
        {
            PagePutOneLine(PrnDotOfLine, buf);
        }
    }
#endif
}

/*
*��������: ��ӡ�հ���
*�������: line ����
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
PutBlackLine(uint32_t line)
{
	uint8_t buf[PrnDotOfByte];
    uint32_t i;
	os_memset(buf,0x00,sizeof(buf));
    for(i=0; i<line; i++)
    {
		PrnPutOneLineToPrintBuf(buf);
    }
}

///////////////////////////////////////////////////////////////////////////////
#ifdef MUL_LANG
// Ԥ�������
void Internationalcode(u8 Code)
{
    switch (Code)   // ����ȱʡ������
    {
        case 0: //PC437 ����
            strprnprop.codetbl = 0;
            break;
        case 1: //PC860 ��������
            strprnprop.codetbl = 3;
            break;
        case 2: //WPC1255(Israel) ϣ������
            strprnprop.codetbl = 32;
            break;
        case 3: //PC866(Cyrillic #2) ����˹��
            strprnprop.codetbl = 59;
            break;
        case 4: //Rumania ����������
            strprnprop.codetbl = 33;
            break;
        case 5: //Farsi ������
            strprnprop.codetbl = 10;
            break;
        case 6: //PC864(Arabic) ��������
            strprnprop.codetbl = 22;
            break;
        case 7: //Vietnam Խ����
            strprnprop.codetbl = 27;
            break;
        case 8: //PC857 ��������
            strprnprop.codetbl = 8;
            break;
        case 9: //(Spianish) ��������
            strprnprop.codetbl = 7;
            break;
        case 10: // PC863(Canada-French)
            strprnprop.codetbl = 4;
            break;
        case 11: //PC864(Arabic) ��������
            strprnprop.codetbl = 14;
            break;
        case 12: //���������� Armenia
            strprnprop.codetbl = 68;
            break;
        case 13: //PC865(Nordic)
            strprnprop.codetbl = 05;
            break;
        case 14://PC852(Latin 2)
            strprnprop.codetbl = 18;
            break;
        case 15: //WPC1254(Turkey)
            strprnprop.codetbl = 25;
            break;
        case 16: //ϣ�� CP737
            strprnprop.codetbl = 20;
            break;
        case 17: //PC850 �������
            strprnprop.codetbl = 02;
            break;
        default://ascii
            strprnprop.codetbl = 0;
            break;
    }
}
#endif

#ifndef MUL_LANG
//==============================================================================
/*
*��������: ��ȡ12x24��ascii����󵽻���
*�������: buf:����ָ�룬ch:�ַ�
*�������: bufָ��ָ��Ļ�������
*����ֵ  : ��
*/
//==============================================================================
void ReadCHINESE_24x24Data(uint8_t *buf,uint32_t ch)
{
	uint32_t hzaddr = 0;

	if (PrintFontType == FONT_JCHINESE) // ������
	{
		if (ch >= 1410) // �����ַ�(16�����ֿ�ʼ�ַ�) 0x582 = 15*94
		{
			hzaddr = (ch - 1410)*72 + USER_FONT_ADDR;
		}
		else
		{
			hzaddr = ch * 72 ;// ÿ������ռ��72 BYTE

		}
	}
    
	if ((PrintFontType == FONT_JCHINESE) && (ch/0x100 == 0xff)) // ������û��Զ�����
    {
        os_memset(buf, 0, 72);
    }
    else
    {
		ReadNByteFromSst(buf, 72, hzaddr);
    }
}

void ReadCHINESE_16x16Data(uint8_t *buf,uint32_t ch)
{
	uint32_t hzaddr = 0;
	if (PrintFontType == FONT_JCHINESE) // ������
	{
		hzaddr = (ch)*32 + 0x7EFB0 ;
	}
	else if (PrintFontType == FONT_FCHINESE)
	{
		hzaddr = ch * 32 + 0x10000 ; // ������
	}
	else if (PrintFontType == FONT_JAPANESE)
	{
		// ����
		hzaddr = 0;
	}
	ReadNByteFromSst(buf, 32, hzaddr);
}

#else
//===== �����ַ��� (0x00 - 0x7f)
static const uint8_t ItcharTbl[]=
{
    0x23, 0x24, 0x40, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x7B, 0x7C, 0x7D, 0x7E
};

static const uint8_t ItcharConverTbl[11][12]=
{

    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // U.S.A (��׼ASCII)
    0xFF, 0xFF, 0x03, 0x04, 0x07, 0x1A, 0xFF, 0xFF, 0x0D, 0x0F, 0x1E, 0x1C,   // France
    0xFF, 0XFF, 0x1A, 0X05, 0X12, 0X0B, 0XFF, 0XFF, 0X10, 0X08, 0X1F, 0X14,   // Germany
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // U.k
    0xFF, 0xFF, 0xFF, 0x06, 0x09, 0x0C, 0xFF, 0xFF, 0x11, 0x1D, 0x13, 0xFF,   // Denmark I
    0XFF, 0X19, 0X1B, 0X05, 0X12, 0X0C, 0X0B, 0X0D, 0X10, 0x08, 0x13, 0x1F,   // Sweden
    0xFF, 0XFF, 0XFF, 0X04, 0XFF, 0X0D, 0XFF, 0X0F, 0X03, 0X16, 0X1E, 0X15,   // Italy
    0x02, 0XFF, 0XFF, 0X18, 0X0A, 0X0E, 0XFF, 0XFF, 0X1C, 0X17, 0XFF, 0XFF,   // Spain
    0xFF, 0XFF, 0XFF, 0XFF, 0X01, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,   // Japan
    0xFF, 0X19, 0X1B, 0X06, 0X09, 0X0C, 0X0B, 0X0D, 0X11, 0X1D, 0X13, 0X1F,   // Norway
    0xFF, 0XFF, 0X1B, 0X06, 0X09, 0X0C, 0X0B, 0X0D, 0X11, 0X1D, 0X13, 0X1F    // Denmark II
};

#endif

/*
*��������: ��ȡ����ҳ�е��ֿ�
*�������: buf:������ָ�룬ch �ַ�
*�������: ��
*����ֵ  : ��
*/
void ICACHE_FLASH_ATTR
ReadASCII_12x24Data(uint8_t *buf,uint8_t ch)
{
	uint32_t addr;
#ifdef MUL_LANG
	if (ch < 0x80)
	{
		if ((strprnprop.charsset != 0) && (strprnprop.charsset < 11))	// ���÷������ַ�
		{
			u8 i ;
			for (i=0; i<12; i++)
			{
				if (ItcharTbl[i] == ch) break;
			}

			if (i != 12)  // ����������ַ�
			{
				if (ItcharConverTbl[strprnprop.charsset][i] != 0xff)
				{
					ch = ItcharConverTbl[strprnprop.charsset][i];
				}
			}
		}
		addr = 0x5000;
	}
	else
	{
		ch -= 0x80;
		switch(strprnprop.codetbl)
		{
			case 0: //���� code page 437
				addr = CP437Add;
				break;
			case 2: //�������
				addr = CP850Add;
				break;
			case 3: //��������	CP860
				addr = PortugalAdd;
				break;
			case 4: // ���ô�-����	 CP863
				addr = CP863Add;
				break;
			case 5: // Ų�� 	CP865
				addr = CP865Add;
				break;
			case 7: //��������	esc pos p:56
				addr = SpianishAdd;
				break;
			case 8:  // ������	 CP857
			case 61:  // ������   CP857
				addr = CP857Add;
				break;
			case 10: //����(��˹)��
				addr = IranAdd;
				break;
			case 14: //�������� copdepage864

			case 22: //�������� copdepage864
				addr = ArabiaAdd;
				break;
			case 18: // ������	 CP852
				addr = CP852Add;
				break;
			case 20: // ϣ�� CP737
				addr = CP737Add;
				break;
			case 25:  //
				addr = TurkeyAdd;  // code page 1254
				break;
			case 27:  //(Vietnam)
				addr = VietnamAdd;
				break;
			case 28:  //cp1251
				addr = WPC1251Add;
				break;
			case 68://	���������ֿ�
				addr = ArmeniaAdd;
				break;
			case 32://��ɫ��
				addr = IsraelAdd;
				break;
			case 33: //��������
				addr = RumaniaAdd;
				break;
			case 40: // ������CP1256
				addr = Arabia_1_Add;
				break;
			case 59://����	   code page 866
				addr = RussianAdd;
				break;
			default://ascii
				addr = CP437Add;
				break;
		}

	}
#else
	if (PrintFontType == FONT_JAPANESE)
	{
		if (ch < 0x80)
		{
			addr = 0x5000;
		}
		else
		{
			ch -= 0x80;
			addr = HalfJapanFontAddr;
		}
	}
	else
	{
		if (ch > 0x7f) ch = '?';
		addr = 0x5000;
	}
#endif
	ReadNByteFromSst(buf, 48, (ch*48 + addr));
}

void ICACHE_FLASH_ATTR
ReadASCII_9x17Data(uint8_t *buf,uint8_t ch)
{
#ifdef MUL_LANG
	uint32_t addr;

	if (ch < 0x80)
	{
		ch -= 0x20;
		addr = 0x3e00;
	}
	else
	{
		ch -= 0x80;
		switch(strprnprop.codetbl)
		{
			case 0: //���� code page 437
				addr = CP437_9x17Add;
				break;
			case 2: //�������
				addr = CP850_9x17Add;
				break;
			case 3: //��������	CP860
				addr = Portugal_9x17Add;
				break;
			case 4: // ���ô�-����	 CP863
				addr = CP863_9x17Add;
				break;
			case 5: // Ų�� 	CP865
				addr = CP865_9x17Add;
				break;
			case 7: //��������	esc pos p:56
				addr = Spianish_9x17Add;
				break;
			case 8:  // ������	 CP857
			case 61:  // ������   CP857
				addr = CP857_9x17Add;
				break;
			case 10: //����(��˹)��
				addr = Iran_9x17Add;
				break;
			case 14: //�������� copdepage864
			case 22: //�������� copdepage864
				addr = Arabia_9x17Add;
				break;
			case 18: // ������	 CP852
				addr = CP852_9x17Add;
				break;
			case 20: // ϣ�� CP737
				addr = CP737_9x17Add;
				break;
			case 25:  //
				addr = Turkey_9x17Add;	// code page 1254
				break;
			case 27:  //(Vietnam)
				addr = Vietnam_9x17Add;
				break;
			case 28:
				addr = WPC1251_9x17Add;
				break;
			case 68://	���������ֿ�
				addr = Armenia_9x17Add;
				break;
			case 32://��ɫ��
				addr = Israel_9x17Add;
				break;
			case 33: //��������
				addr = Rumania_9x17Add;
				break;
			case 40: //�������� copdepage1256
				addr = Arabia_1_9x17Add;
				break;
			case 59://����	   code page 866
				addr = Russian_9x17Add;
				break;
			default://ascii
				addr = CP437_9x17Add;
				break;
		}

	}
	ReadNByteFromSst(buf, 48, (ch*48 + addr));
#else
	os_memset(buf, 0, 14);
	ch -= 0x20;
	ReadNByteFromSst(buf+14, 48, (ch*48 + 0x3e00));
#endif


}


