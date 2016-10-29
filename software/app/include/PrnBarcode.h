#ifndef __PRN__BARCODE__H__
#define __PRN__BARCODE__H__
//==============================================================================
typedef enum
{
    UPCABARCODE    =   0,
    UPCEBARCODE    =   1,
    EAN13BARCODE   =   2,
    EAN8BARCODE    =   3,
    CODE39BARCODE  =   4,
    ITFBARCODE     =   5,
    CODEBARBARCODE =   6,
    CODE93BARCODE  =   7,
    CODE128BARCODE =   8,
}STR_BARCODE_TYPE;
//==============================================================================
#define MAXBARCODELEN   	26
#define MAXMORENHIGH   		162   // 条码默认高度
#define MAXMORENWEIGH    	2     // 条码默认放大倍数
//==============================================================================
extern p_Prn_Barcode   pPrnBarcode;
void PrnBarCodeInit(void);
extern void PrnOneBarcodeSet(u8 *barcode);
//==============================================================================
#endif
