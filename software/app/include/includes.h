#ifndef __INCLUDES_H
	#define __INCLUDES_H
	
#include "ets_sys.h"
#include "osapi.h"
#include "driver/spi.h"

#include "user_interface.h"

#include "font.h"
#include "swTool.h"

// 打印机相关文件
#include "PrnWifi_IO.h"

#include "PrnCommon.h"
#include "PrnCommonRam.h"

//#include "prn_syspara.h"
//#include "prn_ad.h"
#include "Prn_Driver.h"

#include "prn_thermal.h"
//#include "prn_led.h"

//#include "PrnGetbufHandle_drv.h"
#include "PrnGetbufHandle.h"

#include "PrnDotBufRing_drv.h"
#include "PrnDotBufRing.h"

#include "PrnCharHandle_drv.h"
#include "PrnCharHandle.h"

#include "PrnESCHandle_drv.h"
#include "PrnESCHandle.h"

#include "PrnBarcode_drv.h"
#include "PrnBarcode.h"

#include "PrnBmpHandle.h"

#if INCLUDE_FENDUAN
#include "MulHeat.h"
#endif
#include "PrnStatus.h"

//-----------------------------------------------------------------------------
#include "LoadSet.h"
//-----------------------------------------------------------------------------

#include "UserFlashProcessAPI.h"


#define	NOPAPER					0x01		/* 缺纸 */
#define	NOSCROLL				0x02		/* 缺纸卷 */
#define	NOPRESSSW				0x04		/* 开盖 */

#define RBUF_NEXT_PT(dat, offset, size)     (((dat) + (offset)) & ((size) - 1))
//===========================================================================
typedef enum
{
    OP_ERROR,
    OP_OK,
}ENUM_OP_STATE;

#endif	
