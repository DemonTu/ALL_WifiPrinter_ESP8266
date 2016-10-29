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
#define DEFAULT_LINE_SPACE              31                                      // Ĭ���пհ�,Լ����3.875mm
#define DEFAULT_CHAR_SPACE              0                                       // Ĭ���ַ����
#define X_DEFAULT_BASIC_MOVE_UNIT       200                                     // Ĭ�ϵ��ƶ�������λ
#define Y_DEFAULT_BASIC_MOVE_UNIT       200                                     // Ĭ�ϵ��ƶ�������λ
#define PAGE_MODE_MAX_WIDTH             PrnDotOfLine                            // ҳģʽ�����
#define PAGE_MODE_MAX_HIGH              320                                     // Ĭ�ϵ�һ�ű�ǩ��С
#define TAB_NUM                         32                                      // ��������
//==============================================================================
//�����ӡ�Ŀ����ֺͲ���
#define VK_WIDTH        0x01    //���м���ַ�������,���ڿ�����һ���ַ�����Ƿ���Ҫ�Ŵ�һ��,������Ч
#define VK_HEIGHT       0x02    //���м���ַ�������,���ڿ�����һ���ַ��߶��Ƿ���Ҫ�Ŵ�һ��
#define VK_PPECR        0x03    //PPECRͨѶ���Ŀ�ʼ
#define VK_DELMARK      0x04
#define VK_UNDERLINE    0x05
#define VK_REVERSE      0x06
#define VK_UPDOWN       0x07
#define VK_LARGE        0x07    //���м���ַ�������,���ڿ�����һ���ַ����,�߶��Ƿ���Ҫ�Ŵ�һ��

