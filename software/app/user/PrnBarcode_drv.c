#include "includes.h"

const u8 ClrHBit[8]= {0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00};
const u8 ClrLBit[8]= {0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};
const u8 SetHBit[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

// ===========================================================================
//                       �ж��Ƿ�������
//============================================================================
#define	_isDigit(dat)	((((dat) >= '0')&&((dat) <= '9'))? TRUE : FALSE)
#define strToUpper(ch)	( (ch>='a') && (ch<='z') ) ? (ch-'a'+'A') : (ch)

/*******************************************************************************
**                    ��һ���ַ���������Сд��ĸת��Ϊ��д��ĸ
*******************************************************************************/
static void ICACHE_FLASH_ATTR
StrToUpper(uint8_t* ptr_str)
{
    while (*ptr_str )
    {
        *ptr_str = strToUpper(*ptr_str);
        *ptr_str++;
    }
}

//=============================================================================
//                       ���ݷŴ� N ��   0 ���Ŵ�
//=============================================================================
u16 ICACHE_FLASH_ATTR
DataZoomN(u8  * destbuf, u8  * srcbuf, u16 dot,u8 zoom)
{
    u8  ch = 0,m = 0;
    u32 i = 0,j = 0;
    u8  k = 0;
    u16 bitnum;

	zoom += 1;			/* zoom��СΪ1 */
	os_memset(destbuf,0,PrnDotOfByte);
	if( zoom==1 )		/* Ϊ1��ʾ���Ŵ� */
	{
		os_memcpy(destbuf,srcbuf,dot);
		return dot;
	}
	bitnum = 0;

    for(i=0; i<dot; i++)
    {
        ch = srcbuf[i];
        for(j=0; j<8; j++)
        {
            m = (ch & (0x80>>j));
            for(k=0; k<zoom; k++)
            {
				if( bitnum>=PrnDotOfLine )	return ((bitnum+7)/8);
				if( m ) destbuf[bitnum/8] |= SetHBit[bitnum&(8-1)];
				bitnum++;
            }
        }
    }
    return (bitnum/8);
}
static u16 ICACHE_FLASH_ATTR
BarCopyToRealBuf(u8  * destbuf, u8  * srcbuf, u16 dot, u8 width)
{
    if(width)
    {
        dot = DataZoomN(destbuf,srcbuf,dot,width);
    }
    else
    {
        os_memcpy(destbuf,srcbuf,dot);
    }
    return dot;
}
// ===========================================================================
// ���� :
//              AL_FillBarByteBit
// ���� :
//              ����bufferָ��ĵ�ַ��ƫ��offset���������val����ʾ��8����
// ���� :
//              buffer : ������ַ()
//              offset : ƫ��������
//              val    : Ҫ�����8����
// ��� :
//              ��
// ���� :
//              ��
// ============================================================================
static void ICACHE_FLASH_ATTR
AL_FillBarByteBit(u8 *buffer,u32 offset,u8 val)
{
    u8  temp;
    u8  ch,j;
    u32  z;

	if( !val ) return ;
    temp = offset % 8;
    z =  offset / 8;
    if(temp)
    {
        // ǰ����bit
        ch = buffer[z];
        ch &= ClrLBit[7 - temp];
        j = (val >> temp);
        buffer[z] = ch | j;
        // �󼸸�bit
        ch = buffer[z + 1];
        ch &= ClrHBit[temp - 1];
        j = (val << (8 - temp));
        buffer[z + 1] = ch | j;
    }
    else
    {
        // ��������
        buffer[z] = val;
    }
}
//===================================================================================
//
//                ��������У���
//                 startcheck ����������ż����3����������3
//===================================================================================
static unsigned char ICACHE_FLASH_ATTR
UPC_Checksum(unsigned char *srcbarcode,unsigned char *destbarcode,unsigned char len,unsigned char startcheck)
{
    int sum,ch;
    int i;

    sum = 250;
    for (i=0; i<len; i++)
    {
        ch = *srcbarcode++;
        if (TRUE != _isDigit(ch))
        {
            /* EAN-A�����ȫΪ���� */
            return(FALSE);
        }
        ch &= 0x0f;
        *destbarcode++ = ch; /* ��ȡ��ַ���ٵ�ַ�Ӽӣ��ٰ�ch������һ��ȡ�ĵ�ַ */

        if(i & 0x01) /* ���� */
        {
            if(startcheck != 0)
            {
                ch *= 3;
            }
        }
        else /* ż�� */
        {
            if(startcheck == 0)
            {
                ch *= 3;
            }
        }
        sum -= ch;
    }
    sum %= 10;
    /* srcbarcode ����У���ֲ�����ַ�����λNULL*/
    *srcbarcode++ = '0' + sum;
    *srcbarcode = 0;
    /* destbarcode ����У���֣������0xFF��β*/
    *destbarcode++ = sum;
    *destbarcode = 0xff;
    return (TRUE);
}
//=============================================================================
//
//                           EAN-13��
//
//=============================================================================
static const u8 Bar13TypeABC[10][2] =
{
    {0xe4,0xb0},               //DB      1110 0100B       1011 0000B       // 0
    {0xcc,0x98},               //DB      1100 1100B       1001 1000B       // 1
    {0xd8,0xc8},               //DB      1101 1000B       1100 1000B       // 2
    {0x84,0xbc},               //DB      1000 0100B       1011 1100B       // 3
    {0xb8,0xc4},               //DB      1011 1000B       1100 0100B       // 4
    {0x9c,0x8c},               //DB      1001 1100B       1000 1100B       // 5
    {0xa0,0xf4},               //DB      1010 0000B       1111 0100B       // 6
    {0x88,0xdc},               //DB      1000 1000B       1101 1100B       // 7
    {0x90,0xec},               //DB      1001 0000B       1110 1100B       // 8
    {0xe8,0xd0}                //DB      1110 1000B       1101 0000B       // 9
};
static const u8 Code13Type[10][6] =
{
    {0,0,0,0,0,0},      // 0
    {0,0,1,0,1,1},      // 1
    {0,0,1,1,0,1},      // 2
    {0,0,1,1,1,0},      // 3
    {0,1,0,0,1,1},      // 4
    {0,1,1,0,0,1},      // 5
    {0,1,1,1,0,0},      // 6
    {0,1,0,1,0,1},      // 7
    {0,1,0,1,1,0},      // 8
    {0,1,1,0,1,0}       // 9
};
static u8 ICACHE_FLASH_ATTR
GenEAN13Checksum(u8  * srcbarcode,u8  * destbarcode)
{
    if(UPC_Checksum(srcbarcode,destbarcode,12,1))
        return (TRUE);
	return (FALSE);
}

static u16 ICACHE_FLASH_ATTR
BarcodeEndBit(u8 *tmpbuf,u16 offset,u8 endCode,u8 len,u8 lastCode,u8 lastBits)
{
	if( endCode )	AL_FillBarByteBit(tmpbuf,offset,endCode);         // 101 00000
    offset += len;
    if( lastCode )	AL_FillBarByteBit(tmpbuf,offset,lastCode);         // 10000 000
    offset += lastBits;                            // β������
    return offset;
}

static u16 ICACHE_FLASH_ATTR
Barcode13(u8  * barcode,u8  * buffer,u8 width)
{
	u8 type,i;
	u16 offset;
	u8 tmpbuf[PrnDotOfByte];

	type = *barcode++;
	os_memset(tmpbuf,0,PrnDotOfByte);
	offset = 0;
	if(type == 0x0a)   //��Ϊ0x0a��ΪUPC-A��
	{
		// UPC-A
		AL_FillBarByteBit(tmpbuf,offset,0x00);	   // 0000 0000
		offset += 4;								// 4�����׵�
	}
	else
	{
		// EAN-13
		AL_FillBarByteBit(tmpbuf,offset,0x00);	   // 00000 000
		offset += 5;								// 5�����׵�
		AL_FillBarByteBit(tmpbuf,offset,0x00);	   // 0000000 0
		offset += 7;								// ��һ��������7����
	}

	AL_FillBarByteBit(tmpbuf,offset,0xA0);		   // 101 00000
	offset += 3;									// ǰ����  //���������101
	//��������6���ֽڵ�����
	for(i=0 ; i<6 ; i++)
	{
		AL_FillBarByteBit(tmpbuf,offset,(Bar13TypeABC[*barcode++][Code13Type[type%10][i]] ^ 0xff) & 0xfe);
		offset += 7;
	}
	//����5��BIT���м���
	AL_FillBarByteBit(tmpbuf,offset,0x50);		   // 01010 000
	offset += 5;
	//����6���ֽڵ�����
	for(i=0 ; i<6 ; i++)
	{
		AL_FillBarByteBit(tmpbuf,offset,Bar13TypeABC[*barcode++][0] & 0xfe);
		offset += 7;
	}
	AL_FillBarByteBit(tmpbuf,offset,0xA0);		   // 101 00000
	offset += 3;
	AL_FillBarByteBit(tmpbuf,offset,0x00);		   // 10000 000
	offset += 5;								   // β������
	return(BarCopyToRealBuf(buffer,tmpbuf, offset, width));
}

static u8 ICACHE_FLASH_ATTR
GetBarFont5x7(u8 ch,u8 line)
{
    if((line < 1) || (line > BarFontHigh2))
    {
        return(0);
    }
    else
    {
		if (ch < 0x20)
		{
			ch |= 0x30;
		}
        return(CHAR5x7[ch-0x20][line - 1] << 1);
    }
}

// 0 EAN13 1 UPCA
static void ICACHE_FLASH_ATTR
HriFont13(u8  * barcode,u8  * buffer,u8 width,u8 mode)
{
	u8 line,i;
	u16 offset;
	u8 tmpbuf[PrnDotOfByte];

	for(line=0 ; line<(BarFontHigh+2) ; line++)
	{
		os_memset(tmpbuf,0,PrnDotOfByte);
		offset = 0;
		if(mode)
		{
			// UPC-A
			AL_FillBarByteBit(tmpbuf,offset,0x00);		// 0000 0000
			offset += 4;								// 4�����׵�
		}
		else
		{
			// EAN-13
			AL_FillBarByteBit(tmpbuf,offset,0x00);			 // 00000 000
			offset += 5;								// 5�����׵�
			AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[0],line));
			offset += 7;
		}
		AL_FillBarByteBit(tmpbuf,offset,0xA0);				 // 101 00000
		offset += 3;
		if(mode)
		{
			for(i=0 ; i<6 ; i++)
			{
				AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));
				offset += 7;
			}
		}
		else
		{
			for(i=1 ; i<7 ; i++)
			{
				AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));
				offset += 7;
			}
		}
		AL_FillBarByteBit(tmpbuf,offset,0x50);				 // 01010 000
		offset += 5;
		if(mode)
		{
			for(i=6 ; i<12 ; i++)
			{
				AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));
				offset += 7;
			}
		}
		else
		{
			for(i=7 ; i<13 ; i++)
			{
				AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));
				offset += 7;
			}
		}
		AL_FillBarByteBit(tmpbuf,offset,0xA0);				 // 101 00000
		offset += 3;
		AL_FillBarByteBit(tmpbuf,offset,0x00);				 // 10000 000
		offset += 5;										// β������
		BarCopyToRealBuf(buffer,tmpbuf,offset,width);
		buffer += (TOTALBIT / 8);
	}
}


