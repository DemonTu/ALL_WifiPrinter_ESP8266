#ifndef __PRN_THERMAL_H__
 #define __PRN_THERMAL_H__
///////////////////////////////////////////////////////////////////

#if !INCLUDE_FENDUAN				/* �ֶ� */

#define HEAT_2TIMES					NO		// ��2�μ���
///////////////////////////////////////////////////////////////////
#if HEAT_2TIMES
extern u16 BASEHEATTIME;
//#define BASEHEATTIME				250					/**Ĭ�ϼ���ʱ��**/
#else
extern u16 BASEHEATTIME;
//#define BASEHEATTIME				100					/**Ĭ�ϼ���ʱ��**/
#endif
#define BASECOOLTIME	 			10 					/**Ĭ����ȴʱ��**/

#define	MOTOR_CLK_LOW_TIME			100
#define MOTER_FEED_SPEED    		600					/* ��ֽ�ٶ� */

#define FEED_INTERVAL       		5000    			/* ��ֽ���ʱ�� */
#define FEED_INTERVAL1      		1000    			/* ��ӡ��ֽ���ʱ�� */

#define MtSetUpSpeedCnt     		(sizeof(MotorStepTbl) / sizeof(u16))

/* ����ӡ�������Ƿ�Ϊ�� */
#define	_PrnBufIsEmpty()			(LDotReadPtr == LDotWritePtr)
#define	_PrnBufHasData()			(LDotReadPtr != LDotWritePtr)

/* ���ÿ��ʱ�� */
#define	STEP_SPEED(speed)			(125000/speed/MOTOR_STEP_COUNT)

/* �����step��(ÿMOTOR_STEP_COUNT��0.125mm) */
#define	_MOTOR_FEED_STEP(step)		PutBlackLine(step)	
/* �����mm_125����λ(ÿ����λ0.125mm) */
#define	_MOTOR_FEED_UNIT(mm_125)	PutBlackLine(MOTOR_STEP_COUNT*(mm_125))
/* �����mm���� */
#if MOTOR_STEP_COUNT == 2
#define	_MOTOR_FEED_MM(mm)			PutBlackLine(8/2*MOTOR_STEP_COUNT*(mm))
#else
#define	_MOTOR_FEED_MM(mm)			PutBlackLine(8/4*MOTOR_STEP_COUNT*(mm))
#endif

/* �߲�ģʽ */
#define	_NEXT_PRINT_MODE(BACKMODE,NEXTMODE,NEXTTIME)	(BakPrintMode = BACKMODE,PrintMode = NEXTMODE,NextT1Timer = NEXTTIME)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////
extern void Prn_Time_Int(void);
extern void PaperStatusCheck(void);
extern void HeatTimeInit(void);

#endif
#endif
