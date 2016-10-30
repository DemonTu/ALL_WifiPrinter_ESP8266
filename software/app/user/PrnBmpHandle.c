#include "includes.h"
///////////////////////////////////////////////////////////////////////////////
P_STR_MAP p_str_bitmap;

uint8_t bBitMapFlag = 0;
///////////////////////////////////////////////////////////////////////////////
ENUM_OP_STATE ICACHE_FLASH_ATTR
BitmapInit(void)
{
	bBitMapFlag = 0;
	p_str_bitmap = (P_STR_MAP)common_ram;
	os_memset((u8*)p_str_bitmap,0x00,COM_RAM_SIZE);
    return OP_ERROR;
}

//==============================================================================
/*******************************************************************************
*
*                             λͼ��ӡʹ�ú���
*
*******************************************************************************/
#if (BMP_PRINT_EN)
//����  ��ӡPC���ص�λͼ
//����  buffer  λͼ������
//���  ��
//����  ��
// ע:�����ʽ: GS v 0 xL xH yL yH d1 ... dk
// k = (xL+xH*256)*(yL+yH*256)
// ע: hΪ��ֵ��wΪ��ֵ
u8 ICACHE_FLASH_ATTR
GsPrintImage(u8 *buffer)
{
	/*
	** buffer[0] '0'
	** buffer[1] mode
	** buffer[2] xL  (bytes)
	** buffer[3] xH
	** buffer[4] yL  (bit)
	** buffer[5] yH
	*/
	uint32_t h,w_of_bytes,map_width_by_bit;
	uint32_t w_act_bytes;
	ENUM_MAP_MODE map_mode;
	uint8_t Destbuf[PrnDotOfByte];
	uint8_t *p_buf;
	p_buf = common_ram;

	/* ��ȡͼƬ��С���� */
	w_of_bytes = (uint32_t)buffer[3]*256+buffer[2];
	h		   = (uint32_t)buffer[5]*256+buffer[4];
	/* ���ͼƬ�Ŵ���� */
	if (buffer[1] >= '0')
	{
		buffer[1] -= '0';
	}
	if (buffer[1] < 4)
	{
		map_mode = (ENUM_MAP_MODE)buffer[1];
	}
	else
	{
		map_mode = (ENUM_MAP_MODE)(0);
	}
	///////////////////////////////////////////////////////////////////////////////
	w_act_bytes = (w_of_bytes>PrnDotOfByte)?(PrnDotOfByte):(w_of_bytes);

	if (BITMAP_DOUBLE_WIDTH & map_mode)
	{
		map_width_by_bit = w_act_bytes*8*2;
	}
	else
	{
		map_width_by_bit = w_act_bytes*8;
	}

	strprnprop.str_buf.src_shift_start_bit = 0;
	strprnprop.str_buf.src_buf_size = PrnDotOfByte;
	strprnprop.str_buf.shift_bit_total = map_width_by_bit;
	switch (strprnprop.LineModeM)
	{
		case flush_right:
			strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine-map_width_by_bit);
			break;
		case flush_middle:
			strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine-map_width_by_bit)>>1);
			break;
		case flush_left:
		default:
			break;
	}
///////////////////////////////////////////////////////////////////////////////

	/* ��ʼ��ӡͼƬ */
	while(h--)
	{
		os_memset(p_buf, 0, w_of_bytes);
		os_memset(Destbuf, 0, sizeof(Destbuf));
		WaitPrnBufBytes(w_of_bytes,p_buf,4000,_WAIT_NBYTE_EXIT);
		system_soft_wdt_feed();
		/* ����������� */
		if (BITMAP_DOUBLE_WIDTH & map_mode)
		{
			DataZoomN(Destbuf, p_buf, w_act_bytes, 1);
		}
		else
		{
			os_memcpy(Destbuf, p_buf, w_act_bytes);
		}
		/* ����������� */
		if (BITMAP_DOUBLE_HIGH & map_mode)
		{
			PutBlockToPrintDotBuf(Destbuf,1,2);
		}
		else
		{
			PutBlockToPrintDotBuf(Destbuf,1,1);
		}
	}
	return (0);
}