static u16 ICACHE_FLASH_ATTR
FillEan13Dot(u8  * barcode,u8 * barbuf,u8 width)
{
    u16 result;
    u8 buf[PrnDotOfByte];

    if(GenEAN13Checksum(barcode,buf) == FALSE)   		/* ����13��У��� */
    {
        return(0);
    }
    result = Barcode13(buf,barbuf,width);
    HriFont13(barcode,barbuf+PrnDotOfByte,width,0);
    return(result);
}
//=============================================================================
//
//                           EAN-8��
//
//=============================================================================
static unsigned char ICACHE_FLASH_ATTR
GenEAN8Checksum(unsigned char *srcbarcode,unsigned char *destbarcode)
{
    if(UPC_Checksum(srcbarcode,destbarcode,7,0))
    {
        return (TRUE);
    }
	return (FALSE);
}
static unsigned int ICACHE_FLASH_ATTR
Barcode08(unsigned char *barcode,unsigned char *buffer,unsigned char width)
{
    unsigned int i;
    unsigned int offset;
    unsigned char tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 4;                                    /* 4�����׵� */
    AL_FillBarByteBit(tmpbuf,offset,0xA0);          /* 101 00000 */
    offset += 3;                                    /* ǰ���� */
    for(i=0 ; i<4 ; i++)
    {
        AL_FillBarByteBit(tmpbuf,offset,(Bar13TypeABC[*barcode++][Code13Type[0][i]] ^ 0xff) & 0xfe);
        offset += 7;
    }
    AL_FillBarByteBit(tmpbuf,offset,0x50);          /* 01010 000 */
    offset += 5;
    for(i=0 ; i<4 ; i++)
    {
        AL_FillBarByteBit(tmpbuf,offset,Bar13TypeABC[*barcode++][0] & 0xfe);
        offset += 7;
    }
	AL_FillBarByteBit(tmpbuf,offset,0xA0);          /* 101 00000 */
    offset += 3 + 5;
    return(BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width));
}
static unsigned int ICACHE_FLASH_ATTR
HriFont08(unsigned char *barcode,unsigned char *buffer,unsigned char width)
{
    unsigned int line,i;
    unsigned int offset;
    unsigned char tmpbuf[PrnDotOfByte];

    for(line=0 ; line<12 ; line++)
    {
        os_memset(tmpbuf,0,PrnDotOfByte);
        offset = 4;                            			/* 4�����׵� */
        AL_FillBarByteBit(tmpbuf,offset,0xA0);  		/* 101 00000 */
        offset += 3;                              		/* ǰ���� */
        for(i=0 ; i<4 ; i++)
        {
            AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));   // las083
            offset += 7;
        }
        AL_FillBarByteBit(tmpbuf,offset,0x50);    		/* 01010 000 */
        offset += 5;
        for(i=4 ; i<8 ; i++)
        {
            AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));   // las083
            offset += 7;
        }
		AL_FillBarByteBit(tmpbuf,offset,0xA0); 			/* 101 00000 */
        offset += 3 + 5;
        BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width);
        buffer += PrnDotOfByte;
    }
    return (offset);
}
static u16 ICACHE_FLASH_ATTR
FillEan08Dot(u8  * barcode,u8 * barbuf,u8 width)
{
    u16 result;
    u8 buf[PrnDotOfByte];
    /* ����13��У��� */
    if(GenEAN8Checksum(barcode,buf) == FALSE)
    {
        return(FALSE);
    }
    result = Barcode08(buf,barbuf,width);
    HriFont08(barcode,barbuf+PrnDotOfByte,width);
    return(result);
}
//=============================================================================
//
//                           UPC-E��
//
//=============================================================================
static u8 ICACHE_FLASH_ATTR
GenUPCA_EChecksum(u8 *srcbarcode, u8 *destbarcode, u8 barcodelen)
{
    u8 sbuf[16];

    os_memset(sbuf,0,sizeof(sbuf));
    if(UPC_Checksum(srcbarcode,&sbuf[1], barcodelen, 0))
    {
        sbuf[0] = 0x0a;
        os_memcpy(destbarcode,(u8 *)&sbuf[0],barcodelen + 2);
        return 2;
    }
    return FALSE;
}
//0�������żУ�� 1������У��
static const u8 CodeUpcEType[10][6] =
{
    {0,0,0,1,1,1},      // 0
    {0,0,1,0,1,1},      // 1
    {0,0,1,1,0,1},      // 2
    {0,0,1,1,1,0},      // 3
    {0,1,0,0,1,1},      // 4
    {0,1,1,0,0,1},      // 5
    {0,1,1,1,0,0},      // 6
    {0,1,0,1,0,1},      // 7
    {0,1,0,1,1,0},      // 8
    {0,1,1,0,1,0}       // 9
};
static const u8 BarUPCEType[10][2] =
{
    // ż         //   ��
    {0x4e,0x1a},               //DB     0100 1110B    0001 1010B              // 0
    {0x66,0x32},               //DB     0110 0110B    0011 0010B              // 1
    {0x36,0x26},               //DB     0011 0110B    0010 0110B              // 2
    {0x42,0x7a},               //DB     0100 0010B    0111 1010B              // 3
    {0x3a,0x46},               //DB     0011 1010B    0100 0110B              // 4
    {0x72,0x62},               //DB     0111 0010B    0110 0010B              // 5
    {0x0a,0x5e},               //DB     0000 1010B    0101 1110B              // 6
    {0x22,0x76},               //DB     0010 0010B    0111 0110B              // 7
    {0x12,0x6e},               //DB     0001 0010B    0110 1110B              // 8
    {0x2e,0x16}                //DB     0010 1110B    0001 0110B              // 9
};

