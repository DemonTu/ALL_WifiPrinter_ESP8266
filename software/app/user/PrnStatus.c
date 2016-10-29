#include "includes.h"


static uint8_t printerStatus = 0;

/**刷新打印机的状态**/
#define	_REFRESH_PRINTERSTATUS()	(printerStatus = (											\
									(_PRINTER_OVERHEAT()<<6)		|		/**过热**/			\
									((PaperStatus & NOPAPER)<<2)	|		/**缺纸**/			\
									(strprnprop.printenable<<0)				/**连接机芯**/		\
									))	// tqy

/**获取打印机的状态**/
#define	_GET_PRINTERSTATUS()		(printerStatus)
/*******************************************************************************
* Function Name  : AutoSendPrinterStatus
* Description    : 上传打印机状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
SendPrinterStatus(void)	// tqy
{
	_REFRESH_PRINTERSTATUS();
	USART_To_USB_Send_Byte(_GET_PRINTERSTATUS());
}


/*******************************************************************************
*函数功能: 打印状态
*输入参数: 无
*输出参数: 无
*返回值  : 无
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrnStatusHandle(void)
{
	static uint8_t bPrintWarn = 0;
	if( _PRINTER_OVERHEAT() )
	{
		if( (bPrintWarn==0) )
		{
			bPrintWarn = 1;
			// 过热
		}
	}
	else
	{
		bPrintWarn = 0;
	}
	if( strprnprop.autostatus & BIT(2) )	/**是否允许自动上传状态**/
	{
		uint8_t temp;
		temp = _GET_PRINTERSTATUS();
		_REFRESH_PRINTERSTATUS();
		if( temp != _GET_PRINTERSTATUS() )
		{
			USART_To_USB_Send_Byte(_GET_PRINTERSTATUS());
		}
	}
}