#define VK_NUL          0x00
#define VK_EOT          0x04    //ǿ�ƴ�ӡ����״̬(���ܴ�ӡ���Ƿ�ONLINE)
#define VK_ENQ          0x05    //ǿ�ƴ�ӡ������(���ܴ�ӡ���Ƿ�ONLINE)
#define VK_BEL          0x07
#define VK_BACKSPACE    0x08    // �˸�
#define VK_HT           0x09    // TAB
#define VK_LF           0x0a    // '\n'
#define VK_VT           0x0b
#define VK_FF           0x0c
#define VK_CR           0x0d    // '\r'
#define VK_SO           0x0e
#define VK_SI           0x0f
#define VK_DLE          0x10   // ǿ�ƴ�ӡ������(���ܴ�ӡ���Ƿ�ONLINE)
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
	_PRNUNDERLINE =   0x01,  /* �»��� */
	_PRNREVERSE   =   0x02,  /* ���� */
	_PRNUPDOWN    =   0x04,  /* ���µ��� */
	_PRNEMPHASIZE =   0x08,  /* ���� */
	_PRNENHEIGHT  =   0x10,  /* ���� */
	_PRNENWIDTH   =   0x20,  /* ���� */
    _PRNDELMARK   =   0x40,  /* ɾ���� */
    _PRNRollRight90 = 0x80,  /* ˳ʱ����ת90�� */
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
    LABEL_PAPER = 1,            // �е׷�϶��ǩֽ
    BLACK_SIGN_PAPER,       // �޵׺ڱ��ǩֽ
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
    u16  isvalid;         	/* �ж��趨�Ƿ�Ϸ� */
    uint32_t baudrate;    	/* Ĭ�ϲ����� */
    ENUM_PAPER_TYPE paper_type;
    u8   printenable;     	/* �Ƿ�������ӡ�� */
    u8   autostatus;      	/* �Ƿ��Զ���Ӧ״̬ */
    ENUM_LANGUAGE   charsset;/* ����ʹ�õĹ����ַ���������ʹ�õĹ��� */
    u8   codetbl;			/* ����ʹ�õ��ַ��� ��codepage */
    u8   prop;            	/* ��ӡ�������� */
    u8   density;         	/* �趨��ӡŨ�� */
    u8 	 htabpos[34];		/* ˮƽTABλ��,�������32������λ�� */
	u8   htabcnt;      		/* ˮƽTAB����ֵ */
    u8   bufferbusy;      	/* ���ջ�����״̬ */
    u16  bufferused;      	/* ��ӡ������ʣ���״̬ */
    u8   enwidth;         	/* ˮƽ�Ŵ��� ��������ʹ��ʱ��Ȼᱻ�Ŵ�ı��� */
    u8   enheight;        	/* ��ֱ�Ŵ��� ��������ʹ��ʱ�߶Ȼᱻ�Ŵ�ı��� */
    uint8_t xunit;          /* ���������λ,���Ϊ�����ǻ�����λ */
    uint8_t yunit;          /* ���������λ����������gs p ������ */
    ENUM_FONT_SIZE font; 	/* �趨��ӡʹ�õ����� */
    uint32_t LineModeM; 	/* �ж���ģʽ */
    uint16_t current_row; 	/* ��ӡ��ǰ�� */
    uint32_t underline_para;
    /* ��׼ģʽ���� */
    uint16_t left_space;   	/* ��׼ģʽ��߾�  ���ұ߾��ƶ��ĵ�λ��һ���ֽ� */
    uint16_t right_space;  	/* ��׼ģʽ�ұ߾� */
    u8   linespace;       	/* �趨��׼ģʽ�м�� */
    u8   charspace;       	/* �����׼ģʽ�ַ���� */
    /* ������� */
    u8   barcodewidth;    	/* ���������С */
    u8   barcodeheight;
    u8   barcodeleft;     	/* ���������ˮƽƫ�� */
    u16  barcodeHRIpos;
    u32  barcodedot;      	/* ������������С */
    ENUM_FONT_SIZE barcodeHRIfont;
    u8   bartype;
    /* �����в��� */
    STR_BufFillBufByBit str_buf;
} STRPRNPROP;
//==============================================================================
extern STRPRNPROP strprnprop;
//==============================================================================
/*
*   Windows�´�ӡ����ҳ����������:
*  <ESC> "@"   ��ʼ����ӡ��
*  <ESC> "=" 1 ������ӡ��
*  <GS>  "P" b4,b4 �趨x,y�Ľ�����,180dpi,180dpi
*  <ESC> "R" 0 ѡ���ַ���usa
*  <ESC> "t" 2 ѡ���ַ���2,PC850������
*  <ESC> "c3" 4 �趨Sensor����
*  <ESC> "c4" 0 �ر�ȱֽ���
*  <ESC> "2" �趨ȱʡ�м��
*   00 0D
*  <ESC> "J" 5B ��ֽ91��λ
*  <ESC> "J" 0  ��ֽ0��λ
*  <GS> "v" 30 00 00 1d 00 18 ... ��ӡλͼ29bytes*24dots((w*h)

*  <HT>        ˮƽTAB
*  <LF>        ��ӡ,��ֽ
*  <CAN>       ȡ��ҳģʽ�Ĵ�ӡ����
*  <DLE EOT>   ʵʱ״̬��ѯ
*  <DLE ENQ>   ����ӡ����ʵʱ����
*  <FF>        ��ӡ,���ص���׼ģʽ
*  <ESC> <FF>        ��ӡҳģʽ�Ĵ�ӡ����
*  <FF>        �����ӡ���������������ݲ��ҷ��ص���׼ģʽ.��ӡ��Ϻ��ӡ���������ݱ����.
*              ����ָ�<ESC>"W"�趨��ֵ��ȱʡֵ.
*              ����<ESC>"T"�趨��ֵ����
*              ����ֻ����ҳģʽ��Ч
*  <ESC> <FF>  �����ӡҳģʽ�д�ӡ��Χ�ڵ���������,����ֻ����ҳģʽ��Ч
*              ����ı��趨ֵ
*              �����ص���׼ģʽʱ,<FF>����<ESC> <S>������뱻ִ��
*/
/*******************************************************************************
*
*                            ��ӡ�������ú���
*
*******************************************************************************/
/*
*        <ESC> "!" n
*        λ        ����
*        0        0(����12x24),1(����9x24)
*        1-2        ����
*        3        0(��Ũ��),1(��Ũ��)
*        4        0(�����߶�),1(˫���߶�)
*        5        0(˫�����),1(�������)
*        6        ����
*        7        0(ȡ��ģʽ����),1(�趨ģʽ����)
*/
extern u8 EscSelectPageMode(u8 *buffer);                               // 'L'ѡ��ҳģʽ
/****************/
/* ����״̬��ȡ */
/****************/
extern u8 EscInitPrinter(u8 *buffer);                                        // '@'��ʼ����ӡ��
extern u8 FSChangeBaudTate(u8 *buffer);                                // 'a' ���ò�����
extern u8 FSSetPrintDensity(u8 *buffer);                                  // 'b' ���ô�ӡŨ��
extern u8 GsReadPrintID(u8 *buffer);                                      // 'I'���ʹ�ӡ��ID
extern u8 EscSetPrinterOnLine(u8 *buffer);                                // '='ѡ������
//  <ESC> "p" 0 10 20
//  0       ѡ�����0
//  10      ��ʱ��
//  20      �ر�ʱ��
extern u8 EscOutDevice(u8 *buffer);                                       // 'p'��������
extern u8 EscReadOutdeviceStatus(u8 *buffer);                         // 'u'��������״̬
extern u8 EscReadPaperStatus(u8 *buffer);                              // 'v'����ֽ��״̬
extern u8 GsSetAutoAckStatus(u8 *buffer);                              // 'a'����/�ر��Զ�״̬����
//<ESC> "c3"        ѡ�����ֽ������ź�
//<ESC> "c4"        ѡ���û�������ӡ���ź�
//<ESC> "c5"        ����/�رհ���
extern u8 EscSetKeyAvailable(u8 *buffer);                               // 'c'ѡ���ӡֽ�����������ȱֽ�ź�
extern u8 GetPrintStu(void);
extern u8 GPExterntFilter(u8 *buffer);
extern u8 GPExterntCmd(u8 *buffer);
extern u8 GPExterntCut(u8 *buffer);
extern u8 GPOutDevice(u8 *buffer);                                        // ����Ǯ���������(�Ǳ�׼EPSON����,��Ϊ�Ѳ���ӡ��ר��)
extern u8 GsReadStatus(u8 *buffer);                                       // 'r'����״̬
/********************/
/* ��������������� */
/********************/
extern u8 EscSetPrintmode(u8 *buffer);                                   // '!'ѡ���ӡģʽ��ȫ�����������С�»��ߵȵȲ���
extern u8 EscSetEmphasize(u8 *buffer);                                  // 'E'�趨/ȡ������ģʽ
extern u8 EscSetDoubleStrike(u8 *buffer);                                // 'G'�趨/ȡ��˫���ڶ�ģʽ
extern u8 EscSetUpDown(u8 *buffer);                                     // '{'��ӡ���µ���
extern u8 EscSetUnderLine(u8 *buffer);                                   // '-'����/ȡ���»���
extern u8 EscSetRotate90(u8 *buffer);                                     // 'V'˳ʱ�ӷ�����ת90��
extern u8 GsSetBlackWhite(u8 *buffer);                                   // 'B'�趨/ȡ������
extern u8 EscDoubeWidthSetting(u8 *buffer);
extern u8 EscSetDoubleWidth(u8 *buffer);
extern u8 EscClearDoubleWidth(u8 *buffer);