//����  ��ӡ���ص�λͼ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
GsPrnBitimage(u8 *buffer)
{
#if 0
    uint8_t buf_temp[PrnDotOfByte];
    uint8_t buf[PrnDotOfByte];
    uint8_t bitmap_width;
    uint8_t h;
    h = p_str_bitmap->map_h;
    if (buffer[0] >= '0')
    {
        buffer[0] -='0';
    }
    /* �ж�λͼ��С */
    if (p_str_bitmap->map_w > PrnDotOfLine)
    {
        bitmap_width = PrnDotOfByte;
    }
    else
    {
        bitmap_width = (p_str_bitmap->map_w)/8;
    }
    _TimerDelayReset();
    do
    {
        os_memset(buf, 0, sizeof(buf));
        os_memset(buf_temp, 0, sizeof(buf_temp));
        /* ����������� */
        if (BITMAP_DOUBLE_WIDTH & buffer[0])
        {
            //ZoomBufH(buf, buf_temp, bitmap_width, 2);
			DataZoomN(buf, buf_temp, bitmap_width, 1);
        }
        else
        {
            //ZoomBufH(buf, buf_temp, bitmap_width, 1);
			DataZoomN(buf, buf_temp, bitmap_width, 0);
        }
        /* ����������� */
        if (BITMAP_DOUBLE_HIGH & buffer[1])
        {
            PutBlockToPrintDotBuf(buf,1,1);
        }
        PutBlockToPrintDotBuf(buf,1,1);
        pollint();
        if (_TimerDelay(5000))
        {
            return (0);
        }
    }while((--h));
#endif
    return (0);
}
//����  �趨��ӡλͼģʽ
//����  buffer  �����������
//���  ��
//����  ����
// esc * m nl n2 ( d1 d2 dn ...)
// ����m=0,1,2,3,4,5,6,32,33,38,39,40 (m=39;ÿ��Ϊ24�㣻�����ֽ�)
// 0 <= n1 <= 255; 0 <= n2 <= 31;
// ˮƽ����Ϊ= n2 * 256 + n1