static void ICACHE_FLASH_ATTR
HriUpcE(u8  * barcode,u8  * buffer,u8 width)
{
    u8 line,i;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    for(line=0 ; line<(BarFontHigh+2) ; line++)
    {
        os_memset(tmpbuf,0,PrnDotOfByte);
        offset = 7;                                            // 7�����׵�
        AL_FillBarByteBit(tmpbuf,offset,0xA0);                 // 101 00000
        offset += 3;
        for(i=0 ; i<6 ; i++)
        {
            AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));   // las083
            offset += 7;
        }
		AL_FillBarByteBit(tmpbuf,offset,0x54);                 // 0101 01 00
        offset += 6 + 2;
        BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width);
        buffer += PrnDotOfByte;
    }
}

static u16 ICACHE_FLASH_ATTR
BarcodeUpcE(u8 *barcode,  u8 *barbuf, u8 width)
{
    u8 type,i;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    type = barcode[6];        //�����
    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 7;                                      // ��һ��������7����
    AL_FillBarByteBit(tmpbuf,offset,0xA0);           // 101 00000
    offset += 3;                                     // ǰ����  //���������101
    for(i=0 ; i<6 ; i++)
    {
        AL_FillBarByteBit(tmpbuf,offset,  BarUPCEType[*barcode++][CodeUpcEType[type%10][i]] & 0xfe);
        offset += 7;
    }
	AL_FillBarByteBit(tmpbuf,offset,0x54);           // 0101 0100   �һ���
    offset += 6 + 2;
    return(BarCopyToRealBuf(barbuf,tmpbuf, (offset+7)/8, width));
}
static u16 ICACHE_FLASH_ATTR
FillCodeUpcEDot(u8 *barcode,  u8 *barbuf, u8 width)
{
	u16 result;

	result = BarcodeUpcE(barcode, barbuf, width);
	barcode[6] = 0;

	HriUpcE(barcode, barbuf+(TOTALBIT/8),width);
	return result;
}

//=============================================================================
//
//                           UPC-A��
//
//=============================================================================
static u16 ICACHE_FLASH_ATTR
FillUpcADot(u8  * barcode,u8 * barbuf,u8 width)
{
    u16 result;
    u8  buf[PrnDotOfByte];

    if(GenUPCA_EChecksum(barcode,buf,11) == FALSE)                  /* ����13��У��� */
    {
        return(0);
    }
    result = Barcode13(buf,barbuf,width);
    HriFont13(buf,barbuf+PrnDotOfByte,width,1);
    return result;
}

//=============================================================================
//
//                           ITF25��
//
//=============================================================================
static u8 ICACHE_FLASH_ATTR
GenITFChecksum(u8 * srcbarcode,u8 * destbarcode)
{
    u8 len,ch;

    for(len=os_strlen((char *)srcbarcode) ; len ; len--)
    {
        ch = *srcbarcode++;
        if((ch < '0') || (ch > '9'))
        {
            return(FALSE);
        }
        *destbarcode++ = ch & 0x0f;
    }
    *srcbarcode = 0;
    *destbarcode = 0xff;
    return(TRUE);
}

static const u8 ITF25BitTbl[] = {0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};

static void ITFBarFillDot(u8  * buf,u16 offset,u8 d1,u8 d2,u8 unit)
{
	u8 len,bitnum;

	len = unit >> 4;
	unit &= 0x0f;
	for( ; len ; len--)
	{
		bitnum = (d1 & 0x80) ? unit : 2;
		d1 <<= 1;
		AL_FillBarByteBit(buf,offset,ITF25BitTbl[bitnum]);
		offset += bitnum;

		bitnum = (d2 & 0x80) ? unit : 2;
		d2 <<= 1;
		AL_FillBarByteBit(buf,offset,0);
		offset += bitnum;
	}
}


static const u8 ITF25CodeTbl[] =
{
    0x30,       //DB      0011 0000B       // 0
    0x88,       //DB      1000 1000B       // 1
    0x48,       //DB      0100 1000B       // 2
    0xC0,       //DB      1100 0000B       // 3
    0x28,       //DB      0010 1000B       // 4
    0xA0,       //DB      1010 0000B       // 5
    0x60,       //DB      0110 0000B       // 6
    0x18,       //DB      0001 1000B       // 7
    0x90,       //DB      1001 0000B       // 8
    0x50,       //DB      0101 0000B       // 9
};

static u16 ICACHE_FLASH_ATTR
ITFBarcode(u8  * barcode,u8  * buffer,u8 unit,u8 width)
{
    u8 d1,d2;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 7;                             	// 7�����׵�
    d1 = 0x00;
    d2 = 0x00;
    ITFBarFillDot(tmpbuf,offset,d1,d2,unit | 0x20);
    offset += (4 * 2);
    while(*barcode != 0xff)
    {
        d1 = ITF25CodeTbl[*barcode++];
        d2 = ITF25CodeTbl[*barcode++];
        ITFBarFillDot(tmpbuf,offset,d1,d2,unit | 0x50);
        offset += ((6 * 2) + (4 * unit));
    }
    d1 = 0x80;
    d2 = 0x00;
    ITFBarFillDot(tmpbuf,offset,d1,d2,unit | 0x20);
    offset += (2 * 2) + 7 + unit;
    return(BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width));
}

static void HriITF(u8  *barcode,u8  *buffer,u16 dot,u8 width)
{
    u8 pos,line,i,len;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    if(width)
    {
        dot /= (width + 1);
    }
    len = os_strlen((char *)barcode);
    pos = len * 7;
    if(dot > pos)
    {
        pos = (dot - pos) / 2;
    }
    else
    {
        pos = 0;
    }
    for(line=0 ; line<(BarFontHigh+2) ; line++)
    {
        os_memset(tmpbuf,0,PrnDotOfByte);
        offset = pos;
        for(i=0 ; i<len ; i++)
        {
            AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));  // las083
            offset += 7;
        }
        BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width);
        buffer += PrnDotOfByte;
    }
}

static u16 ICACHE_FLASH_ATTR
FillITF25Dot(u8 * barcode,u8 * barbuf,u8 sm,u8 width)
{
    u16 dot;
    u8 buf[PrnDotOfByte];

    if(GenITFChecksum(barcode,buf) == FALSE)
    {
        return(0);
    }

	dot = ITFBarcode(buf,barbuf,(sm) ? 3 : 4,width);
    HriITF(barcode,barbuf+PrnDotOfByte,dot*8,width);
    return(dot);
}

