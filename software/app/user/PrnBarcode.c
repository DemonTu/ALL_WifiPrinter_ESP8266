#include "includes.h"

/* �������뻺���� */
p_Prn_Barcode pPrnBarcode=(p_Prn_Barcode)common_ram;


void ICACHE_FLASH_ATTR
PrnBarCodeInit(void)
{
	pPrnBarcode=(p_Prn_Barcode)common_ram;
	os_memset((u8*)pPrnBarcode,0x00,COM_RAM_SIZE);
}
//==============================================================================
/*
** ��ӡ�Ϸ�����
*/
static void ICACHE_FLASH_ATTR
PrnHriUpDir(void)
{
    u8  buf[PrnDotOfByte];
    /* ��5�пհ� */
    os_memset(buf, 0, sizeof(buf));
    PutBlockToPrintDotBuf(buf, 1, 5);
    /* ����11�� */
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[1][0], 9, 2);
}
/*
** ��ӡ�·�������
*/
static void ICACHE_FLASH_ATTR
PrnHriDownDir(void)
{
	u8  buf[PrnDotOfByte];
    /* ��5�пհ� */
    os_memset(buf, 0, sizeof(buf));
    PutBlockToPrintDotBuf(buf, 1, 2);

    /* ����16�� */
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[1][0], 9, 2);
}
/*
** ��ӡ����
*/
static void ICACHE_FLASH_ATTR
PrnBarcodeDot(void)
{
    PutBlockToPrintDotBuf(&pPrnBarcode->dotbuf[0][0], 1, strprnprop.barcodeheight);
}
/*
** �ڴ�ӡ��λ���������
*/
static void ICACHE_FLASH_ATTR
FillPrnBarDotToPrnBuf(void)
{
    // ������ĵ���
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
SeekSpeCode(u8  Scode) // ���� CODE128 ������
{
    u8 i ;
    for (i=0; i<9; i++)
    {
        if (code128specode[i] == Scode) break;
    }
    return i;
}


/* code 128�������´���:
      1. �������������ǰ�˲����ַ���ѡ�����ӡ����ֹͣ����������������µ�������Ϊ��ͨ���ݴ���
      2. ���"{"�ͽ����������ַ�����������ָ������ϣ����ӡ��ֹͣ��������Ĵ�����������������Ϊ��ͨ���ݴ���.
      3. �����ӡ�����յ��ַ����������ַ�������  �����ӡ��ֹͣ��������Ĵ�����������������Ϊ��ͨ���ݴ���.
      4. ��ӡHRI�ַ�ʱ������ӡshift�ַ����ַ���ѡ������
      5. �����ַ���HRI�ַ�����ӡ
      6. �����ַ� 0x00 - 0x1f �� 0x7f ��HRI �ַ�Ҳ����ӡ

     �����ַ�    ASCII��  ʮ��������
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
//  CODE128  ͨ������ʹ���ַ���A,B,C,�ܹ���128��ASCII���ַ���0-99�����ּ�һЩ�����ַ����б��롣ÿ���ַ���������ַ�����:
//           �ַ���A: ASCII����ַ� 0x00 - 0x5f
//           �ַ���B: ASCII����ַ� 0x20 - 0x7f
//           �ַ���c: 00 - 99�� 100������
//           �����128�뻹Ҫ�����������ݽ���ת��
u8 ICACHE_FLASH_ATTR
code128codechange(u8  *buffer) // ��������ԭʼCODE128��ת����ʵ�ʴ�ӡ����
{

        u8 sour = 0; // ԭʼ����ָ��
        u8 des = 0;  // Ŀ������ָ��

        u8 tmpbuf[0x100];
        u8 codetype;
        u8 temp;
		u8 len;

		len = os_strlen((char *)buffer);
        os_memcpy(tmpbuf, buffer, len);
        os_memset(buffer, 0x00, len);

        if (tmpbuf[0] != 0x7b)
        {
            return 0; // ����
        }

        codetype = SeekSpeCode(tmpbuf[1]);

        if (codetype > 1) // ��ʼ�������ַ�
        {
            return 0; // ����
        }

        for (sour=2; sour<len; sour++)
        {
            if (tmpbuf[sour] == 0x7b)
            {
                ++ sour;
                temp = SeekSpeCode(tmpbuf[sour]);
                if ((temp == 9) && (codetype == 1)) // ����� CODE B �ַ�����
                {
                    buffer[des++] = 0x7b; // �����ַ�
                }
                else if (temp < 3)
                {
                    codetype = temp; // CODR A/B/C �ַ�������
                }
                else if (temp == 3) // shift ���� (CODE A �� CODEB ֮���л�)
                {
                    if (codetype < 2)
                    {
                        if (codetype == 0 ) codetype = 1;
                        else codetype = 0;
                    }
                }
                // ���������ַ�(FUN1-4 )�ݲ�����
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
                            return 0; // ����
                        }
                        break;
                    case 01:             // code B (0x20 - 0x7f)
                        if ((tmpbuf[sour] > 0x1f) && (tmpbuf[sour] < 0x80))
                        {
                            buffer[des++] = tmpbuf[sour];
                        }
                        else
                        {
                            return 0; // ����
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
                            return 0; // ����
                        }
                        break;
                    default:
                        return 0; // ����
//                        break;
                }
            }
        }
		buffer[des] = 0;
        return des;
}



/*******************************************************************************
** ��������: ��ӡһ����
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

    /* ������� */
    switch(strprnprop.bartype)
    {
        case UPCABARCODE:             // UPC-A    UPC-A������0��ʼ��EAN13��
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
        case CODE39BARCODE:           // CODE39          (���ַ�)
            strprnprop.barcodedot = FillBarcode39(barcode, pBarBuf);
            break;
        case ITFBARCODE:              // ITF     1993 ����ĺ��鱾һ�������룬��ɨ��ǹ����ɨ(ɨ��ǹ��֧��)
            strprnprop.barcodedot = FillBarcodeIT25(barcode, pBarBuf);
            break;
        case CODEBARBARCODE:          // CODABAR         (���ַ�)12345 ����ĺ��鱾һ�������룬��ɨ��ǹ����ɨ(ɨ��ǹ��֧��)
            strprnprop.barcodedot = FillBarcodeCodeBar(barcode, pBarBuf);
            break;
        case CODE93BARCODE:           // CODE93          (���ַ�) ɨ��ǹ����ɨ(ɨ��ǹ��֧��)
            strprnprop.barcodedot = FillBarcCode93(barcode, pBarBuf);
            break;
        case CODE128BARCODE:          // CODE128         (���ַ�)
        	code128codechange(barcode); // ��������ԭʼCODE128��ת����ʵ�ʴ�ӡ����
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
    /* ����������ӡ������ */
    FillPrnBarDotToPrnBuf();
}

