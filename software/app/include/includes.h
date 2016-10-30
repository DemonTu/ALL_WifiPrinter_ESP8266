#ifndef __INCLUDES_H
	#define __INCLUDES_H
//===========================================================================
typedef enum
{
    OP_ERROR,
    OP_OK,
}ENUM_OP_STATE;	
//===========================================================================	
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"

#include "user_interface.h"

#include "driver/spi.h"
#include "driver/Hw_timer.h"

#include "font.h"
#include "swTool.h"

// ��ӡ������ļ�
#include "PrnWifi_IO.h"

#include "PrnCommon.h"
#include "PrnCommonRam.h"

//#include "prn_syspara.h"
//#include "prn_ad.h"
#include "Prn_Driver.h"

#include "prn_thermal.h"

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

//-----------------------------------------------------------------------------
// ����
#include "PrnTestPage.h"

#include "PrnStatus.h"

//-----------------------------------------------------------------------------
#include "LoadSet.h"
//-----------------------------------------------------------------------------

// user device
#include "UserFlashProcessAPI.h"
#include "UserKeyDeviceAPI.h"
#include "userSensorDetection.h"
#include "user_devicefind.h"



#define	NOPAPER					0x01		/* ȱֽ */
#define	NOSCROLL				0x02		/* ȱֽ�� */
#define	NOPRESSSW				0x04		/* ���� */

#define RBUF_NEXT_PT(dat, offset, size)     (((dat) + (offset)) & ((size) - 1))
//===========================================================================

#endif	
