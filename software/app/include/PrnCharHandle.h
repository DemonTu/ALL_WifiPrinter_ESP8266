#ifndef __PRNCHARHANDLE__H__
#define __PRNCHARHANDLE__H__
///////////////////////////////////////////////////////////////////////////////
#ifdef MUL_LANG
//============12 X 24 ���� �ֿ⿪ʼ��ַ 0x100000 ===========

#define PortugalAdd  USER_FONT_ADDR             // ��������  CP860
#define IsraelAdd    PortugalAdd + 0x1800L      // ϣ������  CP862
#define RussianAdd   IsraelAdd   + 0x1800L      // ����      CP866
#define RumaniaAdd   RussianAdd  + 0x1800L      // ��������
#define IranAdd      RumaniaAdd  + 0x1800L      // ��˹��
#define VietnamAdd   IranAdd     + 0x1800L      // Խ����
#define TurkeyAdd    VietnamAdd  + 0x1800L      // �������� CP1254
#define SpianishAdd  TurkeyAdd   + 0x1800L      // ��������
#define ArmeniaAdd   SpianishAdd + 0x1800L      // ����������
#define ArabiaAdd    ArmeniaAdd  + 0x1800L      // ������   CP864
#define CP437Add     ArabiaAdd   + 0x1800L      // ����     CP437
#define CP850Add     CP437Add    + 0x1800L      // �������� CP850
#define CP852Add     CP850Add    + 0x1800L      // ������   CP852
#define CP857Add     CP852Add    + 0x1800L      // ������   CP857
#define CP863Add     CP857Add    + 0x1800L      // ���ô�-����   CP863
#define CP865Add     CP863Add    + 0x1800L      // Ų��     CP865
#define CP737Add     CP865Add    + 0x1800L      // ϣ��     CP737
#define IndeaAdd     CP737Add    + 0x1800L      // ӡ��
#define WPC1250Add   IndeaAdd    + 0x1800L      // ���� WPC1250
#define Arabia_1_Add WPC1250Add  + 0x1800L      // ������   CP1256
#define WPC1251Add   Arabia_1_Add+ 0x1800L      //  WPC1251
//============  �ֿ������ַ 0x11c800 ===========

//============9 X 17 ���� �ֿ⿪ʼ��ַ 0x140000 ===========
#define Portugal_9x17Add  USER_FONT_ADDR+0x40000L                        // ��������  CP860
#define Israel_9x17Add    Portugal_9x17Add + 0x1800L      // ϣ������  CP862     ***�ݲ�֧��***
#define Russian_9x17Add   Israel_9x17Add   + 0x1800L      // ����      CP866
#define Rumania_9x17Add   Russian_9x17Add  + 0x1800L      // ��������            ***�ݲ�֧��***
#define Iran_9x17Add      Rumania_9x17Add  + 0x1800L      // ��˹��              ***�ݲ�֧��***
#define Vietnam_9x17Add   Iran_9x17Add     + 0x1800L      // Խ����
#define Turkey_9x17Add    Vietnam_9x17Add  + 0x1800L      // �������� CP1254     ***�ݲ�֧��***
#define Spianish_9x17Add  Turkey_9x17Add   + 0x1800L      // ��������
#define Armenia_9x17Add   Spianish_9x17Add + 0x1800L      // ����������          ***�ݲ�֧��***
#define Arabia_9x17Add    Armenia_9x17Add  + 0x1800L      // ������   CP864      ***�ݲ�֧��***
#define CP437_9x17Add     Arabia_9x17Add   + 0x1800L      // ����     CP437
#define CP850_9x17Add     CP437_9x17Add    + 0x1800L      // �������� CP850
#define CP852_9x17Add     CP850_9x17Add    + 0x1800L      // ������   CP852
#define CP857_9x17Add     CP852_9x17Add    + 0x1800L      // ������   CP857      ***�ݲ�֧��***
#define CP863_9x17Add     CP857_9x17Add    + 0x1800L      // ���ô�-����   CP863
#define CP865_9x17Add     CP863_9x17Add    + 0x1800L      // Ų��     CP865
#define CP737_9x17Add     CP865_9x17Add    + 0x1800L      // ϣ��     CP737      ***�ݲ�֧��***
#define Indea_9x17Add     CP737_9x17Add    + 0x1800L      // ӡ��                ***�ݲ�֧��***
#define WPC1250_9x17Add   Indea_9x17Add    + 0x1800L      // ���� WPC1250        ***�ݲ�֧��***
#define Arabia_1_9x17Add  WPC1250_9x17Add  + 0x1800L      // ������   CP1256     ***�ݲ�֧��***
#define WPC1251_9x17Add   Arabia_1_9x17Add + 0x1800L      //  WPC1251            ***�ݲ�֧��***
//============  �ֿ������ַ 0x15c800 ===========
#else    // �����ֿⶨ��
#define GbkSymbolAddr       USER_FONT_ADDR   		//0x8000   GBK  �������ε�ַ
#define GbkGb2312Addr       GbkSymbolAddr+0x123f0	//0x1a3f0  GBK  GB2312��ַ
#define GbkPart1Addr        GbkGb2312Addr+0x76f80	//0x91370  GBK  part1 ��ַ
#define GbkPart2Addr        GbkPart1Addr+0x6AE00	//0xfc170  GBK  part2 ��ַ
#define Big5ToGbkIdx        GbkPart2Addr+0x93E90	//0x190000 Big5 ��תGBK�������
#define NewBig5ToGbkIdxFlagAddr Big5ToGbkIdx+0xcc80	//0x19cc80 �¾� Big5 ��תGBK������� ��־

