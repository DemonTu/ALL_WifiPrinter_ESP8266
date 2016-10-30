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
*                             位图打印使用函数
*
*******************************************************************************/
#if (BMP_PRINT_EN)
//函数  打印PC下载的位图
//输入  buffer  位图缓冲区
//输出  无
//返回  无
// 注:命令格式: GS v 0 xL xH yL yH d1 ... dk
// k = (xL+xH*256)*(yL+yH*256)
// 注: h为定值，w为变值
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

	/* 获取图片大小参数 */
	w_of_bytes = (uint32_t)buffer[3]*256+buffer[2];
	h		   = (uint32_t)buffer[5]*256+buffer[4];
	/* 获得图片放大参数 */
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

	/* 开始打印图片 */
	while(h--)
	{
		os_memset(p_buf, 0, w_of_bytes);
		os_memset(Destbuf, 0, sizeof(Destbuf));
		WaitPrnBufBytes(w_of_bytes,p_buf,4000,_WAIT_NBYTE_EXIT);
		system_soft_wdt_feed();
		/* 如果倍宽则处理 */
		if (BITMAP_DOUBLE_WIDTH & map_mode)
		{
			DataZoomN(Destbuf, p_buf, w_act_bytes, 1);
		}
		else
		{
			os_memcpy(Destbuf, p_buf, w_act_bytes);
		}
		/* 如果倍高则处理 */
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

//函数  打印下载的位图
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
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
    /* 判断位图大小 */
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
        /* 如果倍宽则处理 */
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
        /* 如果倍高则处理 */
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
//函数  设定打印位图模式
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
// esc * m nl n2 ( d1 d2 dn ...)
// 其中m=0,1,2,3,4,5,6,32,33,38,39,40 (m=39;每列为24点；三个字节)
// 0 <= n1 <= 255; 0 <= n2 <= 31;
// 水平点数为= n2 * 256 + n1

// 命令点阵结构如下：
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

	/* 获取打印模式 */
	prn_mode = bufferIn[0];
	/* 计算长度 */
	if (bufferIn[2] > 3)
	{
		return ERROR;
	}
	/* 获取数据长度 */
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
	/* 获取数据 */
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
	/* 数据转换 */
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
					/* 如果倍宽则处理 */
					if( h_zoon-1 )	DotBuf[j*8+n] = 0xc0;
					else	DotBuf[j*8+n] = 0x80;
				}
			}
		}
		/* 如果倍高则处理 */
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
			//temp = strprnprop.linespace;	// 备份line_space
			//strprnprop.linespace = 0;
			bBitMapFlag = 1;
			PutCharDot_To_LineDotBuf(DotBuf, 1 * h_zoon, h);
			//strprnprop.linespace = temp;	// 恢复line_space
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
	/* 获取打印模式 */
	prn_mode = bufferIn[0];
	/* 计算长度 */
	if (bufferIn[2] > 3)
	{
		return FALSE;
	}
	/* 获取数据长度 */
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
	/* 获取数据 */
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
	/* 数据转换 */
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
		/* 如果倍宽则处理 */
		DataZoomN(dest_data_buf, &temp_buf[0], w_bytes, h_zoon-1);
		bBitMapFlag = 1;
		/* 如果倍高则处理 */
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



//函数  设定指定字符位图
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetUserCharPattern(u8 *buffer)
{
	/**目前由于内存不足，暂不支持**/
    return (0);
}