//=============================================================================
//
//                           CODE128��
//
//=============================================================================
// ����Ƿ������� chklen ������
static u8 ICACHE_FLASH_ATTR
ChknDigit(u8 * dt,u8 remainlen,u8 chklen)
{
    if(remainlen < chklen)
    {
        return(FALSE);
    }
    for( ; chklen ; chklen--)
    {
        if(_isDigit(*dt))    // las083 todo
        {
            dt++;
        }
        else
        {
            return(FALSE);
        }
    }
    return(TRUE);
}

#define status_BToC     99
#define status_CToB     100
#define StartCode_B     104
#define StartCode_C     105
#define EndCode128      106
#define status_B        1
#define status_C        2

static u8 ICACHE_FLASH_ATTR
GetDigitData(u8 * dt)
{
    return(((*dt) & 0x0f) * 10 + ((*(dt + 1)) & 0x0f));
}

static u8 ICACHE_FLASH_ATTR
GetCharData(u8 * dt)
{
    u8 tmp;

    tmp = *dt;
    if((tmp < 0x20) || (tmp > 0x7E))
    {
        tmp = 0x20;
    }
    tmp -= 0x20;
    return(tmp);
}

static u8 FillStartEndCode(u8  * in,u8  * out)
{
    u8 remainlen,CurStatus,totlen;
    u16 chksum,tmpsum;
    u8 tmpout;
    u8  * tmpin;
    u8  * bakpt;

    remainlen = os_strlen((char *)in);
    if(remainlen == 0)
    {
        return(0);
    }
    tmpin = (u8  *)in;
    bakpt = out;
    if(ChknDigit(tmpin,remainlen,2))
    {
        // �����������֣�ʹ��ģʽC
        CurStatus = status_C;
        *out = StartCode_C;
    }
    else
    {
        // û�������������֣�ʹ��ģʽB
        CurStatus = status_B;
        *out = StartCode_B;
    }
    out++;
    totlen = 1;
    while(remainlen)
    {
        if(CurStatus == status_C)
        {
            // ��ǰģʽΪģʽC
            if(ChknDigit(tmpin,remainlen,2))
            {
                // �����������֣�������ģʽ
                tmpout = GetDigitData(tmpin);
                tmpin++;
                remainlen--;
            }
            else
            {
                // û�������������֣��л���ģʽB
                tmpout = GetCharData(tmpin);
                *out++ = status_CToB;
                totlen++;
                CurStatus = status_B;
            }
        }
        else
        {
            // ��ǰģʽΪģʽB
            if(ChknDigit(tmpin,remainlen,4))
            {
                // ����4�����֣��л���ģʽC
                tmpout = GetDigitData(tmpin);
                *out++ = status_BToC;
                totlen++;
                CurStatus = status_C;
                tmpin++;
                remainlen--;
            }
            else
            {
                // û������4�����֣�������ģʽ
                tmpout = GetCharData(tmpin);
            }
        }
        *out++ = tmpout;
        totlen++;
        tmpin++;
        remainlen--;
    }
//-------����У���
    chksum = *bakpt;
    bakpt++;
    for(remainlen=1 ; remainlen<totlen ; remainlen++)
    {
        tmpsum = *bakpt;
        bakpt++;
        chksum += (remainlen * tmpsum);
    }
    tmpout = chksum % 103;
    *out++ = tmpout;
    totlen++;
    *out = EndCode128;
    totlen++;
    return(totlen);
}

