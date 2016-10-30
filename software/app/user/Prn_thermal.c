#include "includes.h"

#if !INCLUDE_FENDUAN						/* 分段 */
///////////////////////////////////////////////////////////////////
#if HEAT_2TIMES
u16 BASEHEATTIME = 400;
static uint16_t HeatTime = 400;
#else
#if MOTOR_STEP_COUNT == 2	// 步进电机步数
u16 BASEHEATTIME = 260;
static uint16_t HeatTime = 260;
#else
u16 BASEHEATTIME = 120;
static uint16_t HeatTime = 120;
#endif
#endif

static uint16_t CoolTime = BASECOOLTIME;
static uint16_t StepHeatTm;
static uint8_t PrnStepUp = 0;


static void MotorStep(void);

//////////////////////////////////////////////////////////////////
enum
{
    _MODE_IDLE = 0,    	/*  0  */
    _MODE_MOTOR_STEP, 	/*  1  */
    _MODE_START_PRINT,	/*  2  */
    _MODE_COOL_STEP,   	/*  3  */
    _MODEOSCHEAT,
    _MODE_START_DELAY,
    MODE_MAX
};
//////////////////////////////////////////////////////////////////

/**加速表**/
u16 const  MotorStepTbl[]  =
{
	/*9000,8000,7000,6000,5000,*/4000,3000,2000,2500,2200,		/* 1 */
	2000,1900,1800,1700,1600,1500,1400,1300,1200,1000,		/* 2 */
	907,837,780,734,695,661,632,607,584,564,				/* 3 */
	546,529,514,500,487,475,464,454,444,435,				/* 4 */
	426,418,411,403,397,390,384,378,372,367,				/* 5 */
	362,357,352,347,343,339,335,331,327,323,				/* 6 */
	320,316,313,310,307,303,301,298,295,292,				/* 7 */
	290,287,284,282,280,277,275,273,271,269,				/* 8 */
	267,265,263,261,259,257,255,253,252,250,				/* 9 */
	248,247,245,243,241,239,237,235,233,231,				/* 10 */
	229,227,225,223,221,219,217,215,213,211,				/* 11 */
	209,207,205,203,201,199,197,195,193,191,				/* 12 */
	189,187,185,183,181,179,178,176,175,173,				/* 13 */
	171,169,167,165,163,161,159,157,155,153,				/* 14 */
	0,0		/* 结束 */
};

#if MOTOR_STEP_COUNT == 2
#define MOTOR_STEP(n)		(MotorStepTbl[n] * 2)
#else
#define MOTOR_STEP(n)		(MotorStepTbl[n])
#endif

/*******************************************************************************
* Function Name  : HeatTimeInit
* Description    : 计算加热参数
* Input          : us
* Output         : None
* Return         : None
*******************************************************************************/
extern void ICACHE_FLASH_ATTR
HeatTimeInit(void)
{
	uint16_t HeatTimeTmp;
	uint16_t CoolTimeTmp;

	HeatTimeTmp = BASEHEATTIME *(6+(strprnprop.density*2))/10;					/**默认加热时间**/
	if (HeatTimeTmp < STEP_SPEED(PrnDrv.Prn_MaxSpeed))	CoolTimeTmp = STEP_SPEED(PrnDrv.Prn_MaxSpeed) - HeatTimeTmp;
	else	CoolTimeTmp = 10;					/**默认冷却时间**/

	HeatTime = HeatTimeTmp;
	CoolTime = CoolTimeTmp;

}

/*******************************************************************************
* Function Name  : PaperStatusCheck
* Description    : 缺纸状态侦测。(1ms执行一次)
* Input          : us
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
PaperStatusCheck(void)
{
	static u8 statusTmp = 0;
	static u8 statCnt = 0;
	static u8 paperState = 0;
	u8 tempStat = 0;
	switch ((++paperState) % 10)
	{
		case 0:
			POWER_ON_SENSOR();
			break;
		case 2:
			/***********缺纸侦测************/
			if (_ISLACKOFPAPER())			tempStat |= NOPAPER;
			else							tempStat &= ~NOPAPER;
#if INCLUDE_PAPER_SCROLL
			if (_ISLACKOFPAPERSCROLL())		tempStat |= NOSCROLL;
			else							tempStat &= ~NOSCROLL;
#endif
			if (statusTmp == tempStat)
			{
				if (++statCnt == 5)
				{
					statCnt = 0;
					PaperStatus = statusTmp;
				}
			}
			else
			{
				statCnt = 0;
			}
			statusTmp = tempStat;