#if 1
//函数  logo 图形数据下载
//输入  buffer  命令缓冲区数据
// (注: (xL + xH*256) <= 1023 ; (yL + yH*256) <= 288)
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
FsLogoDownLoad(u8 *buffer)
{
	unsigned char i,LogoCnt;
	u32 addr = 0;
	u16 ReceiLength = 0;
	u16 x_temp = 0;
	u16 y_temp = 0;
	u8 logo_para_buf[0x100]; // logo 参数区
	u32 logoaddr;
	//u8 tmpbuf[0x2000];		 // 每个 LOGO 最大为 8k
	uint8_t *tmpbuf;
	tmpbuf = common_ram;

	logoaddr = LogoStartAddr;

	LogoCnt = buffer[0];	// logo 数

	if (LogoCnt > 16) // 如果 logo 数大于 16 则退出
	{
		return 0;
	}

	os_memset(logo_para_buf, 0, 0x100); // 清logo参数区

	if (LogoCnt)
	{
		for (i=0; i<LogoCnt; i++)
		{
			if (WaitPrnBufBytes(4, tmpbuf, 1000, 0) == 4)	// tqy
			{

				x_temp = tmpbuf[1] * 256 + tmpbuf[0];
				y_temp = tmpbuf[3] * 256 + tmpbuf[2];

				if (x_temp > 1024) break;// 如果 (xL + xH*256) > 1023 则退出
				if (y_temp > 288) break;// 如果 (yL + yH*256) > 288 则退出
				ReceiLength = x_temp * y_temp * 8;

				if (ReceiLength == 0) break; // 如果接收长度为零
				if (ReceiLength > 0x2000) ReceiLength = 0x2000; // 如果接收长度大于0x2000，则取0x2000

				// x 保存字节(点数*8)
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




//把纵向点填入,w = 图片宽度(点数),h = 图片高度(点数)
// hzoom = 1不倍宽，hzoom=2倍宽
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
	u8 LogoMode;	//打印LOGO模式(=0正常;=1倍宽; =2倍高; =3倍宽高
	u8 vzoom;		// 高度放大
	u8 hzoom;		// 水平放大
	u16 i,m,n;
	u16 xpos;
	u16 srcpos;
	u32 Addr;

	u8 *tmpbuf;
	u8 *DotBuf;
	tmpbuf = &common_ram[0];		// 借用
	DotBuf = &common_ram[512];		// 借用,至少需要 8*PrnDotOfByte

	if (buffer[0] == 0)
	{
		return 0; // logo 数 == 0
	}
	LogoCnt = buffer[0] - 1; // logo 数

	LogoMode = 0;	 // logo 打印模式
	if ( buffer[1] < 4 )
	{
		LogoMode = buffer[1];	 // logo 打印模式
	}
	else if ((buffer[1] >= 48) && (buffer[1] < 52))
	{
		LogoMode = buffer[1] - 48;	  // logo 打印模式
	}

	ReadNByteFromSst(tmpbuf, 0x100, LogoStartAddr); 	// 读 logo 参数区

	// 读取图片的长(单位:点数)跟宽(单位:字节)
	LogoX = tmpbuf[LogoCnt*4+0] + tmpbuf[LogoCnt*4+1] * 0x100;
	LogoY = tmpbuf[LogoCnt*4+2] + tmpbuf[LogoCnt*4+3] * 0x100;

	if (LogoX > 576) LogoX = 576; // LOGO 图幅大小判断
	if (LogoY > 288) LogoY = 288;

	LogoLength = LogoX * LogoY;

	Addr = LogoStartAddr + 0x1000 + LogoCnt*0x2000; // logo 地址

	if(LogoLength <= (8*1024)) // 如果LOGO数据长度在8K以内
	{
		xpos = 0;
		srcpos = 0;
		vzoom = (LogoMode & 0x02) ? 2 : 1;	// 高度放大
		hzoom = (LogoMode & 0x01) ? 2 : 1;	// 水平放大

		switch(strprnprop.LineModeM)
		{
			case flush_middle://中
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
			case flush_right://右
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
			case flush_left:// 左对齐
			default:
				break;
		}
		//==================================================================================
		//if( LogoX ) LogoX --;
		for (i=0; i<LogoY; i++)
		{
			for (m=0; m<LogoX; m++)
			{
				ReadNByteFromSst(&tmpbuf[m], 1, Addr + i + m * LogoY); // 读	LOGO数据至RAM中
			}
			os_memset(DotBuf, 0x00, PrnDotOfByte * 8); 		// 每次打印8行
			PoxFillxLogo(DotBuf, tmpbuf, xpos, srcpos, LogoX, 8, hzoom);	// 每次打印8行
			for (m=0; m<8; m++)
			{
				for (n=0; n<vzoom; n++) // 高度放大
				{
					PrnPutOneLineToPrintBuf(&DotBuf[m*PrnDotOfByte]);
				}
			}
		}
	}
	return 1;
}




//函数  取消用户自定义字符
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscCancelUserChar(u8 *buffer)
{
    return (0);
}
//函数  设定取消用户自定义字符集
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
EscSetCancelUserCharSet(u8 *buffer)
{
    return (0);
}
#endif

//函数  DC2 打印位图
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
Dc2PrintImage(u8 *buffer)
{
	uint8_t x_dot_of_bytes;
	uint8_t y_dot_of_bytes;
	uint8_t buf[PrnDotOfByte];

	x_dot_of_bytes = buffer[1];
	y_dot_of_bytes  = buffer[0];

	if( x_dot_of_bytes>PrnDotOfByte )	/**限制宽度**/
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

    /* 复制进缓冲区 */
    while(y_dot_of_bytes--)
    {
		os_memset(buf, 0, sizeof(buf));
		WaitPrnBufBytes(x_dot_of_bytes, buf, 1000,_WAIT_NBYTE_EXIT);
		PutBlockToPrintDotBuf(buf,1,1);
		system_soft_wdt_feed();
    }
	return (0);
}

//函数  DC2 打印MSB位图
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
Dc2PrintMsbImage(u8 *buffer)
{
	uint16_t y_dot_of_bytes;
	uint8_t buf[48];

	y_dot_of_bytes	= buffer[0]+(uint16_t)buffer[1]*256;

	/* GSCOMMAND */
	os_memset(buf, 0, sizeof(buf));

	/* 复制进缓冲区 */
	while(y_dot_of_bytes--)
	{
		os_memset(buf, 0, sizeof(buf));
		WaitPrnBufBytes(48, buf, 1000,_WAIT_NBYTE_EXIT);
		PutBlockToPrintDotBuf(buf,1,1);
		system_soft_wdt_feed();
	}
	return (0);
}

//函数  DC2 打印LSB位图
//输入  buffer  命令缓冲区数据
//输出  无
//返回  空闲
u8 ICACHE_FLASH_ATTR
Dc2PrintLsbImage(u8 *buffer)
{
	uint16_t y_dot_of_bytes;
	uint8_t buf[48];
	uint8_t i,j,temp;

	y_dot_of_bytes	= buffer[0]+(uint16_t)buffer[1]*256;

	/* GSCOMMAND */
	os_memset(buf, 0, sizeof(buf));

	/* 复制进缓冲区 */
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