static const u8 Code128BitTbl[][2] =
{
    {0xD9,0x80},                //DW      1101  1001  100 0  0 0 0 0B   // ' ' 20   0
    {0xCD,0x80},                //DW      1100  1101  100 0  0 0 0 0B   // '!' 21   1
    {0xCC,0xC0},                //DW      1100  1100  110 0  0 0 0 0B   // '"' 22   2
    {0x93,0x00},                //DW      1001  0011  000 0  0 0 0 0B   // '#' 23   3
    {0x91,0x80},                //DW      1001  0001  100 0  0 0 0 0B   // '$' 24   4
    {0x89,0x80},                //DW      1000  1001  100 0  0 0 0 0B   // '%' 25   5
    {0x99,0x00},                //DW      1001  1001  000 0  0 0 0 0B   // '&' 26   6
    {0x98,0x80},                //DW      1001  1000  100 0  0 0 0 0B   // ''' 27   7
    {0x8C,0x80},                //DW      1000  1100  100 0  0 0 0 0B   // '(' 28   8
    {0xC9,0x00},                //DW      1100  1001  000 0  0 0 0 0B   // ')' 29   9
    {0xC8,0x80},                //DW      1100  1000  100 0  0 0 0 0B   // '*' 2A  10
    {0xC4,0x80},                //DW      1100  0100  100 0  0 0 0 0B   // '+' 2B  11
    {0xB3,0x80},                //DW      1011  0011  100 0  0 0 0 0B   // ',' 2C  12
    {0x9B,0x80},                //DW      1001  1011  100 0  0 0 0 0B   // '-' 2D  13
    {0x99,0xC0},                //DW      1001  1001  110 0  0 0 0 0B   // '.' 2E  14
    {0xB9,0x80},                //DW      1011  1001  100 0  0 0 0 0B   // '/' 2F  15
    {0x9D,0x80},                //DW      1001  1101  100 0  0 0 0 0B   // '0' 30  16
    {0x9C,0xC0},                //DW      1001  1100  110 0  0 0 0 0B   // '1' 31  17
    {0xCE,0x40},                //DW      1100  1110  010 0  0 0 0 0B   // '2' 32  18
    {0xCB,0x80},                //DW      1100  1011  100 0  0 0 0 0B   // '3' 33  19
    {0xC9,0xC0},                //DW      1100  1001  110 0  0 0 0 0B   // '4' 34  20
    {0xDC,0x80},                //DW      1101  1100  100 0  0 0 0 0B   // '5' 35  21
    {0xCE,0x80},                //DW      1100  1110  100 0  0 0 0 0B   // '6' 36  22
    {0xED,0xC0},                //DW      1110  1101  110 0  0 0 0 0B   // '7' 37  23
    {0xE9,0x80},                //DW      1110  1001  100 0  0 0 0 0B   // '8' 38  24
    {0xE5,0x80},                //DW      1110  0101  100 0  0 0 0 0B   // '9' 39  25
    {0xE4,0xC0},                //DW      1110  0100  110 0  0 0 0 0B   // ':' 3A  26
    {0xEC,0x80},                //DW      1110  1100  100 0  0 0 0 0B   // ';' 3B  27
    {0xE6,0x80},                //DW      1110  0110  100 0  0 0 0 0B   // '<' 3C  28
    {0xE6,0x40},                //DW      1110  0110  010 0  0 0 0 0B   // '=' 3D  29
    {0xDB,0x00},                //DW      1101  1011  000 0  0 0 0 0B   // '>' 3E  30
    {0xD8,0xC0},                //DW      1101  1000  110 0  0 0 0 0B   // '?' 3F  31
    {0xC6,0xC0},                //DW      1100  0110  110 0  0 0 0 0B   // '@' 40  32
    {0xA3,0x00},                //DW      1010  0011  000 0  0 0 0 0B   // 'A' 41  33
    {0x8B,0x00},                //DW      1000  1011  000 0  0 0 0 0B   // 'B' 42  34
    {0x88,0xC0},                //DW      1000  1000  110 0  0 0 0 0B   // 'C' 43  35
    {0xB1,0x00},                //DW      1011  0001  000 0  0 0 0 0B   // 'D' 44  36
    {0x8D,0x00},                //DW      1000  1101  000 0  0 0 0 0B   // 'E' 45  37
    {0x8C,0x40},                //DW      1000  1100  010 0  0 0 0 0B   // 'F' 46  38
    {0xD1,0x00},                //DW      1101  0001  000 0  0 0 0 0B   // 'G' 47  39
    {0xC5,0x00},                //DW      1100  0101  000 0  0 0 0 0B   // 'H' 48  40
    {0xC4,0x40},                //DW      1100  0100  010 0  0 0 0 0B   // 'I' 49  41
    {0xB7,0x00},                //DW      1011  0111  000 0  0 0 0 0B   // 'J' 4A  42
    {0xB1,0xC0},                //DW      1011  0001  110 0  0 0 0 0B   // 'K' 4B  43
    {0x8D,0xC0},                //DW      1000  1101  110 0  0 0 0 0B   // 'L' 4C  44
    {0xBB,0x00},                //DW      1011  1011  000 0  0 0 0 0B   // 'M' 4D  45
    {0xB8,0xC0},                //DW      1011  1000  110 0  0 0 0 0B   // 'N' 4E  46
    {0x8E,0xC0},                //DW      1000  1110  110 0  0 0 0 0B   // 'O' 4F  47
    {0xEE,0xC0},                //DW      1110  1110  110 0  0 0 0 0B   // 'P' 50  48
    {0xD1,0xC0},                //DW      1101  0001  110 0  0 0 0 0B   // 'Q' 51  49
    {0xC5,0xC0},                //DW      1100  0101  110 0  0 0 0 0B   // 'R' 52  50
    {0xDD,0x00},                //DW      1101  1101  000 0  0 0 0 0B   // 'S' 53  51
    {0xDC,0x40},                //DW      1101  1100  010 0  0 0 0 0B   // 'T' 54  52
    {0xDD,0xC0},                //DW      1101  1101  110 0  0 0 0 0B   // 'U' 55  53
    {0xEB,0x00},                //DW      1110  1011  000 0  0 0 0 0B   // 'V' 56  54
    {0xE8,0xC0},                //DW      1110  1000  110 0  0 0 0 0B   // 'W' 57  55
    {0xE2,0xC0},                //DW      1110  0010  110 0  0 0 0 0B   // 'X' 58  56
    {0xED,0x00},                //DW      1110  1101  000 0  0 0 0 0B   // 'Y' 59  57
    {0xEC,0x40},                //DW      1110  1100  010 0  0 0 0 0B   // 'Z' 5A  58
    {0xE3,0x40},                //DW      1110  0011  010 0  0 0 0 0B   // '[' 5B  59
    {0xEF,0x40},                //DW      1110  1111  010 0  0 0 0 0B   // '\' 5C  60
    {0xC8,0x40},                //DW      1100  1000  010 0  0 0 0 0B   // ']' 5D  61
    {0xF1,0x40},                //DW      1111  0001  010 0  0 0 0 0B   // '^' 5E  62
    {0xA6,0x00},                //DW      1010  0110  000 0  0 0 0 0B   // '_' 5F  63
    {0xA1,0x80},                //DW      1010  0001  100 0  0 0 0 0B   // ''' 60  64
    {0x96,0x00},                //DW      1001  0110  000 0  0 0 0 0B   // 'a' 61  65
    {0x90,0xC0},                //DW      1001  0000  110 0  0 0 0 0B   // 'b' 62  66
    {0x85,0x80},                //DW      1000  0101  100 0  0 0 0 0B   // 'c' 63  67
    {0x84,0xC0},                //DW      1000  0100  110 0  0 0 0 0B   // 'd' 64  68
    {0xB2,0x00},                //DW      1011  0010  000 0  0 0 0 0B   // 'e' 65  69
    {0xB0,0x80},                //DW      1011  0000  100 0  0 0 0 0B   // 'f' 66  70
    {0x9A,0x00},                //DW      1001  1010  000 0  0 0 0 0B   // 'g' 67  71
    {0x98,0x40},                //DW      1001  1000  010 0  0 0 0 0B   // 'h' 68  72
    {0x86,0x80},                //DW      1000  0110  100 0  0 0 0 0B   // 'i' 69  73
    {0x86,0x40},                //DW      1000  0110  010 0  0 0 0 0B   // 'j' 6a  74
    {0xC2,0x40},                //DW      1100  0010  010 0  0 0 0 0B   // 'k' 6b  75
    {0xCA,0x00},                //DW      1100  1010  000 0  0 0 0 0B   // 'l' 6c  76
    {0xF7,0x40},                //DW      1111  0111  010 0  0 0 0 0B   // 'm' 6d  77
    {0xC2,0x80},                //DW      1100  0010  100 0  0 0 0 0B   // 'n' 6e  78
    {0x8F,0x40},                //DW      1000  1111  010 0  0 0 0 0B   // 'o' 6f  79
    {0xA7,0x80},                //DW      1010  0111  100 0  0 0 0 0B   // 'p' 70  80
    {0x97,0x80},                //DW      1001  0111  100 0  0 0 0 0B   // 'q' 71  81
    {0x93,0xC0},                //DW      1001  0011  110 0  0 0 0 0B   // 'r' 72  82
    {0xBC,0x80},                //DW      1011  1100  100 0  0 0 0 0B   // 's' 73  83
    {0x9E,0x80},                //DW      1001  1110  100 0  0 0 0 0B   // 't' 74  84
    {0x9E,0x40},                //DW      1001  1110  010 0  0 0 0 0B   // 'u' 75  85
    {0xF4,0x80},                //DW      1111  0100  100 0  0 0 0 0B   // 'v' 76  86
    {0xF2,0x80},                //DW      1111  0010  100 0  0 0 0 0B   // 'w' 77  87
    {0xF2,0x40},                //DW      1111  0010  010 0  0 0 0 0B   // 'x' 78  88
    {0xDB,0xC0},                //DW      1101  1011  110 0  0 0 0 0B   // 'y' 79  89
    {0xDE,0xC0},                //DW      1101  1110  110 0  0 0 0 0B   // 'z' 7a  90
    {0xF6,0xC0},                //DW      1111  0110  110 0  0 0 0 0B   // '{' 7b  91
    {0xAF,0x00},                //DW      1010  1111  000 0  0 0 0 0B   // '|' 7c  92
    {0xA3,0xC0},                //DW      1010  0011  110 0  0 0 0 0B   // '}' 7d  93
    {0x8B,0xC0},                //DW      1000  1011  110 0  0 0 0 0B   // '~' 7e  94
    {0xBD,0x00},                //DW      1011  1101  000 0  0 0 0 0B   // ' ' 7f  95
    {0xBC,0x40},                //DW      1011  1100  010 0  0 0 0 0B   //     80  96   FUC3
    {0xF5,0x00},                //DW      1111  0101  000 0  0 0 0 0B   //     81  97   FUC2
    {0xF4,0x40},                //DW      1111  0100  010 0  0 0 0 0B   //     82  98   SHIFT
    {0xBB,0xC0},                //DW      1011  1011  110 0  0 0 0 0B   //     83  99   CODEC
    {0xBD,0xC0},                //DW      1011  1101  110 0  0 0 0 0B   //     84 100   CODEB
    {0xEB,0xC0},                //DW      1110  1011  110 0  0 0 0 0B   //     85 101   FUN4
    {0xF5,0xC0},                //DW      1111  0101  110 0  0 0 0 0B   //     86 102   FUN1
    {0xD0,0x80},                //DW      1101  0000  100 0  0 0 0 0B   //     87 103   START CODE A
    {0xD2,0x00},                //DW      1101  0010  000 0  0 0 0 0B   //     88 104   START CODE B
    {0xD3,0x80},                //DW      1101  0011  100 0  0 0 0 0B   //     89 105   START CODE C
    {0xC7,0x40}                 //DW      1100  0111  010 0  0 0 0 0B   //     8A 106   END CODE
};
static void ICACHE_FLASH_ATTR
HriCode5x7(u8  *barcode,u8  *buffer,u16 dot,u8 width)
{
    u8 pos,line,i,len;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    if(width)
    {
        dot /= (width + 1);
    }
    len = os_strlen((char *)barcode);
    pos = len * 7;
    if(dot > pos)
    {
        pos = (dot - pos) / 2;
    }
    else
    {
        pos = 0;
    }
    for(line=0 ; line<(BarFontHigh2+2) ; line++)
    {
        os_memset(tmpbuf,0,PrnDotOfByte);
        offset = pos;
        for(i=0 ; i<len ; i++)
        {
            AL_FillBarByteBit(tmpbuf,offset,GetBarFont5x7(barcode[i],line));
            offset += 7;
        }
        BarCopyToRealBuf(buffer,tmpbuf,(offset+7)/8,width);
        buffer += PrnDotOfByte;
    }
}