// �������ṹ���£�
// D1  D4   ......
// D2  D5   ......
// D3  D6   ......
u8 ICACHE_FLASH_ATTR
EscSetBitimageMode(u8 * bufferIn)
#if 0
{
	uint16_t i,j;
	uint32_t n = 0;
	uint16_t w=0;
	uint16_t h=0;
	uint16_t data_num=0;
	uint8_t prn_mode;
	uint8_t h_zoon=0;
	uint8_t v_zoon=0;
	uint32_t map_width_by_bit;
	uint16_t temp,base,h_bytes;
	//static uint8_t p_buf[256];
	uint8_t DotBuf[PrnDotOfByte];
#if 0
	uint8_t *p_buf;
	//p_buf = &common_ram[sizeof(P_STR_LINE_BUF)];
	p_buf = &common_ram[0];
#else
	uint8_t p_buf[1200];
#endif

	/* ��ȡ��ӡģʽ */
	prn_mode = bufferIn[0];
	/* ���㳤�� */
	if (bufferIn[2] > 3)
	{
		return ERROR;
	}
	/* ��ȡ���ݳ��� */
	switch(prn_mode)
	{
		case 0:
			h_zoon = 2;
		case 1:
			w = bufferIn[1]+(uint16_t)bufferIn[2]*256;
			h = 8;
			data_num = w;
			if (h_zoon != 2)
			{
				h_zoon = 1;
			}
			v_zoon = 3;
			break;
		case 32:
			h_zoon = 2;
		case 33:
			w = (bufferIn[1]+(uint16_t)bufferIn[2]*256);
			h = 24;
			data_num = w*3;
			if (h_zoon != 2)
			{
				h_zoon = 1;
			}
			v_zoon = 1;
			break;
		default:
			return 0;
	}
	map_width_by_bit = w*h_zoon;
	if (data_num > PrnDotOfLine*3)
	{
		return 0;
	}

	os_memset(p_buf, 0, data_num);
	/* ��ȡ���� */
	WaitPrnBufBytes(data_num, p_buf, 4000,_WAIT_NBYTE_EXIT);

	///////////////////////////////////////////////////////////////////////////////
	strprnprop.str_buf.src_shift_start_bit = 0;
	strprnprop.str_buf.src_buf_size = PrnDotOfByte;
	strprnprop.str_buf.shift_bit_total = map_width_by_bit;
	switch (strprnprop.LineModeM)
	{
		case flush_right:
			if (map_width_by_bit <= PrnDotOfLine)
			{
				strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine-map_width_by_bit);
			}
			break;
		case flush_middle:
			if (map_width_by_bit <= PrnDotOfLine)
			{
				strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine-map_width_by_bit)>>1);
			}
			break;
		case flush_left:
		default:
			break;
	}
	///////////////////////////////////////////////////////////////////////////////
	/* ����ת�� */
	h_bytes = (h+7) / 8;

	//w_bytes = (w+7)/8;
	for (i=0; i<w; i++)
	{
		base = i*h_bytes;

		os_memset(DotBuf, 0x00, sizeof(DotBuf));

		for (j=0; j<h_bytes; j++)
		{
			temp = p_buf[base + j];
			for (n=0; n<8; n++)
			{
				if( temp & bitmap_H[n] )
				{
					/* ����������� */
					if( h_zoon-1 )	DotBuf[j*8+n] = 0xc0;
					else	DotBuf[j*8+n] = 0x80;
				}
			}
		}
		/* ����������� */
		if (v_zoon == 3)
		{
			strprnprop.enheight = 1;
			//PutBlockToPrintDotBuf(dest_data_buf,1,2);
		}
		else
		{
			strprnprop.enheight = 0;
			//PutBlockToPrintDotBuf(dest_data_buf,1,1);
		}
		_IWDG_FEED();
		if ((strprnprop.current_row+h_zoon) <= (PrnDotOfLine - strprnprop.right_space))
		{
			//uint32_t temp;
			//temp = strprnprop.linespace;	// ����line_space
			//strprnprop.linespace = 0;
			bBitMapFlag = 1;
			PutCharDot_To_LineDotBuf(DotBuf, 1 * h_zoon, h);
			//strprnprop.linespace = temp;	// �ָ�line_space
		}
	}
	return (0);
}
#else
{
	uint16_t i,j;
	uint16_t w=0;
	uint16_t h=0;
	uint16_t data_num=0;
	uint8_t prn_mode;
	uint8_t h_zoon=0;
	uint8_t v_zoon=0;
	uint8_t dest_data_buf[PrnDotOfByte];
	uint8_t temp_buf[PrnDotOfByte];
	uint8_t *p_buf;
	uint32_t map_width_by_bit;
	uint16_t base,temp,h_bytes,w_bytes;

	p_buf = common_ram;

	os_memset(p_buf, 0, COM_RAM_SIZE);
	/* ��ȡ��ӡģʽ */
	prn_mode = bufferIn[0];
	/* ���㳤�� */
	if (bufferIn[2] > 3)
	{
		return FALSE;
	}
	/* ��ȡ���ݳ��� */
	switch(prn_mode)
	{
		case 0:
			h_zoon = 2;
		case 1:
			w = bufferIn[1]+(uint16_t)bufferIn[2]*256;
			h = 8;
			data_num = w;
			if (h_zoon != 2)
			{
				h_zoon = 1;
			}
			v_zoon = 3;
			break;
		case 32:
			h_zoon = 2;
		case 33:
			w = (bufferIn[1]+(uint16_t)bufferIn[2]*256);
			h = 24;
			data_num = w*3;
			if (h_zoon != 2)
			{
				h_zoon = 1;
			}
			v_zoon = 1;
			break;
		default:
			return 0;
	}
	map_width_by_bit = w*h_zoon;
	if( data_num>PrnDotOfLine*3)
	{
		return 0;
	}
	/* ��ȡ���� */
	WaitPrnBufBytes(data_num, p_buf, 4000,_WAIT_NBYTE_EXIT);

	///////////////////////////////////////////////////////////////////////////////
	strprnprop.str_buf.src_shift_start_bit = 0;
	strprnprop.str_buf.src_buf_size = PrnDotOfByte;
	strprnprop.str_buf.shift_bit_total = map_width_by_bit;
	switch (strprnprop.LineModeM)
	{
		case flush_right:
			if (PrnDotOfLine >= map_width_by_bit)
			{
				strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine-map_width_by_bit);
			}
			break;
		case flush_middle:
			if (PrnDotOfLine >= map_width_by_bit)
			{
				strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine-map_width_by_bit)>>1);
			}
			break;
		case flush_left:
		default:
			break;
	}
	///////////////////////////////////////////////////////////////////////////////
	/* ����ת�� */
	h_bytes = (h+7)/8;
	w_bytes = (w+7)/8;
	for (i=0; i<h; i++)
	{
		base = i/8;
		temp = i%8;
		os_memset(dest_data_buf,0,PrnDotOfByte);
		os_memset(temp_buf, 0, PrnDotOfByte);
		for(j=0;j<w;j++)
		{
			if( p_buf[base] & bitmap_H[temp] )
			{
				temp_buf[j/8] |= bitmap_H[j&7];
			}
			base += h_bytes;
		}
		/* ����������� */
		DataZoomN(dest_data_buf, &temp_buf[0], w_bytes, h_zoon-1);
		bBitMapFlag = 1;
		/* ����������� */
		if (v_zoon == 3)
		{
			PutBlockToPrintDotBuf(dest_data_buf, 1, 2);
		}
		else
		{
			PutBlockToPrintDotBuf(dest_data_buf, 1, 1);
		}
		system_soft_wdt_feed();
	}
	return (0);
}

