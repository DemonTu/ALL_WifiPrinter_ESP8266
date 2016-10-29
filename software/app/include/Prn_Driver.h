#ifndef __PRN_DRIVER_H__
#define __PRN_DRIVER_H__

typedef  struct{
	u16 HeatCntDotsNum;
	u16 CurLineHeatTm;		// 加热时间
	u16 CurLineAllTm;		// 总时间，速度
}PrnHeatParaStruct;

typedef struct{
	u16 Prn_Rave;                   // 打印机电阻
    u16 Prn_Ric;                    // 内阻
    u16 Prn_Rcom;                   // 每个点增加的电阻
    u16	Prn_Power;					// 加热能量
    u16	Prn_MaxSpeed;
}ALSTR_PRN_INITARG;

// LATCH 数据
#define LatchLow()      			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_LH_IO_UNM),0)
#define LatchHigh()     			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_LH_IO_UNM),1)

#define LatchData()    				(LatchLow(),LatchHigh())

#if PRN_STB_ACTIVE_LOW	// STB低有效
#define PRINTER_STB_ON()      		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),0)
#define PRINTER_STB_OFF()    		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),1)
#else
#define PRINTER_STB_ON()    		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),1)
#define PRINTER_STB_OFF()      		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),0)
#endif

#define	StartDst()					PRINTER_STB_ON()
#define	StopDst()					PRINTER_STB_OFF()
/* 控制马达方向的 不支持 */
#define MOTOR_DIR_F() 				//GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),0)
#define MOTOR_DIR_B()     			//GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),1)

#define MotorClkLow()      			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_CLK_IO_UNM),0)
#define MotorClkHigh()     			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_CLK_IO_UNM),1)

#define DISABLE_MOTOR()       		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_EN_IO_UNM),0)
#define ENABLE_MOTOR()         		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_EN_IO_UNM),1)

// SENSOR_ON
#if SENSOR_ON_SIGACTIVE
#define POWER_OFF_SENSOR()   		GPIO_OUTPUT_SET(GPIO_ID_PIN(SENSOR_ON_IO_UNM),0)
#define POWER_ON_SENSOR() 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SENSOR_ON_IO_UNM),1)
#else
#define POWER_OFF_SENSOR()   		GPIO_OUTPUT_SET(GPIO_ID_PIN(SENSOR_ON_IO_UNM),1)
#define POWER_ON_SENSOR() 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SENSOR_ON_IO_UNM),0)
#endif

#if INCLUDE_DRAWER1					// 没有钱箱
#define	OpenDrawer1()				//GPIO_SetBits(DRAWER1_GPIO, DRAWER1)
#define	CloseDrawer1()				//GPIO_ResetBits(DRAWER1_GPIO, DRAWER1)
#else
#define	OpenDrawer1()
#define	CloseDrawer1()
#endif

#if INCLUDE_DRAWER2
#define	OpenDrawer2()				//GPIO_SetBits(DRAWER2_GPIO, DRAWER2)
#define	CloseDrawer2()				//GPIO_ResetBits(DRAWER2_GPIO, DRAWER2)
#else
#define	OpenDrawer2()				// 抽屉
#define	CloseDrawer2()

#endif

////////////////////////////////////////////////////////////////////////////////
#if INCLUDE_FENDUAN
extern u8  DotBuffer[DotBufHigh][PrnDotOfByte+sizeof(PrnHeatParaStruct)];
#else
extern u8  DotBuffer[DotBufHigh][PrnDotOfByte];
#endif
extern u32 LDotReadPtr,LDotWritePtr;

extern u16 NextT1Timer;    			/* 定时器1时间暂存器 */
extern u8  PrintMode;
extern u8  PaperStatus;
extern u8  BakPrintMode;

////////////////////////////////////////////////////////////////////////////////
extern void PrnGpioInit(void);
extern void PrnDriverInit(void);
extern void PrnSpiInit(void);
extern void CopyOneDotLineToPrn(u8 *buffer);
extern void TogglePrintOnLine(unsigned char value);
extern void InitPrint(void);
extern void InitPrintPare(void);
#endif