static u16 ICACHE_FLASH_ATTR
Cod128DotFun(u8 * in,u8  * out,u8 len,u8 width)
{
    u8 tmp;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 4;                    /* ������� */
    for( ; len ; len--)
    {
        tmp = *in++;
		offset = BarcodeEndBit(tmpbuf,offset,Code128BitTbl[tmp][0],8,Code128BitTbl[tmp][1],3);
    }
	AL_FillBarByteBit(tmpbuf,offset,0xC0);
    offset += 2 + 4;           		/* �ұ߽��� */
    return(BarCopyToRealBuf(out,tmpbuf,(offset+7)/8,width));
}

extern u16 ICACHE_FLASH_ATTR
FillCode128Dot(u8 * barcode,u8  * barbuf,u8 width)
{
    u8 len;
    u16 dot;
    u8 tmpin[PrnDotOfByte];

    len = FillStartEndCode(barcode,tmpin);
    if(len == 0)
    {
        return(0);
    }
    dot = Cod128DotFun(tmpin,barbuf,len,width);
    HriCode5x7(barcode,barbuf+PrnDotOfByte,dot*8,width);
    return(dot);
}
//=============================================================================
//
//                           CODE39��
//
//=============================================================================
//����14BIT
static const u8 Code39BitTbl[][2] =
{
    {0xA6,0XD0},  // 10100110 1101    0
    {0xD2,0XB0},  // 11010010 1011    1
    {0xB2,0XB0},  // 10110010 1011    2
    {0xD9,0X50},  // 11011001 0101    3
    {0xA6,0XB0},  // 10100110 1011    4
    {0xD3,0X50},  // 11010011 0101    5
    {0xB3,0X50},  // 10110011 0101    6
    {0xA5,0XB0},  // 10100101 1011    7
    {0xD2,0XD0},  // 11010010 1101    8
    {0xB2,0XD0},  // 10110010 1101    9

    {0xD4,0XB0},  // 11010100 1011    A
    {0xB4,0XB0},  // 10110100 1011    B
    {0xDA,0X50},  // 11011010 0101    C
    {0xAC,0XB0},  // 10101100 1011    D
    {0xD6,0X50},  // 11010110 0101    E
    {0xB6,0X50},  // 10110110 0101    F
    {0xA9,0XB0},  // 10101001 1011    G
    {0xD4,0XD0},  // 11010100 1101    H
    {0xB4,0XD0},  // 10110100 1101    I
    {0xAC,0XD0},  // 10101100 1101    J
    {0xD5,0X30},  // 11010101 0011     K
    {0xB5,0X30},  // 10110101 0011     L
    {0xDA,0X90},  // 11011010 1001     M
    {0xAD,0X30},  // 10101101 0011     N
    {0xD6,0X90},  // 11010110 1001     O
    {0xB6,0X90},  // 10110110 1001     P
    {0xAB,0X30},  // 10101011 0011     Q
    {0xD5,0X90},  // 11010101 1001     R
    {0xB5,0X90},  // 10110101 1001     S
    {0xAD,0X90},  // 10101101 1001     T
    {0xCA,0XB0},  // 11001010 1011     U
    {0x9A,0XB0},  // 10011010 1011     V
    {0xCD,0X50},  // 11001101 0101     W
    {0x96,0XB0},  // 10010110 1011     X
    {0xCB,0X50},  // 11001011 0101     Y
    {0x9B,0X50},  // 10011011 0101     Z

    {0x95,0XB0},  // 10010101 1011     -
    {0xCA,0XD0},  // 11001010 1101     .
    {0x9A,0XD0},  // 10011010 1101
    {0x92,0X50},  // 10010010 0101     $
    {0x92,0X90},  // 10010010 1001     /
    {0x94,0X90},  // 10010100 1001     +
    {0xA4,0X90},  // 10100100 1001     %
    {0x96,0XD0},  // 10010110 1101     *
};
static u16 ICACHE_FLASH_ATTR
Cod39DotFun(u8 * in,u8  * out,u8 len,u8 doublewidth)
{
    u8 tmp;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 4;                    // �������
    for( ; len ; len--)
    {
        tmp = *in++;
		offset = BarcodeEndBit(tmpbuf,offset,Code39BitTbl[tmp][0],8,Code39BitTbl[tmp][1],5);
    }
    offset += 4;                    // �ұ�����
    return(BarCopyToRealBuf(out,tmpbuf,(offset+7)/8,doublewidth));
}

//39���
const u8 CODE39TAB[] =
{
//        '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%',0
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'A', 'B','C','D','E','F','G','H', 'I','J','K','L','M','N','O', 'P','Q','R','S','T','U','V', 'W','X','Y','Z',
    '-',
    '.',
    ' ',
    '$',
    '/',
    '+',
    '%',
};

static u16 ICACHE_FLASH_ATTR
FillStart39EndCode(u8 *barcode, u8 *output)
{
    u16 len = 0;
    u16 len1;
    u16 i;

    len = os_strlen((const char *)barcode);

    *output++ = ('*' + 1);
    for(len1=0; len1<len; len1++)
    {
        for(i=0; i<43; i++)
        {
            if(CODE39TAB[i] ==  barcode[len1])
            {
                *output++ = i;
                break;
            }
        }
        if(i == 43)
        {
            return (0);   // �Ҳ����ַ�
        }
    }
    *output++ = ('*' + 1);

    return(len+2);
}
extern u16 ICACHE_FLASH_ATTR
FillCode39Dot(u8 *barcode, u8 *barbuf, u8 width)
{
    u8 len;
    u16 dot;
    u8 tmpin[PrnDotOfByte];

    len = FillStart39EndCode(barcode,tmpin);
    if(len == 0)
    {
        return(0);
    }
    dot = Cod39DotFun(tmpin,barbuf,len,width);
    HriCode5x7(barcode,barbuf+PrnDotOfByte,dot*8,width);
    return(dot);
}
//=============================================================================
//
//                           Codebar��
//
//=============================================================================
#define CODE_BAR_A  16
static const u8 CODECodebarTAB[] =
{
//       DB      '0123456789-.$/+:ABCD',0
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '-',
    '$',
    '.',
    '/',
    '+',
    ':',
    'A', 'B','C','D'
};

static u16 ICACHE_FLASH_ATTR
FillStartCodeBarEndCode(u8 *barcode, u8 *output)
{
	u16 len1 = 0;
	u16 i;
	u16 relen = 0;
	u16 len = 0;
	len = os_strlen((char *)barcode);

	switch (barcode[0])  // ���ʼ��
	{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
			len1 = 1;
			* output = (barcode[0] - 'A') + CODE_BAR_A;
			if (len == 0) return (0); //??????? ���������� ??????????
			break;

			default:
				* output = CODE_BAR_A;	 // Ĭ�� A
				break;

	}

	output ++;
	relen ++;

	for(; len1<len; len1++)
	{
		for(i=0; i<20;i++)
		{
			if(CODECodebarTAB[i] ==  barcode[len1])
			{
				*output++ = i;
				relen++;
				break;
			}
		}

		if (i == 20) //û���ҵ���Ӧ���ַ�
		{
			return (0); //??????? ���������� ??????????
		}
	}

	switch (barcode[len-1]) // ����������
	{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
			break;

			default:
				*output++ = CODE_BAR_A;  // Ĭ�� A
				relen++;
				break;

	}
	*output++ = 0xff;
	return(relen);
}