#if INCLUDE_CUTPAPER
			if (bStartCutPaper == FALSE)
#endif
				POWER_OFF_SENSOR();
			break;
		default:
			break;
	}
}


/*******************************************************************************
* Function Name  : PrintIdleFun
* Description    : idel 处理流程
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static ICACHE_FLASH_ATTR
void PrintIdleFun(void)
{
    static u8 prnstage = 0;
    switch (++prnstage)
    {
        case 1:  /* 关闭步进马达电源 */
        	DISABLE_MOTOR();
            break;
        case 2:  /* 判断有无打印任务 */
            if (_PrnBufHasData())
            {
				HeatTimeInit();
				/* 有数据要打印 */
                PrintMode = _MODE_START_PRINT;
                PrnStepUp = 0;
				ENABLE_MOTOR();
				StepHeatTm = HeatTime * 2;
				#if MOTOR_STEP_COUNT == 4
				StepHeatTm = HeatTime * 2;
				#endif

				MotorStep();		// RC保护电路， 增加扰动
				PrintMode = _MODE_START_DELAY;
            }
            break;
		case 3:
			break;
		case 4:
			break;
        default:
            prnstage = 0;
            break;
    }
    NextT1Timer = FEED_INTERVAL;
}

/*******************************************************************************
* Function Name  : ThermSeri
* Description    : 打印头送数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t TP3_TmpDotBuf[PrnDotOfByte];

static void ICACHE_FLASH_ATTR
ThermSeri(void)
{

	os_memcpy(TP3_TmpDotBuf,&DotBuffer[LDotReadPtr][0],PrnDotOfByte);

	LDotReadPtr = RBUF_NEXT_PT(LDotReadPtr,1,DotBufHigh);
}

/*******************************************************************************
* Function Name  : MotorStep
* Description    : 走纸步进
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ICACHE_FLASH_ATTR
MotorStep(void)
{
	static uint8_t bFlag = 0;

	if (bFlag == 0)
	{
		bFlag = 1;
		MotorClkLow();
	}
	else
	{
		bFlag = 0;
		MotorClkHigh();
	}
}

/*******************************************************************************
* Function Name  : Prn_Time_Int
* Description    : 打印中断定时服务函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ICACHE_FLASH_ATTR
Prn_Time_Int(void)
{
	u16 temp;
	
	if (NextT1Timer < 10) NextT1Timer = 10;
    hw_timer_arm(NextT1Timer - 1);
	/***************************************************/
	StopDst();
	switch(PrintMode)
	{
       case _MODE_IDLE:
			PrintIdleFun();			/* idle 走纸流程 */
			break;
		case _MODE_START_DELAY:
			
			MotorStep();		// RC保护电路， 增加扰动
			PrintMode = _MODE_START_PRINT;

			break;

		case _MODE_START_PRINT:
			ThermSeri();
			_NEXT_PRINT_MODE(_MODE_COOL_STEP, _MODE_COOL_STEP, StepHeatTm);
           break;
		case _MODE_MOTOR_STEP:		/* 加热走步 */
			LatchData();

			StartDst(); 			/**开始加热**/
			MotorStep();
			if (MOTOR_STEP(PrnStepUp) > (HeatTime + CoolTime))	/* 加速区 */
			{
				PrnStepUp ++;
			}
			temp = MOTOR_STEP(PrnStepUp);
			if (temp > (HeatTime + CoolTime))	NextT1Timer = temp - HeatTime;
			else								NextT1Timer = CoolTime;

			PrintMode = BakPrintMode;
			if (_PrnBufHasData())			/* 有数据要打印 */
			{
				ThermSeri();				/* 送数据 */
				if (StepHeatTm > (HeatTime + 10))	StepHeatTm -= 10;
				else						StepHeatTm = HeatTime;
			}
			else
			{
				PrintMode = _MODE_IDLE;
				NextT1Timer = FEED_INTERVAL1;
			}
			break;
		case _MODE_COOL_STEP:
			/* 判断spi数据是否已经发送完 */
		//	if (DMA_GetFlagStatus(SPI_MASTER_Tx_DMA_FLAG))	 tqy
			{
				_NEXT_PRINT_MODE(_MODE_COOL_STEP, _MODE_MOTOR_STEP, StepHeatTm);
				MotorStep();
			}
			break;
		default:
			PrintMode = _MODE_IDLE;
			break;
	}
}

#endif