#endif



//����  �趨ָ���ַ�λͼ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetUserCharPattern(u8 *buffer)
{
	/**Ŀǰ�����ڴ治�㣬�ݲ�֧��**/
    return (0);
}

#if 1
//����  logo ͼ����������
//����  buffer  �����������
// (ע: (xL + xH*256) <= 1023 ; (yL + yH*256) <= 288)
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
FsLogoDownLoad(u8 *buffer)
{
	unsigned char i,LogoCnt;
	u32 addr = 0;
	u16 ReceiLength = 0;
	u16 x_temp = 0;
	u16 y_temp = 0;
	u8 logo_para_buf[0x100]; // logo ������
	u32 logoaddr;
	//u8 tmpbuf[0x2000];		 // ÿ�� LOGO ���Ϊ 8k
	uint8_t *tmpbuf;
	tmpbuf = common_ram;

	logoaddr = LogoStartAddr;

	LogoCnt = buffer[0];	// logo ��

	if (LogoCnt > 16) // ��� logo ������ 16 ���˳�
	{
		return 0;
	}

	os_memset(logo_para_buf, 0, 0x100); // ��logo������

	if (LogoCnt)
	{
		for (i=0; i<LogoCnt; i++)
		{
			if (WaitPrnBufBytes(4, tmpbuf, 1000, 0) == 4)	// tqy
			{

				x_temp = tmpbuf[1] * 256 + tmpbuf[0];
				y_temp = tmpbuf[3] * 256 + tmpbuf[2];

				if (x_temp > 1024) break;// ��� (xL + xH*256) > 1023 ���˳�
				if (y_temp > 288) break;// ��� (yL + yH*256) > 288 ���˳�
				ReceiLength = x_temp * y_temp * 8;

				if (ReceiLength == 0) break; // ������ճ���Ϊ��
				if (ReceiLength > 0x2000) ReceiLength = 0x2000; // ������ճ��ȴ���0x2000����ȡ0x2000

				// x �����ֽ�(����*8)
				x_temp *= 8;

				logo_para_buf[i*4 + 0] = x_temp % 0x100;
				logo_para_buf[i*4 + 1] = x_temp / 0x100;
				logo_para_buf[i*4 + 2] = y_temp % 0x100;
				logo_para_buf[i*4 + 3] = y_temp / 0x100;

				addr =	logoaddr + 0x1000 + i * 0x2000;

				while (ReceiLength >= 0x100)
				{
					WaitPrnBufBytes(0x100, tmpbuf, 1000, 0);
					WriteNByteToSst(tmpbuf, 0x100, addr, 0);
					addr	+= 0x100;
					ReceiLength -= 0x100;
				}
				if (ReceiLength != 0)
				{
					WaitPrnBufBytes(ReceiLength, tmpbuf, 1000, 0);
					WriteNByteToSst(tmpbuf, ReceiLength, addr, 0);
				}
			}

		}

		WriteNByteToSst(logo_para_buf, 0x100, logoaddr, 0);
	}
	return 0;
}




