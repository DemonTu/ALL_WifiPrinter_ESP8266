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

#define MODIFY_DATE					"2016/09/10"			/* �޸����� */

//-----------------------------------------------------------------------------
#define	USB_HID						0						/* HID */
#define USB_VIRTUAL                 1                       /* ���⴮�� */
#define USB_PRINTER                 2                       /* �������USB��ӡ��Э�� */

#define INCLUDE_USB          		USB_PRINTER				/* USB���� */
//#define INCLUDE_USB_DEVICE_ID		// �Ƿ���USB DEVICE_ID
//-----------------------------------------------------------------------------
#define VER_VERSION                 0x1000                  /* ����汾�� */
#define VER_YEAR                    0x2016                  /* �� */
#define VER_MONTH                   0x09                    /* �� */
#define VER_DAY                     0x10                    /* �� */
//-----------------------------------------------------------------------------
#define HARD_VER                    0x00040001              /* Ӳ���汾 */
///////////////////////////////////////////////////////////////////////////////
#define	_USE_CODEFLASH_FONTDOT		NO						/* �Ƿ�ʹ��Code�е��ֿ� */

///////////////////////////////////////////////////////////////////////////////
#if 	(CONDITION == 1)			//2���Ъ���
#define	MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				NO						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 2)			//2���Ъ����
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				NO						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 3)			//3���Ъ����
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				YES						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 4)			//3���Ъ���
#define	MUL_LANG
#define	INCLUDE_FENDUAN				NO						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				YES						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 5)			//2��ֶ�����
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				NO						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 6)			// 2��ֶζ��
#define	MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				NO						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 7)			//3��ֶ�����
#undef  MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				YES						/* �Ƿ�ʹ��TP34��ӡͷ */

#elif 	(CONDITION == 8)			// 3��ֶζ��
#define	MUL_LANG
#define	INCLUDE_FENDUAN				YES						/* �Ƿ�ֶμ��� */
#define	TP34_HEAD_EN				YES						/* �Ƿ�ʹ��TP34��ӡͷ */

#else
#define	MUL_LANG					/* ������� */
#define	INCLUDE_FENDUAN				YES
#define	TP34_HEAD_EN				NO						/* �Ƿ�ʹ��TP34��ӡͷ */

#endif

///////////////////////////////////////////////////////////////////////////////
#if MACHINE_TYPE == MACHINE_WIFI
// wifi ptinter ESP8266
#define	_MACHINE_TYPE_STRING		"MACHINE_WP8266 "__SOFTVER
#define	__SOFTVER					"v1.000"

#define	INCLUDE_SETTING				YES			/* ���� */
#define	INCLUDE_SPI1				NO			/* SP1(FLASH) */
#define INCLUDE_SERIAL_FLASH		YES			/* ����FLASH */
#define	INCLUDE_DRAWER1				NO			/* Ǯ��1 */
#define	INCLUDE_DRAWER2				NO			/* Ǯ��2 */
#define	INCLUDE_PAPER_SCROLL		NO			/* ֽ����� */
#define	INCLUDE_CUTPAPER			NO			/* �е� */
#define	INCLUDE_BUZZER				NO			/* ������ʹ�� */
#define	INCLUDE_SET_USBTYPE			NO			/* ��������USB���� */
#define INCLUDE_TP_DET				NO			/* ��ӡͷ�Զ�ʶ�� */
#define	INCLUDE_PLATENSW			NO			/* �ᵽλ���(����) */
#define INCLUDE_POWER_AD			NO			/* ��Դ��� */


#define DEFAULT_12V_HEAT_PW			100			/* 12v��ӡͷ�������� */
#define DEFAULT_7V_HEAT_PW			80			/* 7v��ӡͷ�������� */

#define PRN_STB_ACTIVE_LOW			YES			/* STB����Ч */
//#define PRN_MOTOR_DIR_B				YES			/* ������������Ϊ�� */
#else
#define	_MACHINE_TYPE_STRING		"MACHINE_WP8266 "__SOFTVER
#define	__SOFTVER					"v1.000"

#define	INCLUDE_SETTING				YES			/* ���� */
#define	INCLUDE_SPI1				YES			/* SP1(FLASH) */
#define INCLUDE_SERIAL_FLASH		YES			/* ����FLASH */
#define	INCLUDE_DRAWER1				NO			/* Ǯ��1 */
#define	INCLUDE_DRAWER2				NO			/* Ǯ��2 */
#define	INCLUDE_PAPER_SCROLL		NO			/* ֽ����� */
#define	INCLUDE_CUTPAPER			NO			/* �е� */
#define	INCLUDE_BUZZER				NO			/* ������ʹ�� */
#define	INCLUDE_SET_USBTYPE			YES			/* ��������USB���� */
#define INCLUDE_TP_DET				YES			/* ��ӡͷ�Զ�ʶ�� */
#define	INCLUDE_PLATENSW			NO			/* �ᵽλ���(����) */
#define INCLUDE_POWER_AD			YES			/* ��Դ��� */


#define DEFAULT_12V_HEAT_PW			100			/* 12v��ӡͷ�������� */
#define DEFAULT_7V_HEAT_PW			80			/* 7v��ӡͷ�������� */

#endif

///////////////////////////////////////////////////////////////////////////////
/* ���ÿ��0.125mm��Ҫ�ߵĲ��� */
#define	MOTOR_STEP_COUNT			4

// �����ӡͷ���
#if TP34_HEAD_EN
#define PrnDotOfLine  				(PrnDotOfByte*8)
#define PrnDotOfByte				72
#define TOTALBIT        			(PrnDotOfByte * 8)
#define DotBufHigh    				64						/* ����� */

#else
#define PrnDotOfLine  				(PrnDotOfByte*8)
#define PrnDotOfByte				48
#define TOTALBIT        			(PrnDotOfByte * 8)
#define DotBufHigh    				64						/* ����� */
#endif


#endif

