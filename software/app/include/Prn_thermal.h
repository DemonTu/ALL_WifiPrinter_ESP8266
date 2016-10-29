#ifndef __PRN_THERMAL_H__
 #define __PRN_THERMAL_H__
///////////////////////////////////////////////////////////////////

#if !INCLUDE_FENDUAN				/* 分段 */

#define HEAT_2TIMES					NO		// 分2次加热
///////////////////////////////////////////////////////////////////
#if HEAT_2TIMES
extern u16 BASEHEATTIME;
//#define BASEHEATTIME				250					/**默认加热时间**/
#else
extern u16 BASEHEATTIME;
//#define BASEHEATTIME				100					/**默认加热时间**/
#endif
#define BASECOOLTIME	 			10 					/**默认冷却时间**/

#define	MOTOR_CLK_LOW_TIME			100
#define MOTER_FEED_SPEED    		600					/* 走纸速度 */

#define FEED_INTERVAL       		5000    			/* 走纸间隔时间 */
#define FEED_INTERVAL1      		1000    			/* 打印走纸间隔时间 */

#define MtSetUpSpeedCnt     		(sizeof(MotorStepTbl) / sizeof(u16))

/* 检查打印缓冲区是否为空 */
#define	_PrnBufIsEmpty()			(LDotReadPtr == LDotWritePtr)
#define	_PrnBufHasData()			(LDotReadPtr != LDotWritePtr)

/* 电机每步时间 */
#define	STEP_SPEED(speed)			(125000/speed/MOTOR_STEP_COUNT)

/* 电机走step步(每MOTOR_STEP_COUNT步0.125mm) */
#define	_MOTOR_FEED_STEP(step)		PutBlackLine(step)	
/* 电机走mm_125个单位(每个单位0.125mm) */
#define	_MOTOR_FEED_UNIT(mm_125)	PutBlackLine(MOTOR_STEP_COUNT*(mm_125))
/* 电机走mm毫米 */
#if MOTOR_STEP_COUNT == 2
#define	_MOTOR_FEED_MM(mm)			PutBlackLine(8/2*MOTOR_STEP_COUNT*(mm))
#else
#define	_MOTOR_FEED_MM(mm)			PutBlackLine(8/4*MOTOR_STEP_COUNT*(mm))
#endif

/* 走步模式 */
#define	_NEXT_PRINT_MODE(BACKMODE,NEXTMODE,NEXTTIME)	(BakPrintMode = BACKMODE,PrintMode = NEXTMODE,NextT1Timer = NEXTTIME)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////
extern void Prn_Time_Int(void);
extern void PaperStatusCheck(void);
extern void HeatTimeInit(void);

#endif
#endif