#define HalfJapanFontAddr   Big5ToGbkIdx - 0x1800 // ����Ƭ���ĵ�ַ
#endif

/*
* �л��棬����任��������
*/
#define MAX_LINE_DOT_BUF_HIGH       (24*2)                                     // ���������24��������ȡ�Ŵ�2����ĸ߶��ٱ���
#define MAX_FONT_SIZE               (PrnDotOfByte*MAX_LINE_DOT_BUF_HIGH/2)     // �������任�Ļ����С��Ŀǰ֧��8�����ڵ������屶�������»�������

///////////////////////////////////////////////////////////////////////////////
//==============================================================================
/*
*��������: ��ȡ12x24����8x16��ascii����󵽻���
*�������: buf:����ָ�룬ch:�ַ�
*�������: bufָ��ָ��Ļ�������
*����ֵ  : ��
*/
#define	_ReadASCII_12x24Data(buf,ch)	os_memcpy(buf, CHAR12x24[ch-0x20], sizeof(CHAR12x24[0]))
#define _ReadASCII_8x16Data(buf,ch)		os_memcpy(buf, CHAR8x16[ch-0x20], sizeof(CHAR8x16[0]))
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef __packed struct
{
    uint8_t LineDotBuf[MAX_LINE_DOT_BUF_HIGH][PrnDotOfByte];
    uint8_t Charmap[MAX_FONT_SIZE];
    uint32_t max_high;    // ��¼��ǰ�л���������и�
} STR_LINE_BUF,*P_STR_LINE_BUF;

extern P_STR_LINE_BUF P_STRLineBuf;

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
* �ַ������ʼ������
*/
extern void PrnCharInit(void);
/*
** ��ȡ�ַ�����任������л�����
*/
extern void CharacterToCharacterDotBuf(uint16_t ch, ENUM_CHAR_NUM char_num);
/*
* ���л��������ݣ�0d0a��������õ��������
*/
extern void PutLineDotBufToPrnDotBuf(void);
/*
* ����鵽��ӡ������
*/
extern void PutBlockToPrintDotBuf(uint8_t *buf, u16 LineNum, uint32_t zoom);
/*
** ����n�пհ��е���ӡ���󻺳���
*/
extern void PutBlankLineToPrintDotBuf(u16 LineNum);
//==============================================================================
extern void ReadCHINESE_24x24Data(uint8_t *buf,uint32_t ch);
extern void ReadCHINESE_16x16Data(uint8_t *buf,uint32_t ch);
extern void ReadASCII_12x24Data(uint8_t *buf,uint8_t ch);
extern void ReadASCII_Data(u8 font,uint8_t width,uint8_t height,uint8_t *buf,uint8_t ch);
extern void Internationalcode(u8 Code);
extern void PutBlackLine(uint32_t line);
extern void PutCharDot_To_LineDotBuf(uint8_t *buf, uint8_t w, uint8_t h);

#endif
