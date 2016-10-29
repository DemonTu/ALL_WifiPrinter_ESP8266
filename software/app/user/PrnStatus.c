#include "includes.h"


static uint8_t printerStatus = 0;

/**ˢ�´�ӡ����״̬**/
#define	_REFRESH_PRINTERSTATUS()	(printerStatus = (											\
									(_PRINTER_OVERHEAT()<<6)		|		/**����**/			\
									((PaperStatus & NOPAPER)<<2)	|		/**ȱֽ**/			\
									(strprnprop.printenable<<0)				/**���ӻ�о**/		\
									))	// tqy

/**��ȡ��ӡ����״̬**/
#define	_GET_PRINTERSTATUS()		(printerStatus)
/*******************************************************************************
* Function Name  : AutoSendPrinterStatus
* Description    : �ϴ���ӡ��״̬
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
*��������: ��ӡ״̬
*�������: ��
*�������: ��
*����ֵ  : ��
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
			// ����
		}
	}
	else
	{
		bPrintWarn = 0;
	}
	if( strprnprop.autostatus & BIT(2) )	/**�Ƿ������Զ��ϴ�״̬**/
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


