#ifndef __PRN_EPSON_H__
#define __PRN_EPSON_H__
//==============================================================================
#undef EPSONDEBUG
//==============================================================================
#define PAGE_MODE_EN            0
#define BARCODE_EN              1
#define BMP_PRINT_EN            1
#define PAPER_MARK_EN           0
#define SET_FONT_SIZE_EN        1
#define PRN_TEST_EN             0
//==============================================================================
#define DEFAULT_LINE_SPACE              31                                      // 默认行空白,约等于3.875mm
#define DEFAULT_CHAR_SPACE              0                                       // 默认字符间距
#define X_DEFAULT_BASIC_MOVE_UNIT       200                                     // 默认的移动基本单位
#define Y_DEFAULT_BASIC_MOVE_UNIT       200                                     // 默认的移动基本单位
#define PAGE_MODE_MAX_WIDTH             PrnDotOfLine                            // 页模式最大宽度
#define PAGE_MODE_MAX_HIGH              320                                     // 默认的一张标签大小
#define TAB_NUM                         32                                      // 跳格总数
//==============================================================================
//定义打印的控制字和参数
#define VK_WIDTH        0x01    //行中间的字符控制字,用于控制下一个字符宽度是否需要放大一倍,中文无效
#define VK_HEIGHT       0x02    //行中间的字符控制字,用于控制下一个字符高度是否需要放大一倍
#define VK_PPECR        0x03    //PPECR通讯包的开始
#define VK_DELMARK      0x04
#define VK_UNDERLINE    0x05
#define VK_REVERSE      0x06
#define VK_UPDOWN       0x07
#define VK_LARGE        0x07    //行中间的字符控制字,用于控制下一个字符宽度,高度是否需要放大一倍

#define VK_NUL          0x00
#define VK_EOT          0x04    //强制打印机读状态(不管打印机是否ONLINE)
#define VK_ENQ          0x05    //强制打印机动作(不管打印机是否ONLINE)
#define VK_BEL          0x07
#define VK_BACKSPACE    0x08    // 退格
#define VK_HT           0x09    // TAB
#define VK_LF           0x0a    // '\n'
#define VK_VT           0x0b
#define VK_FF           0x0c
#define VK_CR           0x0d    // '\r'
#define VK_SO           0x0e
#define VK_SI           0x0f
#define VK_DLE          0x10   // 强制打印机动作(不管打印机是否ONLINE)
#define VK_DC1          0x11
#define VK_XON          0x11
#define VK_DC2          0x12
#define VK_DC3          0x13
#define VK_XOFF         0x13
#define VK_DC4          0x14
#define VK_CAN          0x18
#define VK_EM           0x19
#define VK_SUB          0x1a
#define VK_ESC          0x1b
#define VK_FS           0x1c
#define VK_GS           0x1d
#define VK_RS           0x1e
typedef enum
{
	_PRNUNDERLINE =   0x01,  /* 下滑线 */
	_PRNREVERSE   =   0x02,  /* 反白 */
	_PRNUPDOWN    =   0x04,  /* 上下导致 */
	_PRNEMPHASIZE =   0x08,  /* 粗体 */
	_PRNENHEIGHT  =   0x10,  /* 倍高 */
	_PRNENWIDTH   =   0x20,  /* 倍宽 */
    _PRNDELMARK   =   0x40,  /* 删除线 */
    _PRNRollRight90 = 0x80,  /* 顺时针旋转90度 */
} font_property;
//==============================================================================
typedef enum
{
    flush_left   = 0,
    flush_middle = 1,
    flush_right  = 2,
} LINE_MODE;
typedef enum
{
    NO_PAPER              = 0x0001,
    CUT_PAPER_ERROR       = 0x0002,
} ERROR_STATE_BIT;
typedef enum
{
    PRINTER_ON = 0,
    PRINTER_OFF,
} PRINTER_STATE;