/**************************/
/* �м���ַ������������ */
/**************************/
extern u8 EscSetDefLineSpace(u8 *buffer);                               // '2'�趨ȱʡ���пհ�
extern u8 EscSetLineSpace(u8 *buffer);                                   // '3'�趨�пհ�
extern u8 GsSetLeftMargin(u8 *buffer);                                    // 'L'�趨��߾�
extern u8 EscSetCharSpace(u8 *buffer);                                  // <SP>�趨�ַ��Ҽ��
extern u8 FsSelectHzBhw(u8 *buffer);                                    // 'S'//ѡ��/ȡ�����ֱ��߱���
extern u8 FsSelectHzULine(u8 *buffer);
extern u8 GsSetPageWidth(u8 *buffer);                                    // 'W'��׼ģʽ���ÿɴ�ӡ�ߴ�
/****************/
/* �ַ��������� */
/****************/
extern u8 EscSetFontSize(u8 *buffer);                                     // 'M'���������С
extern u8 EscSelectCharsTbl(u8 *buffer);                                 // 't'ѡ���ַ������
extern u8 EscSelectCharsSet(u8 *buffer);                                 // 'R'ѡ������ַ���
extern u8 GsSetFontEnlarge(u8 *buffer);                                  // '!' �趨�ַ���С
extern u8 FsSetPrintmode(u8 *buffer);                                    // '!'�趨���ִ�ӡģʽ���
extern u8 FsSetWordInt(u8 *buffer);                                       // '%'�趨�����ּ��
/****************/
/* λ���������� */
/****************/
extern u8 EscSetAbsPosition(u8 *buffer);                                 // '$' �趨��ӡ�ľ���λ��
extern u8 EscSetRelPosition(u8 *buffer);                                  // '\'���ô�ӡ�����λ��
extern u8 GsSetMotionUnit(u8 *buffer);                                    // 'P'�趨ˮƽ��ֱ������λ
extern u8 EscSetPageLineMode(u8 * buffer);                            // 'a' ����ģʽ (��/��/����)
/************/
/* ��ֽ���� */
/************/
extern u8 EscPrnBuffer(u8 *buffer);                                        // 'd'��ӡ,ǰ��n��
extern u8 EscPrnBufferFeed(u8 *buffer);                                  // 'J'��ӡ,��ֽ,����������Ҫ��ʱ�ı���һ�н�ֽ����
/**************/
/* �궨������ */
/**************/
extern u8 GsStartEndMacro(u8 *buffer);                                   // ':'��ʼ/�����궨��
extern u8 GsExeMacro(u8 *buffer);                                         // '^'ִ�к�
/************/
/* �ڱ����� */
/************/
extern u8 PrintTestPage(u8 *buffer);                                       //���úڱ���λ�ü�˺ֽλ��ƫ����
extern u8 GscBlankMark(u8 *buffer);                                       // ǰ��ֽ���ڱ�λ��
/*
*     <ESC> "D" n1,n2,n3...nk <NUL>
*     k        1-32,�����趨���32��TAB��λ��
*     n1..nk    ÿ��TAB��λ��
*     ÿ���趨TAB��λ��ȡ��ǰһ�ε��趨
*/
extern u8 EscSetTabsPos(u8 *buffer);                                     // 'D'�趨ˮƽTAB��λ��

