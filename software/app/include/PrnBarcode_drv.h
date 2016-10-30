#ifndef __PRN__DRIVER_BARCODE_H__
#define __PRN__DRIVER_BARCODE_H__
//==============================================================================

#define BarFontHigh         7
#define BarFontHigh2        7

#define BARCODE_ZOOM 		(strprnprop.barcodewidth - 1)	/* 条码默认放大倍数，数据放大 N 倍   0 不放大 */

typedef struct
{
    u8 dotbuf[17][PrnDotOfByte];      	/* 1点行条码 + 16点行数字 */
    uint32_t barcode_h;            		/* 条码宽度 */
} *p_Prn_Barcode;

extern const u8 ClrHBit[8];
extern const u8 ClrLBit[8];
extern const u8 SetHBit[8];
//////////////////////////////////////////////////////////////////
/* 输入barcode字符串，返回填好的barcode */
extern u16 FillBarCodeUPCA(u8  * barcode,u8  * barbuf);
extern u16 FillBarCodeUPCE(u8 *barcode,u8  * barbuf);
extern u16 FillBarcodeEan8(u8  * barcode,u8  * barbuf);
extern u16 FillBarcodeEan13(u8  * barcode,u8  * barbuf);
extern u16 FillBarcodeIT25(u8  * barcode,u8  * barbuf);

#if 0
extern u16 FillBarcode128(u8  *barcode,u8  *barbuf);
extern u16 FillBarcode39(u8  *barcode,u8  *barbuf);
extern u16 FillBarcodeCodeBar(u8  *barcode,u8  *barbuf);
extern u16 FillBarcCode93(u8 *barcode, u8 *barbuf);
#else
extern u16 FillCode128Dot(u8 * barcode,u8  * barbuf,u8 width);
extern u16 FillCode39Dot(u8 *barcode, u8 *barbuf, u8 width);
extern u16 FillCodeCodebarDot(u8 *barcode, u8 *barbuf, u8 width);
extern u16 FillCode93Dot(u8 *barcode, u8 *barbuf, u8 width);


// 128码
#define	FillBarcode128(barcode,barbuf)		FillCode128Dot(barcode,barbuf,BARCODE_ZOOM)

// 39码
#define	FillBarcode39(barcode,barbuf)		FillCode39Dot(barcode,barbuf,BARCODE_ZOOM)

/* CODEBAR码 */
#define FillBarcodeCodeBar(barcode,barbuf)	FillCodeCodebarDot(barcode,barbuf,BARCODE_ZOOM)

//打印93码
#define FillBarcCode93(barcode,barbuf)		FillCode93Dot(barcode,barbuf,BARCODE_ZOOM)
#endif

u16 DataZoomN(u8  * destbuf, u8  * srcbuf, u16 dot,u8 zoom);

//==============================================================================
#endif