//*****************************************************************************
typedef struct
{
    uint8_t w;
    uint8_t h;
}STR_FONT_SIZE;
typedef enum
{
    FONT12X24 = 0,
    FONT8X16,
    FONT5X7,
    FONT8X8,
    FONT16X32,
    FONT9X24,
    FONT_MAX,
}ENUM_FONT_SIZE;
//*****************************************************************************
//*****************************************************************************
typedef enum
{
    SERIES_PAPER = 0,
    LABEL_PAPER = 1,            // 有底缝隙标签纸
    BLACK_SIGN_PAPER,       // 无底黑标标签纸
}ENUM_PAPER_TYPE;
typedef enum
{
    USA = 0,  // default
    FRANCE = 1,
    GERMANY,
    UK,
    DENMARK1,
    SWEDEN,
    ITALY,
    SPAIN1,
    JAPAN,
    NORWAY,
    DENMARK2,
    SPAIN2,
    LATIN_AMERICA,
    KOREA,
    CHINESE_S,
    CHINESE_T,
}ENUM_LANGUAGE;
//*****************************************************************************
typedef enum
{
    PC437 = 0,
    katakana = 1,
    PC850,
    PC860,
    PC863,
    PC865,
    WPC1252 = 16,
    PC866 = 17,
    PC852 = 18,
    PC858 = 19,
}ENUM_CODE_PAGE;
//==============================================================================
extern const STR_FONT_SIZE FONT_MAP[FONT_MAX];

