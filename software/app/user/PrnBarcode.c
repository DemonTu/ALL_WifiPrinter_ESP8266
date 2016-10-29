#include "includes.h"

/* 定义条码缓冲区 */
p_Prn_Barcode pPrnBarcode=(p_Prn_Barcode)common_ram;


void ICACHE_FLASH_ATTR
PrnBarCodeInit(void)
{
	pPrnBarcode=(p_Prn_Barcode)common_ram;
	os_memset((u8*)pPrnBarcode,0x00,COM_RAM_SIZE);
}
//==============================================================================
/*
** 打印上方字体
*/
static void ICACHE_FLASH_ATTR
PrnHriUpDir(void)
{
    u8  buf[PrnDotOfByte];
    /* 补5行空白 */
    os_memset(buf, 0, sizeof(buf));
    PutBlockToPrintDotBuf(buf, 1, 5);
    /* 放入11行 */
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[1][0], 9, 2);
}
/*
** 打印下方方字体
*/
static void ICACHE_FLASH_ATTR
PrnHriDownDir(void)
{
	u8  buf[PrnDotOfByte];
    /* 补5行空白 */
    os_memset(buf, 0, sizeof(buf));
    PutBlockToPrintDotBuf(buf, 1, 2);

    /* 放入16行 */
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[1][0], 9, 2);
}
/*
** 打印条码
*/
static void ICACHE_FLASH_ATTR
PrnBarcodeDot(void)
{
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[0][0], 1, strprnprop.barcodeheight);
}
/*
** 在打印的位置填入点阵
*/
static void ICACHE_FLASH_ATTR
FillPrnBarDotToPrnBuf(void)
{
    // 填条码的点阵
    if( strprnprop.barcodeHRIpos&0x1 )
    {
		PrnHriUpDir();
    }
	PrnBarcodeDot();
	if( strprnprop.barcodeHRIpos&0x2 )
	{
		PrnHriDownDir();
	}
}

static const u8 code128specode[9]  = {0x41,0x42,0x43,0x53,0x31,0x32,0x33,0x34,0x7b};

u8 ICACHE_FLASH_ATTR
SeekSpeCode(u8  Scode) // 查找 CODE128 特殊吗
{
    u8 i ;
    for (i=0; i<9; i++)
    {
        if (code128specode[i] == Scode) break;
    }
    return i;
}


/* code 128码做如下处理:
      1. 如果条码数据最前端不是字符集选择，则打印机将停止这条命令处理，并将余下的数据作为普通数据处理。
      2. 如果"{"和紧接着它的字符不是上面所指定的组合，则打印机停止这条命令的处理，并将余下数据作为普通数据处理.
      3. 如果打印机接收的字符不是条码字符集数据  。则打印机停止这条命令的处理，并将余下数据作为普通数据处理.
      4. 打印HRI字符时，不打印shift字符和字符集选择数据
      5. 功能字符的HRI字符不打印
      6. 控制字符 0x00 - 0x1f 及 0x7f 的HRI 字符也不打印

     特殊字符    ASCII码  十六进制码
      shift      {S       0x7b 0x53
      code A     {A       0x7b 0x41
      code B     {B       0x7b 0x42
      code C     {C       0x7b 0x43
      funC1      {1       0x7b 0x31
      funC2      {2       0x7b 0x32
      funC3      {3       0x7b 0x33
      funC4      {4       0x7b 0x34
      {          {{       0x7b 0x7b
*/
//  CODE128  通过交替使用字符集A,B,C,能够对128个ASCII码字符和0-99的数字及一些特殊字符进行编码。每个字符集编码的字符如下:
//           字符集A: ASCII码的字符 0x00 - 0x5f
//           字符集B: ASCII码的字符 0x20 - 0x7f
//           字符集c: 00 - 99的 100个数字
//           如果是128码还要对所输入数据进行转换
u8 ICACHE_FLASH_ATTR
code128codechange(u8  *buffer) // 将所输入原始CODE128码转换成实际打印数据
{

        u8 sour = 0; // 原始数据指针
        u8 des = 0;  // 目的数据指针

        u8 tmpbuf[0x100];
        u8 codetype;
        u8 temp;
		u8 len;

		len = os_strlen((char *)buffer);
        os_memcpy(tmpbuf, buffer, len);
        os_memset(buffer, 0x00, len);

        if (tmpbuf[0] != 0x7b)
        {
            return 0; // 出错
        }

        codetype = SeekSpeCode(tmpbuf[1]);

        if (codetype > 1) // 开始需输入字符
        {
            return 0; // 出错
        }

        for (sour=2; sour<len; sour++)
        {
            if (tmpbuf[sour] == 0x7b)
            {
                ++ sour;
                temp = SeekSpeCode(tmpbuf[sour]);
                if ((temp == 9) && (codetype == 1)) // 如果是 CODE B 字符输入
                {
                    buffer[des++] = 0x7b; // 特殊字符
                }
                else if (temp < 3)
                {
                    codetype = temp; // CODR A/B/C 字符集设置
                }
                else if (temp == 3) // shift 功能 (CODE A 与 CODEB 之间切换)
                {
                    if (codetype < 2)
                    {
                        if (codetype == 0 ) codetype = 1;
                        else codetype = 0;
                    }
                }
                // 其它功能字符(FUN1-4 )暂不处理
            }
            else
            {
                switch(codetype)
                {
                    case 00:             // code A (0x00 - 0x5f)
                        if (tmpbuf[sour] < 0x60)
                        {
                            buffer[des++] = tmpbuf[sour];
                        }
                        else
                        {
                            return 0; // 出错
                        }
                        break;
                    case 01:             // code B (0x20 - 0x7f)
                        if ((tmpbuf[sour] > 0x1f) && (tmpbuf[sour] < 0x80))
                        {
                            buffer[des++] = tmpbuf[sour];
                        }
                        else
                        {
                            return 0; // 出错
                        }
                        break;
                    case 02:             // code C (00 - 99)
                        if (tmpbuf[sour] < 100)
                        {
                            buffer[des++] = ((tmpbuf[sour] & 0xf0) >> 4) + 0x30;
                            buffer[des++] = (tmpbuf[sour] & 0x0f) + 0x30;
                        }
                        else
                        {
                            return 0; // 出错
                        }
                        break;
                    default:
                        return 0; // 出错
//                        break;
                }
            }
        }
		buffer[des] = 0;
        return des;
}