//����9BIT(11)
// 0-9 ���Ϊ9BIT
//       DB      '0123456789-.$/+:ABCD',0

static const u8 CodeCodebarBitTbl[][2] =
{
    {0xA9,0X80},  // 10101001 1       0
    {0xAC,0X80},  // 10101100 1       1
    {0xA5,0X80},  // 10100101 1       2
    {0xCA,0X80},  // 11001010 1       3
    {0xB4,0X80},  // 10110100 1       4
    {0xD4,0X80},  // 11010100 1       5
    {0x95,0X80},  // 10010101 1       6
    {0x96,0X80},  // 10010110 1       7
    {0x9A,0X80},  // 10011010 1       8
    {0xD2,0X80},  // 11010010 1       9
    {0xA6,0X80},  // 10100110 1       -
    {0xB2,0X80},  // 10110010 1       $
    {0xDB,0X40},  // 11011011 01      .
    {0xDA,0XC0},  // 11011010 11      /
    {0xB6,0XC0},  // 10110110 11      +
    {0xD6,0XC0},  // 11010110 11      :

    {0xB2,0X40},  // 10110010 01      A
    {0x92,0XC0},  // 10010010 11      B
    {0xA4,0XC0},  // 10100100 11      C
    {0xA6,0X40},  // 10100110 01      D
};
static u16 ICACHE_FLASH_ATTR
CodCodeBarDotFun(u8 * in,u8  * out,u8 len,u8 width)
{
    u8 tmp;
    u16 offset;
    u8 tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0,PrnDotOfByte);
    offset = 4;                    // �������
    for( ; len ; len--)
    {
        tmp = *in++;
		offset = BarcodeEndBit(tmpbuf,offset,CodeCodebarBitTbl[tmp][0],8,CodeCodebarBitTbl[tmp][1],(tmp<=11)?2:3);
    }
    offset += 4;                    // �ұ�����
    /* ����Ŵ� */
    return(BarCopyToRealBuf(out,tmpbuf,(offset+7)/8,width));
}
//��ӡCodebar��
u16 ICACHE_FLASH_ATTR
FillCodeCodebarDot(u8 *barcode, u8 *barbuf, u8 width)
{
    u8 len;
    u16 dot;
    u8 tmpin[PrnDotOfByte];
    /* ���ַ������������Сд��ĸת���ɴ�д */
    StrToUpper(barcode);

    len = FillStartCodeBarEndCode(barcode,tmpin);
    if(len == 0)
    {
        return(0);
    }
    dot = CodCodeBarDotFun(tmpin,barbuf,len,width);
    HriCode5x7(barcode,barbuf+PrnDotOfByte,dot*8,width);
    return(dot);
}
//=============================================================================
//
//                           Code 93��
//
//=============================================================================
static const u8 CODE93TBL[] =
{
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%"
};
static const u8 SHIFT93SET1[] =
{
    ",++++++++++++++++++++++++++,,,,, ---  ----- -             -,,,,,,                          ,,,,,,..........................,,,,,"
};
static const u8 SHIFT93SET2[] =
{
    "UABCDEFGHIJKLMNOPQRSTUVWXYZABCDE ABC  FGHIJ L             ZFGHIJV                          KLMNOWABCDEFGHIJKLMNOPQRSTUVWXYZPQRST"
};
//93���У��
static unsigned char ICACHE_FLASH_ATTR
Gen93Checksum(unsigned char *srcbarcode, unsigned char *destbarcode)
{
    u16 i,j,len;
    u8 value;
    u8 count = 0;  //����
    u16 c_cksum, k_cksum;
//    u8 h,l;

    len = os_strlen((const char *)srcbarcode);
    value = *srcbarcode;
    for (i=0; i<len; i++)
    {
        /* ���뷶Χ�ж� */
        if (*srcbarcode >= 0x80)
        {
            *srcbarcode= 0xff;
            destbarcode[count] = 0xff;
            return 0xff;
        }
        /* ȡ��һ����Ԫ */
        value = *srcbarcode;
        /* ָ���һ */
        srcbarcode++;
        /* �ó���Ԫ��Ӧ��valueֵ */
        for (j=0; j<43; j++)
        {
            if(value == CODE93TBL[j])
            {
                destbarcode[count] = j;
                break;
            }
        }
        /* �����Ӧ����Ԫ����43 */
        if(j == 43)
        {
            destbarcode[count] = SHIFT93SET1[value];
            value = SHIFT93SET2[value];
            for (j=0; j<43; j++)
            {
                if(value == CODE93TBL[j])
                {
                    destbarcode[count] = j;
                    break;
                }
            }
        }
        count++;
    }
    *srcbarcode = 0;
    c_cksum = 0;
    k_cksum = 0;
    /* �����ۼӺ� */
    while(count)
    {
        c_cksum += (*destbarcode) * count;
        k_cksum += (*destbarcode) * (count+1);
        destbarcode++;
        count--;
    }

    //����c_cksum  k_chsum
    /* ����c_checksum */

    *destbarcode = c_cksum%47;
    /* k_cksum����c_cksum */
    k_cksum += *destbarcode;
    /* ָ������ */
    destbarcode++;
    /* ����k_checksum */

    *destbarcode = k_cksum%47;
    destbarcode++;
    *destbarcode = 0xff;
    return CODE93BARCODE;
    //ȫ���Ѿ�ת�����,��У���
}