//�����������,w = ͼƬ���(����),h = ͼƬ�߶�(����)
// hzoom = 1������hzoom=2����
static void ICACHE_FLASH_ATTR
PoxFillxLogo(u8 *src,u8 *map,u16 xpos,u16 srcpos,u16 w,u16 h,u16 hzoom)
{
    u16 i, j, m;
	u16 index;
	u16 pos;
	u8 Mask;
	Mask = 0x80;
	index = 0;
	if( hzoom==0 ) hzoom = 1;
    for (j=0; j < h; j++)
    {
		pos = xpos;
        for (i = srcpos; i < w; i++)
        {
            if( map[i] & Mask )
            {
				for(m=0;m<hzoom;m++)
				{
					if( pos < PrnDotOfLine )
					{
	                	src[index + (pos>>3)] |= bitmap_H[pos&7];
					}
					pos ++;
				}
            }
			else
			{
				pos += hzoom;
			}
        }
		Mask >>= 1;
		index += PrnDotOfByte;
    }
}

u8 ICACHE_FLASH_ATTR
FsLogoPrn(u8 *buffer)
{
	u16 LogoLength;
	u16 LogoX,LogoY;
	u8 LogoCnt;
	u8 LogoMode;	//��ӡLOGOģʽ(=0����;=1����; =2����; =3�����
	u8 vzoom;		// �߶ȷŴ�
	u8 hzoom;		// ˮƽ�Ŵ�
	u16 i,m,n;
	u16 xpos;
	u16 srcpos;
	u32 Addr;

	u8 *tmpbuf;
	u8 *DotBuf;
	tmpbuf = &common_ram[0];		// ����
	DotBuf = &common_ram[512];		// ����,������Ҫ 8*PrnDotOfByte

	if (buffer[0] == 0)
	{
		return 0; // logo �� == 0
	}
	LogoCnt = buffer[0] - 1; // logo ��

	LogoMode = 0;	 // logo ��ӡģʽ
	if ( buffer[1] < 4 )
	{
		LogoMode = buffer[1];	 // logo ��ӡģʽ
	}
	else if ((buffer[1] >= 48) && (buffer[1] < 52))
	{
		LogoMode = buffer[1] - 48;	  // logo ��ӡģʽ
	}

	ReadNByteFromSst(tmpbuf, 0x100, LogoStartAddr); 	// �� logo ������

	// ��ȡͼƬ�ĳ�(��λ:����)����(��λ:�ֽ�)
	LogoX = tmpbuf[LogoCnt*4+0] + tmpbuf[LogoCnt*4+1] * 0x100;
	LogoY = tmpbuf[LogoCnt*4+2] + tmpbuf[LogoCnt*4+3] * 0x100;

	if (LogoX > 576) LogoX = 576; // LOGO ͼ����С�ж�
	if (LogoY > 288) LogoY = 288;

	LogoLength = LogoX * LogoY;

	Addr = LogoStartAddr + 0x1000 + LogoCnt*0x2000; // logo ��ַ

	if(LogoLength <= (8*1024)) // ���LOGO���ݳ�����8K����
	{
		xpos = 0;
		srcpos = 0;
		vzoom = (LogoMode & 0x02) ? 2 : 1;	// �߶ȷŴ�
		hzoom = (LogoMode & 0x01) ? 2 : 1;	// ˮƽ�Ŵ�

		switch(strprnprop.LineModeM)
		{
			case flush_middle://��
				m = LogoX * hzoom;
				if( m < PrnDotOfLine )
				{
					xpos = (PrnDotOfLine - m)/2;
				}
				else
				{
					srcpos = (m - PrnDotOfLine)/2/hzoom;
				}
				break;
			case flush_right://��
				m = LogoX * hzoom;
				if( m < PrnDotOfLine )
				{
					xpos = PrnDotOfLine - m;
				}
				else
				{
					srcpos = (m - PrnDotOfLine)/hzoom;
				}
				break;
			case flush_left:// �����
			default:
				break;
		}
		//==================================================================================
		//if( LogoX ) LogoX --;
		for (i=0; i<LogoY; i++)
		{
			for (m=0; m<LogoX; m++)
			{
				ReadNByteFromSst(&tmpbuf[m], 1, Addr + i + m * LogoY); // ��	LOGO������RAM��
			}
			os_memset(DotBuf, 0x00, PrnDotOfByte * 8); 		// ÿ�δ�ӡ8��
			PoxFillxLogo(DotBuf, tmpbuf, xpos, srcpos, LogoX, 8, hzoom);	// ÿ�δ�ӡ8��
			for (m=0; m<8; m++)
			{
				for (n=0; n<vzoom; n++) // �߶ȷŴ�
				{
					PrnPutOneLineToPrintBuf(&DotBuf[m*PrnDotOfByte]);
				}
			}
		}
	}
	return 1;
}