typedef __packed struct
{
    u16  isvalid;         	/* 判断设定是否合法 */
    uint32_t baudrate;    	/* 默认波特率 */
    ENUM_PAPER_TYPE paper_type;
    u8   printenable;     	/* 是否启动打印机 */
    u8   autostatus;      	/* 是否自动回应状态 */
    ENUM_LANGUAGE   charsset;/* 定义使用的国际字符集，定义使用的国家 */
    u8   codetbl;			/* 定义使用的字符表 ，codepage */
    u8   prop;            	/* 打印控制属性 */
    u8   density;         	/* 设定打印浓度 */
    u8 	 htabpos[34];		/* 水平TAB位置,最多设置32个跳格位置 */
	u8   htabcnt;      		/* 水平TAB计数值 */
    u8   bufferbusy;      	/* 接收缓冲区状态 */
    u16  bufferused;      	/* 打印缓冲区剩余的状态 */
    u8   enwidth;         	/* 水平放大倍数 字体正常使用时宽度会被放大的倍数 */
    u8   enheight;        	/* 垂直放大倍数 字体正常使用时高度会被放大的倍数 */
    uint8_t xunit;          /* 定义基本单位,如果为零则是基本单位 */
    uint8_t yunit;          /* 定义基本单位，该命令用gs p 来设置 */
    ENUM_FONT_SIZE font; 	/* 设定打印使用的字体 */
    uint32_t LineModeM; 	/* 行对齐模式 */
    uint16_t current_row; 	/* 打印当前列 */
    uint32_t underline_para;
    /* 标准模式参数 */
    uint16_t left_space;   	/* 标准模式左边距  左右边距移动的单位是一个字节 */
    uint16_t right_space;  	/* 标准模式右边距 */
    u8   linespace;       	/* 设定标准模式行间距 */
    u8   charspace;       	/* 定义标准模式字符间距 */
    /* 条码参数 */
    u8   barcodewidth;    	/* 定义条码大小 */
    u8   barcodeheight;
    u8   barcodeleft;     	/* 定义条码的水平偏移 */
    u16  barcodeHRIpos;
    u32  barcodedot;      	/* 定义条码点阵大小 */
    ENUM_FONT_SIZE barcodeHRIfont;
    u8   bartype;
    /* 定义行参数 */
    STR_BufFillBufByBit str_buf;
} STRPRNPROP;
//==============================================================================
extern STRPRNPROP strprnprop;
//==============================================================================
/*
*   Windows下打印测试页的命令如下:
*  <ESC> "@"   初始化打印机
*  <ESC> "=" 1 启动打印机
*  <GS>  "P" b4,b4 设定x,y的解析度,180dpi,180dpi
*  <ESC> "R" 0 选择字符集usa
*  <ESC> "t" 2 选择字符表2,PC850多语言
*  <ESC> "c3" 4 设定Sensor类型
*  <ESC> "c4" 0 关闭缺纸侦测
*  <ESC> "2" 设定缺省行间距
*   00 0D
*  <ESC> "J" 5B 进纸91单位
*  <ESC> "J" 0  进纸0单位
*  <GS> "v" 30 00 00 1d 00 18 ... 打印位图29bytes*24dots((w*h)

*  <HT>        水平TAB
*  <LF>        打印,进纸
*  <CAN>       取消页模式的打印数据
*  <DLE EOT>   实时状态查询
*  <DLE ENQ>   到打印机的实时请求
*  <FF>        打印,返回到标准模式
*  <ESC> <FF>        打印页模式的打印数据
*  <FF>        命令打印缓冲区的所有数据并且返回到标准模式.打印完毕后打印缓冲区数据被清空.
*              命令恢复<ESC>"W"设定的值到缺省值.
*              命令<ESC>"T"设定的值不变
*              命令只有在页模式有效
*  <ESC> <FF>  命令打印页模式中打印范围内的所有数据,命令只有在页模式有效
*              命令不改变设定值
*              当返回到标准模式时,<FF>或者<ESC> <S>命令必须被执行
*/
/*******************************************************************************
*
*                            打印属性设置函数
*
*******************************************************************************/
/*
*        <ESC> "!" n
*        位        功能
*        0        0(字体12x24),1(字体9x24)
*        1-2        保留
*        3        0(低浓度),1(高浓度)
*        4        0(单倍高度),1(双倍高度)
*        5        0(双倍宽度),1(单倍宽度)
*        6        保留
*        7        0(取消模式定义),1(设定模式定义)
*/
extern u8 EscSelectPageMode(u8 *buffer);                               // 'L'选择页模式
/****************/
/* 外设状态读取 */
/****************/
extern u8 EscInitPrinter(u8 *buffer);                                        // '@'初始化打印机
extern u8 FSChangeBaudTate(u8 *buffer);                                // 'a' 设置波特率
extern u8 FSSetPrintDensity(u8 *buffer);                                  // 'b' 设置打印浓度
extern u8 GsReadPrintID(u8 *buffer);                                      // 'I'发送打印机ID
extern u8 EscSetPrinterOnLine(u8 *buffer);                                // '='选择外设
//  <ESC> "p" 0 10 20
//  0       选择抽屉0
//  10      打开时间
//  20      关闭时间
extern u8 EscOutDevice(u8 *buffer);                                       // 'p'产生脉冲
extern u8 EscReadOutdeviceStatus(u8 *buffer);                         // 'u'发送外设状态
extern u8 EscReadPaperStatus(u8 *buffer);                              // 'v'发送纸张状态
extern u8 GsSetAutoAckStatus(u8 *buffer);                              // 'a'允许/关闭自动状态反馈
//<ESC> "c3"        选择输出纸张完毕信号
//<ESC> "c4"        选择用户结束打印的信号
//<ESC> "c5"        允许/关闭按键
extern u8 EscSetKeyAvailable(u8 *buffer);                               // 'c'选择打印纸传感器以输出缺纸信号
extern u8 GetPrintStu(void);
extern u8 GPExterntFilter(u8 *buffer);
extern u8 GPExterntCmd(u8 *buffer);
extern u8 GPExterntCut(u8 *buffer);
extern u8 GPOutDevice(u8 *buffer);                                        // 产生钱箱控制脉冲(非标准EPSON命令,此为佳伯打印机专用)
extern u8 GsReadStatus(u8 *buffer);                                       // 'r'发送状态
/********************/
/* 字体变形设置命令 */
/********************/
extern u8 EscSetPrintmode(u8 *buffer);                                   // '!'选择打印模式，全局设置字体大小下划线等等参数
extern u8 EscSetEmphasize(u8 *buffer);                                  // 'E'设定/取消着重模式
extern u8 EscSetDoubleStrike(u8 *buffer);                                // 'G'设定/取消双倍黑度模式
extern u8 EscSetUpDown(u8 *buffer);                                     // '{'打印上下倒置
extern u8 EscSetUnderLine(u8 *buffer);                                   // '-'设置/取消下划线
extern u8 EscSetRotate90(u8 *buffer);                                     // 'V'顺时钟方向旋转90度
extern u8 GsSetBlackWhite(u8 *buffer);                                   // 'B'设定/取消反白
extern u8 EscDoubeWidthSetting(u8 *buffer);
extern u8 EscSetDoubleWidth(u8 *buffer);
extern u8 EscClearDoubleWidth(u8 *buffer);