/*******************************************************************************
** 函数功能: 打印一条码
**
**
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrnOneBarcodeSet(u8 *barcode)
{
	uint8_t *pBarBuf;
	uint8_t i;
	uint8_t tempBuf[PrnDotOfByte];
	PrnBarCodeInit();
	pBarBuf = (uint8_t *)&pPrnBarcode->dotbuf[0][0];
	os_memset(pBarBuf, 0x00, sizeof(pPrnBarcode->dotbuf));

    /* 填充条码 */
    switch(strprnprop.bartype)
    {
        case UPCABARCODE:             // UPC-A    UPC-A码是以0开始的EAN13码
            strprnprop.barcodedot = FillBarCodeUPCA(barcode, pBarBuf);
            break;
        case UPCEBARCODE:             // UPC-E    not
            strprnprop.barcodedot = FillBarCodeUPCE(barcode, pBarBuf);
            break;
        case EAN13BARCODE:            // JAN13 (EAN13)
            strprnprop.barcodedot = FillBarcodeEan13(barcode, pBarBuf);
            break;
        case EAN8BARCODE:             // JAN8 (EAN8)
            strprnprop.barcodedot = FillBarcodeEan8(barcode, pBarBuf);
            break;
        case CODE39BARCODE:           // CODE39          (带字符)
            strprnprop.barcodedot = FillBarcode39(barcode, pBarBuf);
            break;
        case ITFBARCODE:              // ITF     1993 打出的和书本一样的条码，但扫描枪不能扫(扫描枪不支持)
            strprnprop.barcodedot = FillBarcodeIT25(barcode, pBarBuf);
            break;
        case CODEBARBARCODE:          // CODABAR         (带字符)12345 打出的和书本一样的条码，但扫描枪不能扫(扫描枪不支持)
            strprnprop.barcodedot = FillBarcodeCodeBar(barcode, pBarBuf);
            break;
        case CODE93BARCODE:           // CODE93          (带字符) 扫描枪不能扫(扫描枪不支持)
            strprnprop.barcodedot = FillBarcCode93(barcode, pBarBuf);
            break;
        case CODE128BARCODE:          // CODE128         (带字符)
        	code128codechange(barcode); // 将所输入原始CODE128码转换成实际打印数据
            strprnprop.barcodedot = FillBarcode128(barcode, pBarBuf);
            break;
        default:
            break;
    }
	strprnprop.barcodedot *= 8;
///////////////////////////////////////////////////////////////////////////////
    switch (strprnprop.LineModeM)
    {
        case flush_right:
            strprnprop.str_buf.dest_shift_start_bit = (PrnDotOfLine-strprnprop.barcodedot);
            strprnprop.str_buf.src_shift_start_bit = 0;
            strprnprop.str_buf.shift_bit_total = strprnprop.barcodedot;
            break;
        case flush_middle:
            strprnprop.str_buf.dest_shift_start_bit = ((PrnDotOfLine-strprnprop.barcodedot)>>1);
            strprnprop.str_buf.src_shift_start_bit = 0;
            strprnprop.str_buf.shift_bit_total = strprnprop.barcodedot;
            break;
        case flush_left:
			if( strprnprop.barcodeleft )
			{
				for(i=0; i<17; i+=1)
				{
					os_memcpy(tempBuf,pPrnBarcode->dotbuf[i],PrnDotOfByte-strprnprop.barcodeleft);
					os_memset(pPrnBarcode->dotbuf[i], 0x00, strprnprop.barcodeleft);
					os_memcpy(&pPrnBarcode->dotbuf[i][strprnprop.barcodeleft],tempBuf,PrnDotOfByte-strprnprop.barcodeleft);
				}
			}
        default:
            break;
    }
    strprnprop.str_buf.src_buf_size = PrnDotOfByte;
///////////////////////////////////////////////////////////////////////////////
    /* 将条码放入打印缓冲区 */
    FillPrnBarDotToPrnBuf();
}