//����  ȡ���û��Զ����ַ�
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscCancelUserChar(u8 *buffer)
{
    return (0);
}
//����  �趨ȡ���û��Զ����ַ���
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
EscSetCancelUserCharSet(u8 *buffer)
{
    return (0);
}
#endif

//����  DC2 ��ӡλͼ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
Dc2PrintImage(u8 *buffer)
{
	uint8_t x_dot_of_bytes;
	uint8_t y_dot_of_bytes;
	uint8_t buf[PrnDotOfByte];

	x_dot_of_bytes = buffer[1];
	y_dot_of_bytes  = buffer[0];

	if( x_dot_of_bytes>PrnDotOfByte )	/**���ƿ��**/
	{
		x_dot_of_bytes = PrnDotOfByte;
	}

	/* GSCOMMAND */
	os_memset(buf, 0, sizeof(buf));

	strprnprop.str_buf.src_shift_start_bit = 0;
	strprnprop.str_buf.src_buf_size = PrnDotOfByte;
	strprnprop.str_buf.shift_bit_total = x_dot_of_bytes*8;
	switch (strprnprop.LineModeM)
	{
		case flush_right:
			strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine-x_dot_of_bytes*8);
			break;
		case flush_middle:
			strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine-x_dot_of_bytes*8)>>1);
			break;
		case flush_left:
		default:
			break;
	}

    /* ���ƽ������� */
    while(y_dot_of_bytes--)
    {
		os_memset(buf, 0, sizeof(buf));
		WaitPrnBufBytes(x_dot_of_bytes, buf, 1000,_WAIT_NBYTE_EXIT);
		PutBlockToPrintDotBuf(buf,1,1);
		system_soft_wdt_feed();
    }
	return (0);
}

//����  DC2 ��ӡMSBλͼ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
Dc2PrintMsbImage(u8 *buffer)
{
	uint16_t y_dot_of_bytes;
	uint8_t buf[48];

	y_dot_of_bytes	= buffer[0]+(uint16_t)buffer[1]*256;

	/* GSCOMMAND */
	os_memset(buf, 0, sizeof(buf));

	/* ���ƽ������� */
	while(y_dot_of_bytes--)
	{
		os_memset(buf, 0, sizeof(buf));
		WaitPrnBufBytes(48, buf, 1000,_WAIT_NBYTE_EXIT);
		PutBlockToPrintDotBuf(buf,1,1);
		system_soft_wdt_feed();
	}
	return (0);
}

//����  DC2 ��ӡLSBλͼ
//����  buffer  �����������
//���  ��
//����  ����
u8 ICACHE_FLASH_ATTR
Dc2PrintLsbImage(u8 *buffer)
{
	uint16_t y_dot_of_bytes;
	uint8_t buf[48];
	uint8_t i,j,temp;

	y_dot_of_bytes	= buffer[0]+(uint16_t)buffer[1]*256;

	/* GSCOMMAND */
	os_memset(buf, 0, sizeof(buf));

	/* ���ƽ������� */
	while(y_dot_of_bytes--)
	{
		os_memset(buf, 0, sizeof(buf));
		WaitPrnBufBytes(48, buf, 1000,_WAIT_NBYTE_EXIT);
		for(i=0;i<PrnDotOfByte;i++)
		{
			temp = buf[i];
			for(j=0;j<8;j++)
			{
				buf[i] >>= 1;
				if( temp&(0x80>>j))
				{
					buf[i] |= 0x80;
				}
			}
		}
		PutBlockToPrintDotBuf(buf,1,1);
		system_soft_wdt_feed();
	}
	return (0);
}

#endif

//////////////////////////////////////////////////////////////////////