/**************************/
/* 行间距字符间距设置命令 */
/**************************/
extern u8 EscSetDefLineSpace(u8 *buffer);                               // '2'设定缺省的行空白
extern u8 EscSetLineSpace(u8 *buffer);                                   // '3'设定行空白
extern u8 GsSetLeftMargin(u8 *buffer);                                    // 'L'设定左边距
extern u8 EscSetCharSpace(u8 *buffer);                                  // <SP>设定字符右间距
extern u8 FsSelectHzBhw(u8 *buffer);                                    // 'S'//选择/取消汉字倍高倍宽
extern u8 FsSelectHzULine(u8 *buffer);
extern u8 GsSetPageWidth(u8 *buffer);                                    // 'W'标准模式设置可打印尺寸
/****************/
/* 字符设置命令 */
/****************/
extern u8 EscSetFontSize(u8 *buffer);                                     // 'M'设置字体大小
extern u8 EscSelectCharsTbl(u8 *buffer);                                 // 't'选择字符代码表
extern u8 EscSelectCharsSet(u8 *buffer);                                 // 'R'选择国际字符集
extern u8 GsSetFontEnlarge(u8 *buffer);                                  // '!' 设定字符大小
extern u8 FsSetPrintmode(u8 *buffer);                                    // '!'设定汉字打印模式组合
extern u8 FsSetWordInt(u8 *buffer);                                       // '%'设定汉字字间距
/****************/
/* 位置设置命令 */
/****************/
extern u8 EscSetAbsPosition(u8 *buffer);                                 // '$' 设定打印的绝对位置
extern u8 EscSetRelPosition(u8 *buffer);                                  // '\'设置打印的相对位置
extern u8 GsSetMotionUnit(u8 *buffer);                                    // 'P'设定水平垂直基本单位
extern u8 EscSetPageLineMode(u8 * buffer);                            // 'a' 对齐模式 (左/右/居中)
/************/
/* 走纸命令 */
/************/
extern u8 EscPrnBuffer(u8 *buffer);                                        // 'd'打印,前进n行
extern u8 EscPrnBufferFeed(u8 *buffer);                                  // 'J'打印,进纸,命令用于需要暂时改变这一行进纸距离
/**************/
/* 宏定义命令 */
/**************/
extern u8 GsStartEndMacro(u8 *buffer);                                   // ':'开始/结束宏定义
extern u8 GsExeMacro(u8 *buffer);                                         // '^'执行宏
/************/
/* 黑表命令 */
/************/
extern u8 PrintTestPage(u8 *buffer);                                       //设置黑标检测位置及撕纸位置偏移量
extern u8 GscBlankMark(u8 *buffer);                                       // 前进纸至黑标位置
/*
*     <ESC> "D" n1,n2,n3...nk <NUL>
*     k        1-32,可以设定最多32个TAB的位置
*     n1..nk    每个TAB的位置
*     每次设定TAB的位置取消前一次的设定
*/
extern u8 EscSetTabsPos(u8 *buffer);                                     // 'D'设定水平TAB的位置

extern u8 GsSetSmoothMode(u8 *buffer);                                 // 'b'设置连续模式
extern u8 EscLQ1600KNoUseCmd(u8 * buffer);                          // 无用
extern u8 GpOpenDrawer(u8 * buffer);                                // 为与佳伯打印机兼容而加
extern void SaveHighRam(u8 *dest,u8 length);
extern u8 EscSettingPrinter(uint8_t *buffer);
extern u8 EscSetSystemSleep(uint8_t *buffer);
/*******************************************************************************
*
*                      条码打印使用函数
*
*******************************************************************************/
#if (BARCODE_EN)
extern u8 GsSetHRIFont(u8 *buffer);                                       // 'f'设定fri的字体
extern u8 GsSetHRIPosition(u8 *buffer);                                 // 'H'选择HRI字符的打印位置
extern u8 GsSetBarcodeWidth(u8 *buffer);                              // 'w'设置条码线条的宽度
extern u8 GsSetBarcodeHeight(u8 *buffer);                              // 'h'设置条码的高度
extern u8 GsPrintBarcode(u8 *buffer);                                    // 'k'打印条码
extern u8 GsPrinterBarcodeLeft(u8 *buffer);

#else
#define GsSetHRIFont                      (NULL)                                       // 'f'设定fri的字体
#define GsSetHRIPosition                  (NULL)                                // 'H'选择HRI字符的打印位置
#define GsSetBarcodeWidth                 (NULL)                              // 'w'设置条码线条的宽度
#define GsSetBarcodeHeight                (NULL)                              // 'h'设置条码的高度
#define GsPrintBarcode                    (NULL)                                 // 'k'打印条码
#define GsPrinterBarcodeLeft              (NULL)                                 // 'k'打印条码