extern u8 GsSetSmoothMode(u8 *buffer);                                 // 'b'��������ģʽ
extern u8 EscLQ1600KNoUseCmd(u8 * buffer);                          // ����
extern u8 GpOpenDrawer(u8 * buffer);                                // Ϊ��Ѳ���ӡ�����ݶ���
extern void SaveHighRam(u8 *dest,u8 length);
extern u8 EscSettingPrinter(uint8_t *buffer);
extern u8 EscSetSystemSleep(uint8_t *buffer);
/*******************************************************************************
*
*                      �����ӡʹ�ú���
*
*******************************************************************************/
#if (BARCODE_EN)
extern u8 GsSetHRIFont(u8 *buffer);                                       // 'f'�趨fri������
extern u8 GsSetHRIPosition(u8 *buffer);                                 // 'H'ѡ��HRI�ַ��Ĵ�ӡλ��
extern u8 GsSetBarcodeWidth(u8 *buffer);                              // 'w'�������������Ŀ��
extern u8 GsSetBarcodeHeight(u8 *buffer);                              // 'h'��������ĸ߶�
extern u8 GsPrintBarcode(u8 *buffer);                                    // 'k'��ӡ����
extern u8 GsPrinterBarcodeLeft(u8 *buffer);

#else
#define GsSetHRIFont                      (NULL)                                       // 'f'�趨fri������
#define GsSetHRIPosition                  (NULL)                                // 'H'ѡ��HRI�ַ��Ĵ�ӡλ��
#define GsSetBarcodeWidth                 (NULL)                              // 'w'�������������Ŀ��
#define GsSetBarcodeHeight                (NULL)                              // 'h'��������ĸ߶�
#define GsPrintBarcode                    (NULL)                                 // 'k'��ӡ����
#define GsPrinterBarcodeLeft              (NULL)                                 // 'k'��ӡ����

#endif
/*******************************************************************************
*
*                             λͼ��ӡʹ�ú���
*
*******************************************************************************/
#if (BMP_PRINT_EN)
/*
*  <GS> "v0" m wL wH hL hH
*  m       ָ���ֱ���
*  wH,wL   λͼ���
*  hH,hL   λͼ�߶�
*/
extern u8 GsPrintImage(u8 *buffer);                                      // 'v' ��ӡ��դλͼ
extern u8 GsPrnBitimage(u8 *buffer);                                     // '/'��ӡ���ص�λͼ
extern u8 EscSetBitimageMode(u8 *buffer);                             // '*'����λͼģʽ
extern u8 EscSetUserCharPattern(u8 *buffer);                          // '&'�����û��Զ����ַ�λͼ,��������map
extern u8 EscSetCancelUserCharSet(u8 *buffer);                      // '%'����/ȡ���û�������ַ���
extern u8 FsLogoDownLoad(u8 *buffer);                                // 'q' logo ͼ����������
extern u8 FsLogoPrn(u8 *buffer);
extern u8 EscCancelUserChar(u8 *buffer);                              // '?'ȡ���û�������ַ�
extern u8 Dc2PrintImage(u8 *buffer);						/** ��ӡָ����ȸ߶�λͼ **/
extern u8 Dc2PrintMsbImage(u8 *buffer);				/** ��ӡָ��MSBλͼ **/
extern u8 Dc2PrintLsbImage(u8 *buffer);				/** ��ӡָ��MSBλͼ **/

#else
#define GsPrintImage                    (NULL)                                      // 'v' ��ӡ��դλͼ
#define GsPrnBitimage                   (NULL)                                     // '/'��ӡ���ص�λͼ
#define EscSetBitimageMode              (NULL)                             // '*'����λͼģʽ
#define EscSetUserCharPattern           (NULL)                          // '&'�����û��Զ����ַ�λͼ,��������map
#define EscSetCancelUserCharSet         (NULL)                      // '%'����/ȡ���û�������ַ���
#define FsLogoDownLoad                  (NULL)                                // 'q' logo ͼ����������
#define FsLogoPrn						(NULL)
#define EscCancelUserChar               (NULL)                              // '?'ȡ���û�������ַ�
#define	Dc2PrintImage					(NULL)
#define	Dc2PrintMsbImage				(NULL)
#define	Dc2PrintLsbImage				(NULL)
#endif
/*******************************************************************************
*
*                             ҳģʽ���ú���
*
*******************************************************************************/
#if (PAGE_MODE_EN)
extern u8 EscPrnPageData(u8 *buffer);                                    // VK_FFҳģʽ�´�ӡ
extern u8 EscSelectStandardMode(u8 *buffer);                          // 'S'ѡ���׼ģʽ
extern u8 EscSetPrnDirection(u8 *buffer);                                // 'T'����ҳģʽ�´�ӡ������
extern u8 EscSetPageSetting(u8 *buffer);                                // 'W'����ҳģʽ�´�ӡ����
extern u8 GsSetAbsVertPosition(u8 *buffer);                            // '$'ҳģʽѡ��ֱλ�õľ���λ��
extern u8 GsSetRelVertPosition(u8 *buffer);                             // '\'ҳģʽ���趨��ֱ�����λ��
extern u8 EscSetLine(uint8_t *buffer);                                     // 'l' ҳģʽ�»���
#else
#define EscPrnPageData                  (NULL)                                    // VK_FFҳģʽ�´�ӡ
#define EscSelectStandardMode           (NULL)                          // 'S'ѡ���׼ģʽ
#define EscSetPrnDirection              (NULL)                                // 'T'����ҳģʽ�´�ӡ������
#define EscSetPageSetting               (NULL)                                // 'W'����ҳģʽ�´�ӡ����
#define GsSetAbsVertPosition            (NULL)                            // '$'ҳģʽѡ��ֱλ�õľ���λ��
#define GsSetRelVertPosition            (NULL)                             // '\'ҳģʽ���趨��ֱ�����λ��
#define EscSetLine                      (NULL)                                   // 'l' ҳģʽ�»���

#endif
/*******************************************************************************
*
*                      ��ֽ���ú���
*
*******************************************************************************/
extern u8 GsCutPaper(u8 *buffer);                                         // 'V'��ֽ
extern u8 EscCutPaper(u8 *buffer);                                        // 'i'����ֽ
/*******************************************************************************
*
*                               �ⲿ���ú���
*
*******************************************************************************/
void PutBlankLineToPrintDotBuf(u16 LineNum);
void CursorToNextTabPosition(void);

/*******************************************************************************
*
*                      DC2���ú���
*
*******************************************************************************/
extern u8 Dc2PrintTestPage(uint8_t *buffer);
extern u8 Dc2SetPrintDark(uint8_t *buffer);


extern u8  GsExpCmd(u8  *buffer);

#endif

