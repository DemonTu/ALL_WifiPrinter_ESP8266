#ifndef __PRNCHARHANDLE__H__
#define __PRNCHARHANDLE__H__
///////////////////////////////////////////////////////////////////////////////
#ifdef MUL_LANG
//============12 X 24 字体 字库开始地址 0x100000 ===========

#define PortugalAdd  USER_FONT_ADDR             // 葡萄牙文  CP860
#define IsraelAdd    PortugalAdd + 0x1800L      // 希伯来文  CP862
#define RussianAdd   IsraelAdd   + 0x1800L      // 俄文      CP866
#define RumaniaAdd   RussianAdd  + 0x1800L      // 罗马尼亚
#define IranAdd      RumaniaAdd  + 0x1800L      // 波斯文
#define VietnamAdd   IranAdd     + 0x1800L      // 越南文
#define TurkeyAdd    VietnamAdd  + 0x1800L      // 土耳其文 CP1254
#define SpianishAdd  TurkeyAdd   + 0x1800L      // 西班牙文
#define ArmeniaAdd   SpianishAdd + 0x1800L      // 亚美尼亚文
#define ArabiaAdd    ArmeniaAdd  + 0x1800L      // 阿拉伯   CP864
#define CP437Add     ArabiaAdd   + 0x1800L      // 美国     CP437
#define CP850Add     CP437Add    + 0x1800L      // 多种语言 CP850
#define CP852Add     CP850Add    + 0x1800L      // 拉丁语   CP852
#define CP857Add     CP852Add    + 0x1800L      // 土耳其   CP857
#define CP863Add     CP857Add    + 0x1800L      // 加拿大-法语   CP863
#define CP865Add     CP863Add    + 0x1800L      // 挪威     CP865
#define CP737Add     CP865Add    + 0x1800L      // 希腊     CP737
#define IndeaAdd     CP737Add    + 0x1800L      // 印度
#define WPC1250Add   IndeaAdd    + 0x1800L      // 波兰 WPC1250
#define Arabia_1_Add WPC1250Add  + 0x1800L      // 阿拉伯   CP1256
#define WPC1251Add   Arabia_1_Add+ 0x1800L      //  WPC1251
//============  字库结束地址 0x11c800 ===========