#endif
/*******************************************************************************
*
*                             位图打印使用函数
*
*******************************************************************************/
#if (BMP_PRINT_EN)
/*
*  <GS> "v0" m wL wH hL hH
*  m       指定分辨率
*  wH,wL   位图宽度
*  hH,hL   位图高度
*/
extern u8 GsPrintImage(u8 *buffer);                                      // 'v' 打印光栅位图
extern u8 GsPrnBitimage(u8 *buffer);                                     // '/'打印下载的位图
extern u8 EscSetBitimageMode(u8 *buffer);                             // '*'设置位图模式
extern u8 EscSetUserCharPattern(u8 *buffer);                          // '&'定义用户自定义字符位图,就是字体map
extern u8 EscSetCancelUserCharSet(u8 *buffer);                      // '%'设置/取消用户定义的字符集
extern u8 FsLogoDownLoad(u8 *buffer);                                // 'q' logo 图形数据下载
extern u8 FsLogoPrn(u8 *buffer);
extern u8 EscCancelUserChar(u8 *buffer);                              // '?'取消用户定义的字符
extern u8 Dc2PrintImage(u8 *buffer);						/** 打印指定宽度高度位图 **/
extern u8 Dc2PrintMsbImage(u8 *buffer);				/** 打印指定MSB位图 **/
extern u8 Dc2PrintLsbImage(u8 *buffer);				/** 打印指定MSB位图 **/

#else
#define GsPrintImage                    (NULL)                                      // 'v' 打印光栅位图
#define GsPrnBitimage                   (NULL)                                     // '/'打印下载的位图
#define EscSetBitimageMode              (NULL)                             // '*'设置位图模式
#define EscSetUserCharPattern           (NULL)                          // '&'定义用户自定义字符位图,就是字体map
#define EscSetCancelUserCharSet         (NULL)                      // '%'设置/取消用户定义的字符集
#define FsLogoDownLoad                  (NULL)                                // 'q' logo 图形数据下载
#define FsLogoPrn						(NULL)
#define EscCancelUserChar               (NULL)                              // '?'取消用户定义的字符
#define	Dc2PrintImage					(NULL)
#define	Dc2PrintMsbImage				(NULL)
#define	Dc2PrintLsbImage				(NULL)
#endif
/*******************************************************************************
*
*                             页模式所用函数
*
*******************************************************************************/
#if (PAGE_MODE_EN)
extern u8 EscPrnPageData(u8 *buffer);                                    // VK_FF页模式下打印
extern u8 EscSelectStandardMode(u8 *buffer);                          // 'S'选择标准模式
extern u8 EscSetPrnDirection(u8 *buffer);                                // 'T'设置页模式下打印区域方向
extern u8 EscSetPageSetting(u8 *buffer);                                // 'W'设置页模式下打印区域
extern u8 GsSetAbsVertPosition(u8 *buffer);                            // '$'页模式选择垂直位置的绝对位置
extern u8 GsSetRelVertPosition(u8 *buffer);                             // '\'页模式中设定垂直的相对位置
extern u8 EscSetLine(uint8_t *buffer);                                     // 'l' 页模式下画线
#else
#define EscPrnPageData                  (NULL)                                    // VK_FF页模式下打印
#define EscSelectStandardMode           (NULL)                          // 'S'选择标准模式
#define EscSetPrnDirection              (NULL)                                // 'T'设置页模式下打印区域方向
#define EscSetPageSetting               (NULL)                                // 'W'设置页模式下打印区域
#define GsSetAbsVertPosition            (NULL)                            // '$'页模式选择垂直位置的绝对位置
#define GsSetRelVertPosition            (NULL)                             // '\'页模式中设定垂直的相对位置
#define EscSetLine                      (NULL)                                   // 'l' 页模式下画线

#endif
/*******************************************************************************
*
*                      切纸所用函数
*
*******************************************************************************/
extern u8 GsCutPaper(u8 *buffer);                                         // 'V'切纸
extern u8 EscCutPaper(u8 *buffer);                                        // 'i'半切纸
/*******************************************************************************
*
*                               外部引用函数
*
*******************************************************************************/
void PutBlankLineToPrintDotBuf(u16 LineNum);
void CursorToNextTabPosition(void);

/*******************************************************************************
*
*                      DC2所用函数
*
*******************************************************************************/
extern u8 Dc2PrintTestPage(uint8_t *buffer);
extern u8 Dc2SetPrintDark(uint8_t *buffer);


extern u8  GsExpCmd(u8  *buffer);

#endif

