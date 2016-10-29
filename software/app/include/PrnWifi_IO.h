#ifndef __PRNWIFI_IO_H
	#define __PRNWIFI_IO_H

//=====================================================================
//			打印数据驱动
//=====================================================================
// DAT
#define PRN_DAT_IO_MUX      		PERIPHS_IO_MUX_MTCK_U   /* SPI2 */
#define PRN_DAT_IO_UNM           	13
#define PRN_DAT_IO_FUNC				FUNC_GPIO13
	// CLK
#define PRN_CLK_IO_MUX       		PERIPHS_IO_MUX_MTMS_U
#define PRN_CLK_IO_UNM            	14
#define PRN_CLK_IO_FUNC				FUNC_GPIO14
	// LATCH
#define PRN_LH_IO_MUX       		PERIPHS_IO_MUX_MTDO_U
#define PRN_LH_IO_UNM             	15
#define PRN_LH_IO_FUNC				FUNC_GPIO15
	// STB
#define PRN_STB_IO_MUX       		PERIPHS_IO_MUX_MTDI_U
#define PRN_STB_IO_UNM           	12
#define PRN_STB_IO_FUNC				FUNC_GPIO12
	// DIR
#define PRN_MT_DIR_IO_MUX    		
#define PRN_MT_DIR_IO_UNM        	/* 步进转动方向 GPIO_Pin_9 */
#define PRN_MT_DIR_IO_FUNC
	// STEP
#define PRN_STEP_IO_MUX      		PERIPHS_IO_MUX_U0RXD_U
#define PRN_STEP_IO_UNM             3   	/* 步进输入端 */
#define PRN_STEP_IO_FUNC			FUNC_GPIO3
	// EN
#define PRN_EN_IO_MUX        		PERIPHS_IO_MUX_GPIO2_U
#define PRN_EN_IO_UNM             	2	/* 走纸步进马达控制端(低有效) */
#define PRN_EN_IO_FUNC				FUNC_GPIO2

#define SENSOR_ON_IO_MUX 			PERIPHS_IO_MUX_GPIO5_U
#define SENSOR_ON_IO_UNM 	        5    	
#define SENSOR_ON_IO_FUNC			FUNC_GPIO5
#define SENSOR_ON_SIGACTIVE			0			/* 定义有效信号电平 */

#endif
	
