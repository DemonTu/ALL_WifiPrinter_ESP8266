#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define LEWEI_PLATFORM      0

#define USE_OPTIMIZE_PRINTF

#ifndef CONDITION
#define CONDITION					2
#endif

#define	YES							1
#define	NO							0

#define MACHINE_PP9M3				0
#define MACHINE_TP27M3				1
#define MACHINE_TP28M3				2
#define MACHINE_TP2JM3				3
#define MACHINE_TP2VM3				4
#define MACHINE_WIFI				5


#define MACHINE_TYPE				MACHINE_WIFI

#define MODIFY_DATE					"2016/09/10"			/* 修改日期 */

//-----------------------------------------------------------------------------
#define	USB_HID						0						/* HID */
#define USB_VIRTUAL                 1                       /* 虚拟串口 */
#define USB_PRINTER                 2                       /* 定义采样USB打印类协议 */

#define INCLUDE_USB          		USB_PRINTER				/* USB类型 */
//#define INCLUDE_USB_DEVICE_ID		// 是否发送USB DEVICE_ID
//-----------------------------------------------------------------------------
#define VER_VERSION                 0x1000                  /* 软件版本号 */
#define VER_YEAR                    0x2016                  /* 年 */
#define VER_MONTH                   0x09                    /* 月 */
#define VER_DAY                     0x10                    /* 日 */
//-----------------------------------------------------------------------------
#define HARD_VER                    0x00040001              /* 硬件版本 */
///////////////////////////////////////////////////////////////////////////////
#define	_USE_CODEFLASH_FONTDOT		NO						/* 是否使用Code中的字库 */

///////////////////////////////////////////////////////////////////////////////
#if 	(CONDITION == 1)			//2寸间歇多国
#define	MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* 是否分段加热 */
#define	TP34_HEAD_EN				NO						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 2)			//2寸间歇中文
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* 是否分段加热 */
#define	TP34_HEAD_EN				NO						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 3)			//3寸间歇中文
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* 是否分段加热 */
#define	TP34_HEAD_EN				YES						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 4)			//3寸间歇多国
#define	MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* 是否分段加热 */
#define	TP34_HEAD_EN				YES						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 5)			//2寸分段中文
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* 是否分段加热 */
#define	TP34_HEAD_EN				NO						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 6)			// 2寸分段多国
#define	MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* 是否分段加热 */
#define	TP34_HEAD_EN				NO						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 7)			//3寸分段中文
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* 是否分段加热 */
#define	TP34_HEAD_EN				YES						/* 是否使用TP34打印头 */

#elif 	(CONDITION == 8)			// 3寸分段多国
#define	MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* 是否分段加热 */
#define	TP34_HEAD_EN				YES						/* 是否使用TP34打印头 */

#else
#define	MUL_LANG					/* 多国语言 */
#define	INCLUDE_FENDUAN				YES
#define	TP34_HEAD_EN				NO						/* 是否使用TP34打印头 */

#endif

///////////////////////////////////////////////////////////////////////////////
#if MACHINE_TYPE == MACHINE_WIFI
// wifi ptinter ESP8266
#define	_MACHINE_TYPE_STRING		"MACHINE_WP8266 "__SOFTVER
#define	__SOFTVER					"v1.000"

#define	INCLUDE_SETTING				YES			/* 参数 */
#define	INCLUDE_SPI1				NO			/* SP1(FLASH) */
#define INCLUDE_SERIAL_FLASH		YES			/* 串行FLASH */
#define	INCLUDE_DRAWER1				NO			/* 钱箱1 */
#define	INCLUDE_DRAWER2				NO			/* 钱箱2 */
#define	INCLUDE_PAPER_SCROLL		NO			/* 纸卷侦测 */
#define	INCLUDE_CUTPAPER			NO			/* 切刀 */
#define	INCLUDE_BUZZER				NO			/* 蜂鸣器使能 */
#define	INCLUDE_SET_USBTYPE			NO			/* 参数设置USB类型 */
#define INCLUDE_TP_DET				NO			/* 打印头自动识别 */
#define	INCLUDE_PLATENSW			NO			/* 轴到位侦测(开盖) */
#define INCLUDE_POWER_AD			NO			/* 电源侦测 */


#define DEFAULT_12V_HEAT_PW			100			/* 12v打印头加热能量 */
#define DEFAULT_7V_HEAT_PW			80			/* 7v打印头加热能量 */

#define PRN_STB_ACTIVE_LOW			YES			/* STB低有效 */
//#define PRN_MOTOR_DIR_B				YES			/* 电机马达正方向为负 */
#else
#define	_MACHINE_TYPE_STRING		"MACHINE_WP8266 "__SOFTVER
#define	__SOFTVER					"v1.000"

#define	INCLUDE_SETTING				YES			/* 参数 */
#define	INCLUDE_SPI1				YES			/* SP1(FLASH) */
#define INCLUDE_SERIAL_FLASH		YES			/* 串行FLASH */
#define	INCLUDE_DRAWER1				NO			/* 钱箱1 */
#define	INCLUDE_DRAWER2				NO			/* 钱箱2 */
#define	INCLUDE_PAPER_SCROLL		NO			/* 纸卷侦测 */
#define	INCLUDE_CUTPAPER			NO			/* 切刀 */
#define	INCLUDE_BUZZER				NO			/* 蜂鸣器使能 */
#define	INCLUDE_SET_USBTYPE			YES			/* 参数设置USB类型 */
#define INCLUDE_TP_DET				YES			/* 打印头自动识别 */
#define	INCLUDE_PLATENSW			NO			/* 轴到位侦测(开盖) */
#define INCLUDE_POWER_AD			YES			/* 电源侦测 */


#define DEFAULT_12V_HEAT_PW			100			/* 12v打印头加热能量 */
#define DEFAULT_7V_HEAT_PW			80			/* 7v打印头加热能量 */

#endif

///////////////////////////////////////////////////////////////////////////////
/* 电机每走0.125mm需要走的步数 */
#define	MOTOR_STEP_COUNT			4

// 定义打印头宽度
#if TP34_HEAD_EN
#define PrnDotOfLine  				(PrnDotOfByte*8)
#define PrnDotOfByte				72
#define TOTALBIT        			(PrnDotOfByte * 8)
#define DotBufHigh    				64						/* 点阵高 */

#else
#define PrnDotOfLine  				(PrnDotOfByte*8)
#define PrnDotOfByte				48
#define TOTALBIT        			(PrnDotOfByte * 8)
#define DotBufHigh    				64						/* 点阵高 */
#endif


#endif