static const u8 FILL93CHAR_TAB[] =
{
    0x8a,//  DB      10001010B   // 0    0
    0xA4,//  DB      10100100B   // 1    1
    0XA2,//  DB      10100010B   // 2    2
    0XA1,//  DB      10100001B   // 3    3
    0X94,//  DB      10010100B   // 4    4
    0X92,//  DB      10010010B   // 5    5
    0X91,//  DB      10010001B   // 6    6
    0XA8,//  DB      10101000B   // 7    7
    0X89,//  DB      10001001B   // 8    8
    0X85,//  DB      10000101B   // 9    9
    0XD4,//  DB      11010100B   // A   10
    0XD2,//  DB      11010010B   // B   11
    0XD1,//  DB      11010001B   // C   12
    0XCA,//  DB      11001010B   // D   13
    0XC9,//  DB      11001001B   // E   14
    0XC5,//  DB      11000101B   // F   15
    0XB4,//  DB      10110100B   // G   16
    0XB2,//  DB      10110010B   // H   17
    0XB1,//  DB      10110001B   // I   18
    0X9A,//  DB      10011010B   // J   19
    0X8D,//  DB      10001101B   // K   20
    0XAC,//  DB      10101100B   // L   21
    0XA6,//  DB      10100110B   // M   22
    0XA3,//  DB      10100011B   // N   23
    0X96,//  DB      10010110B   // O   24
    0X8B,//  DB      10001011B   // P   25
    0XDA,//  DB      11011010B   // Q   26
    0XD9,//  DB      11011001B   // R   27
    0XD6,//  DB      11010110B   // S   28
    0XD3,//  DB      11010011B   // T   29
    0XCB,//  DB      11001011B   // U   30
    0XCD,//  DB      11001101B   // V   31
    0XB6,//  DB      10110110B   // W   32
    0XB3,//  DB      10110011B   // X   33
    0X9B,//  DB      10011011B   // Y   34
    0X9D,//  DB      10011101B   // Z   35
    0X97,//  DB      10010111B   // -   36
    0XEA,//  DB      11101010B   // .   37
    0XE9,//  DB      11101001B   //     38
    0XE5,//  DB      11100101B   // $   39
    0XB7,//  DB      10110111B   // /   40
    0XBB,//  DB      10111011B   // +   41
    0XD7,//  DB      11010111B   // %   42
    0X93,//  DB      10010011B   //^$   43   ��չ$
    0XED,//  DB      11101101B   //^%   44   ��չ%
    0XEB,//  DB      11101011B   //^/   45   ��չ/
    0X99,//  DB      10011001B   //^+   46   ��չ+
    0XAF,//  DB      10101111B   //     47   ������
};
static  u16 ICACHE_FLASH_ATTR
CODE93Barcode(u8 *barcode, u8 *barbuf, u8 width )
{
    u16 offset = 0;
    u8 tmpbuf[PrnDotOfByte];

    os_memset(tmpbuf,0, sizeof(tmpbuf));

	AL_FillBarByteBit(tmpbuf,offset,FILL93CHAR_TAB[47]);
    offset += 8 + 1;
    while(*barcode != 0xff)
    {
		AL_FillBarByteBit(tmpbuf,offset,FILL93CHAR_TAB[*barcode++]);
        offset += 8 + 1;
    }
    /* ��������խ�׽��� */
	AL_FillBarByteBit(tmpbuf,offset,FILL93CHAR_TAB[47]);
    offset += 8 + 1;
	/* ����������󻹵��ټ�һ��խ�� */
    AL_FillBarByteBit(tmpbuf,offset,0x80);
    offset += 1 + 5;
    return(BarCopyToRealBuf(barbuf,tmpbuf, (offset+7)/8, width));
}
u16 ICACHE_FLASH_ATTR
FillCode93Dot(u8 *barcode, u8 *barbuf, u8 width)
{
    u16 dot;
    u8 tmpin[PrnDotOfByte];
    Gen93Checksum(barcode, tmpin);
    dot = CODE93Barcode(tmpin,barbuf,width);
    HriCode5x7(barcode,barbuf+PrnDotOfByte,dot*8,width);
    return(dot);
}

//============================================================================
//
//                     �ṩ�ⲿ��ӡ����
//
//============================================================================
/******
** ���ϳ�����8λ
** ֻ��������(0~9)
** �̶�ʹ��һλ�����
** ����ʽ����
** ʹ�����ֿ��
******/
u16 ICACHE_FLASH_ATTR
FillBarcodeEan8(u8  * barcode,u8  * barbuf)
{
    u8 barlen;
    /* ��ȡ���봫���ֽ��� */
    barlen = os_strlen((char *)barcode);
    /* ��ȹ̶�Ϊ7��8λ�������7λ��ڰ�λУ���Զ����� */
    if((barlen == 7)||(barlen == 8))
    {
        return(FillEan08Dot(barcode,barbuf,BARCODE_ZOOM));
    }
    return FALSE;
}
//�����ǮUPC_E
u16 ICACHE_FLASH_ATTR
FillBarCodeUPCE(u8 *barcode,u8  * barbuf)
{
	u8 bartype;
	u8 buffer[PrnDotOfByte];
	u8 barlen;

	barlen = os_strlen((char *)barcode);
	if((barlen != 11)&&(barlen != 12))return 0;
	bartype = GenUPCA_EChecksum(barcode, &buffer[0], barlen);

	if ((bartype == EAN13BARCODE) && (buffer[1] == 0))		   //ֻ����̬�����0������ſ���ѹ��
	{
		if ((buffer[4] <= 2)
			&& (buffer[5] == 0)
			&& (buffer[6] == 0)
			&& (buffer[7] == 0)
			&& (buffer[8] == 0))
		{
			//���ѹ������1��0 XXT00 00XXX (T:0-2)��ѹ����T
			buffer[1] = buffer[12];
			buffer[7] = buffer[4];
			buffer[4] = buffer[9];
			buffer[5] = buffer[10];
			buffer[6] = buffer[11];

			barcode[7] = barcode[4];
			barcode[4] = barcode[9];
			barcode[5] = barcode[10];
			barcode[6] = barcode[11];
			barcode[8] = barcode[12];

			bartype = UPCEBARCODE;
//			  printf("UPCE1\r\n");
		}
		else if ((buffer[5] == 0)
			&& (buffer[6] == 0)
			&& (buffer[7] == 0)
			&& (buffer[8] == 0)
			&& (buffer[9] == 0))
		{
			//���ѹ������2��0 XXX00 000XX,ѹ����3
			buffer[1] = buffer[12];
			buffer[5] = buffer[10];
			buffer[6] = buffer[11];
			buffer[7] = 3;
			barcode[5] = barcode[10];
			barcode[6] = barcode[11];
			barcode[7] = '3';
			barcode[8] = barcode[12];

			bartype = UPCEBARCODE;
//			  printf("UPCE2\r\n");
		}
		else if ((buffer[6] == 0)
			&& (buffer[7] == 0)
			&& (buffer[8] == 0)
			&& (buffer[9] == 0)
			&& (buffer[10] == 0))
		{
			//���ѹ������3��0 XXXX0 0000X��ѹ����4
			buffer[1] = buffer[12];
			buffer[6] = buffer[11];
			buffer[7] = 4;
			barcode[6] = barcode[11];
			barcode[7] = '4';
			barcode[8] = barcode[12];

			bartype = UPCEBARCODE;
//			  printf("UPCE3\r\n");
		}
		else if ((buffer[7] == 0)
			&& (buffer[8] == 0)
			&& (buffer[9] == 0)
			&& (buffer[10] == 0)
			&& (buffer[11] >= 5))
		{
			//���ѹ������4��0 XXXXX 0000T (T:5-9)��ѹ����T
			buffer[1] = buffer[12];
			buffer[7] = buffer[11];
			barcode[7] = barcode[11];
			barcode[8] = barcode[12];

			bartype = UPCEBARCODE;
		}
	}

	if(bartype == UPCEBARCODE)
	{

		return(FillCodeUpcEDot(buffer, barbuf, BARCODE_ZOOM));
	}
	else
	{
		u16 result = 0;
		result = Barcode13(buffer, barbuf, BARCODE_ZOOM);
		HriFont13(buffer, barbuf + PrnDotOfByte, BARCODE_ZOOM, (buffer[0] == 0x0a));
		return result;
	}
}

//�����ӡUPCA
u16 ICACHE_FLASH_ATTR
FillBarCodeUPCA(u8  * barcode,u8  * barbuf)
{
    u8 barlen;

    barlen = os_strlen((char *)barcode);

    if((barlen == 11)||(barlen == 12))
    {
        return(FillUpcADot(barcode, barbuf, BARCODE_ZOOM));
    }
    return 0;
}
// ��ӡ 13 ��
u16 ICACHE_FLASH_ATTR
FillBarcodeEan13(u8  * barcode,u8  * barbuf)
{
    u8 barlen;

    barlen = os_strlen((char *)barcode);

    if((barlen == 12)||(barlen == 13))
    {
        // EAN-13��
        return(FillEan13Dot(barcode,barbuf,BARCODE_ZOOM));
    }
    return 0;
}
// ��ӡ����25 ��
u16 ICACHE_FLASH_ATTR
FillBarcodeIT25(u8  * barcode,u8  * barbuf)
{
    u8 barlen;

    barlen = os_strlen((char *)barcode);

    // ����ʽ25��,���ȱ���Ϊż��
    if(((barlen & 0x01) == 0))
    {
        // ��խ����Ϊ 2 : 5
        //return(FillITF25Dot(barcode,barbuf,0,BARCODE_ZOOM));

        // ��խ����Ϊ 2 : 4
        return(FillITF25Dot(barcode,barbuf,1,BARCODE_ZOOM));
    }
    return 0;
}