//============9 X 17 字体 字库开始地址 0x140000 ===========
#define Portugal_9x17Add  USER_FONT_ADDR+0x40000L                        // 葡萄牙文  CP860
#define Israel_9x17Add    Portugal_9x17Add + 0x1800L      // 希伯来文  CP862     ***暂不支持***
#define Russian_9x17Add   Israel_9x17Add   + 0x1800L      // 俄文      CP866
#define Rumania_9x17Add   Russian_9x17Add  + 0x1800L      // 罗马尼亚            ***暂不支持***
#define Iran_9x17Add      Rumania_9x17Add  + 0x1800L      // 波斯文              ***暂不支持***
#define Vietnam_9x17Add   Iran_9x17Add     + 0x1800L      // 越南文
#define Turkey_9x17Add    Vietnam_9x17Add  + 0x1800L      // 土耳其文 CP1254     ***暂不支持***
#define Spianish_9x17Add  Turkey_9x17Add   + 0x1800L      // 西班牙文
#define Armenia_9x17Add   Spianish_9x17Add + 0x1800L      // 亚美尼亚文          ***暂不支持***
#define Arabia_9x17Add    Armenia_9x17Add  + 0x1800L      // 阿拉伯   CP864      ***暂不支持***
#define CP437_9x17Add     Arabia_9x17Add   + 0x1800L      // 美国     CP437
#define CP850_9x17Add     CP437_9x17Add    + 0x1800L      // 多种语言 CP850
#define CP852_9x17Add     CP850_9x17Add    + 0x1800L      // 拉丁语   CP852
#define CP857_9x17Add     CP852_9x17Add    + 0x1800L      // 土耳其   CP857      ***暂不支持***
#define CP863_9x17Add     CP857_9x17Add    + 0x1800L      // 加拿大-法语   CP863
#define CP865_9x17Add     CP863_9x17Add    + 0x1800L      // 挪威     CP865
#define CP737_9x17Add     CP865_9x17Add    + 0x1800L      // 希腊     CP737      ***暂不支持***
#define Indea_9x17Add     CP737_9x17Add    + 0x1800L      // 印度                ***暂不支持***
#define WPC1250_9x17Add   Indea_9x17Add    + 0x1800L      // 波兰 WPC1250        ***暂不支持***
#define Arabia_1_9x17Add  WPC1250_9x17Add  + 0x1800L      // 阿拉伯   CP1256     ***暂不支持***
#define WPC1251_9x17Add   Arabia_1_9x17Add + 0x1800L      //  WPC1251            ***暂不支持***
//============  字库结束地址 0x15c800 ===========
#else    // 繁体字库定义
#define GbkSymbolAddr       USER_FONT_ADDR   		//0x8000   GBK  符号字形地址
#define GbkGb2312Addr       GbkSymbolAddr+0x123f0	//0x1a3f0  GBK  GB2312地址
#define GbkPart1Addr        GbkGb2312Addr+0x76f80	//0x91370  GBK  part1 地址
#define GbkPart2Addr        GbkPart1Addr+0x6AE00	//0xfc170  GBK  part2 地址
#define Big5ToGbkIdx        GbkPart2Addr+0x93E90	//0x190000 Big5 码转GBK索引码表
#define NewBig5ToGbkIdxFlagAddr Big5ToGbkIdx+0xcc80	//0x19cc80 新旧 Big5 码转GBK索引码表 标志

#define HalfJapanFontAddr   Big5ToGbkIdx - 0x1800 // 日文片甲文地址
#endif

/*
* 行缓存，字体变换缓存配置
*/
#define MAX_LINE_DOT_BUF_HIGH       (24*2)                                     // 最高字体是24所以这里取放大2倍后的高度再倍高
#define MAX_FONT_SIZE               (PrnDotOfByte*MAX_LINE_DOT_BUF_HIGH/2)     // 最大字体变换的缓存大小，目前支持8倍，在单个字体倍宽的情况下会有问题

///////////////////////////////////////////////////////////////////////////////
//==============================================================================
/*
*函数功能: 读取12x24或者8x16的ascii码点阵到缓存
*输入参数: buf:缓存指针，ch:字符
*输出参数: buf指针指向的缓存内容
*返回值  : 无
*/
#define	_ReadASCII_12x24Data(buf,ch)	os_memcpy(buf, CHAR12x24[ch-0x20], sizeof(CHAR12x24[0]))
#define _ReadASCII_8x16Data(buf,ch)		os_memcpy(buf, CHAR8x16[ch-0x20], sizeof(CHAR8x16[0]))
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef __packed struct
{
    uint8_t LineDotBuf[MAX_LINE_DOT_BUF_HIGH][PrnDotOfByte];
    uint8_t Charmap[MAX_FONT_SIZE];
    uint32_t max_high;    // 记录当前行缓冲区最大行高
} STR_LINE_BUF,*P_STR_LINE_BUF;

extern P_STR_LINE_BUF P_STRLineBuf;

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
* 字符处理初始化函数
*/
extern void PrnCharInit(void);
/*
** 将取字符点阵变换后放入行缓冲区
*/
extern void CharacterToCharacterDotBuf(uint16_t ch, ENUM_CHAR_NUM char_num);
/*
* 清行缓冲区数据，0d0a命令经常会用到这个函数
*/
extern void PutLineDotBufToPrnDotBuf(void);
/*
* 放入块到打印缓冲区
*/
extern void PutBlockToPrintDotBuf(uint8_t *buf, u16 LineNum, uint32_t zoom);
/*
** 放入n行空白行到打印点阵缓冲区
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
