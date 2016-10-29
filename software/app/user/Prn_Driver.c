#include "includes.h"
/*
#include "ets_sys.h"
#include "osapi.h"
#include "driver/spi.h"

#include "user_interface.h"
//#include "user_config.h"

#include "PrnWifi_IO.h"
#include "Prn_Driver.h"
*/
#define TIMING1_PERIOD			500              /* 5ms */
//=====================  打印驱动 发送数据 ============================================
#if INCLUDE_FENDUAN
u8  DotBuffer[DotBufHigh][PrnDotOfByte+sizeof(PrnHeatParaStruct)];
#else
u8  DotBuffer[DotBufHigh][PrnDotOfByte];
#endif

u32 LDotReadPtr,LDotWritePtr;

u16 NextT1Timer;       				/* 定时器1时间暂存器 */
u8  PrintMode;
u8  BakPrintMode;
u8  PaperStatus;


void ICACHE_FLASH_ATTR
InitPrintPare(void)
{
    PrnEscInit();
    PrnCharInit();
}

/*******************************************************************************
* Function Name  : PrnSpiInit
* Description    : spi初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrnSpiInit(void)
{
	spi_master_init(HSPI);
}

/*******************************************************************************
* Function Name  : PrnGpioInit
* Description    : AD Io初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrnGpioInit(void)
{
	// GPIO init
	/* LATCH 加热叫 */
	PIN_PULLUP_DIS(PRN_LH_IO_MUX);
	PIN_FUNC_SELECT(PRN_LH_IO_MUX, PRN_LH_IO_FUNC);	

	PIN_PULLUP_DIS(SENSOR_ON_IO_MUX);
	PIN_FUNC_SELECT(SENSOR_ON_IO_MUX, SENSOR_ON_IO_FUNC);	

	PIN_PULLUP_EN(PRN_STB_IO_MUX);
	PIN_FUNC_SELECT(PRN_STB_IO_MUX, PRN_STB_IO_FUNC);	

	//PIN_PULLUP_DIS(PRN_MT_DIR_IO_MUX);
	//PIN_FUNC_SELECT(PRN_MT_DIR_IO_MUX, PRN_MT_DIR_IO_FUNC);	

	PIN_PULLUP_DIS(PRN_STEP_IO_MUX);
	PIN_FUNC_SELECT(PRN_STEP_IO_MUX, PRN_STEP_IO_FUNC);	

	PIN_PULLUP_DIS(PRN_EN_IO_MUX);
	PIN_FUNC_SELECT(PRN_EN_IO_MUX, PRN_EN_IO_FUNC);	
}


/*******************************************************************************
* Function Name  : Time1Init
* Description    : 定时器1定时做 加热时间中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ICACHE_FLASH_ATTR
Time1Init(void)
{
	hw_timer_init(NMI_SOURCE, 0);
	hw_timer_set_func(Prn_Time_Int);
	hw_timer_arm(TIMING1_PERIOD);
}

//函数  切换打印机的连线
//输入  value   0:关闭打印机 1:启动打印机 0xff:切换打印机 0xfe:设定不变
//输出  无
//返回  无
void ICACHE_FLASH_ATTR
TogglePrintOnLine(unsigned char value)
{
    if (value == 0xfe)
    {
        strprnprop.printenable = strprnprop.printenable;
    }
    else if (value == 0xff)
    {
        strprnprop.printenable = 1 - strprnprop.printenable;
    }
    else
    {
        strprnprop.printenable = value;
    }
}

void ICACHE_FLASH_ATTR
InitPrint(void)
{
    InitPrintPare(); 		/* 打印机的参数初始化程序 */
}

/*******************************************************************************
* Function Name  : PrnDriverInit
* Description    : driver初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PrnDriverInit(void)
{
	StopDst();
    DISABLE_MOTOR();	/* 关闭步进马达驱动IC片选端 */

	MOTOR_DIR_B();	//tqy
	//MOTOR_DIR_F();

    PrnSpiInit();
    Time1Init();

    NextT1Timer = 1000;

    LDotReadPtr = 0;
    LDotWritePtr = 0;
    PrintMode = 0;
    PaperStatus = 0;
	BakPrintMode = 0;
//===========================================================================
	// tqy
	PrnDrv.Prn_Rave = 17600;		/* 打印机电阻 *10毫欧姆 */
	PrnDrv.Prn_Ric = 1500;			/* 内阻 *10毫欧姆 */
	PrnDrv.Prn_Rcom = 5;			/* 每个点增加的电阻 *10毫欧姆 */
	PrnDrv.Prn_Power = DEFAULT_7V_HEAT_PW;			/* 加热能量 */
	PrnDrv.Prn_MaxSpeed = 80;
//===========================================================================	
	InitPrint();
    POWER_OFF_SENSOR();
}

