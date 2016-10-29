#ifndef __PRN_DRIVER_H__
#define __PRN_DRIVER_H__

typedef  struct{
	u16 HeatCntDotsNum;
	u16 CurLineHeatTm;		// ����ʱ��
	u16 CurLineAllTm;		// ��ʱ�䣬�ٶ�
}PrnHeatParaStruct;

typedef struct{
	u16 Prn_Rave;                   // ��ӡ������
    u16 Prn_Ric;                    // ����
    u16 Prn_Rcom;                   // ÿ�������ӵĵ���
    u16	Prn_Power;					// ��������
    u16	Prn_MaxSpeed;
}ALSTR_PRN_INITARG;

// LATCH ����
#define LatchLow()      			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_LH_IO_UNM),0)
#define LatchHigh()     			GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_LH_IO_UNM),1)

#define LatchData()    				(LatchLow(),LatchHigh())

#if PRN_STB_ACTIVE_LOW	// STB����Ч
#define PRINTER_STB_ON()      		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),0)
#define PRINTER_STB_OFF()    		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),1)
#else
#define PRINTER_STB_ON()    		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),1)
#define PRINTER_STB_OFF()      		GPIO_OUTPUT_SET(GPIO_ID_PIN(PRN_STB_IO_UNM),0)
#endif

#define	StartDst()					PRINTER_STB_ON()
#define	StopDst()					PRINTER_STB_OFF()
/* ������﷽��� ��֧�� */
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

#if INCLUDE_DRAWER1					// û��Ǯ��
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
#define	OpenDrawer2()				// ����
#define	CloseDrawer2()

#endif

////////////////////////////////////////////////////////////////////////////////
#if INCLUDE_FENDUAN
extern u8  DotBuffer[DotBufHigh][PrnDotOfByte+sizeof(PrnHeatParaStruct)];
#else
extern u8  DotBuffer[DotBufHigh][PrnDotOfByte];
#endif
extern u32 LDotReadPtr,LDotWritePtr;

extern u16 NextT1Timer;    			/* ��ʱ��1ʱ���ݴ��� */
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

