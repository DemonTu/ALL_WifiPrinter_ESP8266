/******************************************************************************
 * Copyright 2016 DINDING (Demon)
 *
 * FileName: sensorDetection.c
 *
 * Description: read sensor count
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/
#include "includes.h"

//=======================================================
#define SENSOR_IO_MUX     PERIPHS_IO_MUX_GPIO4_U
#define SENSOR_IO_NUM     4
#define SENSOR_IO_FUNC    FUNC_GPIO4

LOCAL os_timer_t sensor_timer;


/******************************************************************************
 * FunctionName : sensor_20ms_cb
 * Description  : 20ms 延时去抖函数
 * Parameters   : flag trashy
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
sensor_20ms_cb(uint8_t flag)
{
    if (0 == GPIO_INPUT_GET(GPIO_ID_PIN(SENSOR_IO_NUM))) 
	{
		// 
    }

	gpio_pin_intr_state_set(GPIO_ID_PIN(SENSOR_IO_NUM), GPIO_PIN_INTR_NEGEDGE);
}

/******************************************************************************
 * FunctionName : sensorHandler
 * Description  : sensor interrupt handler
 * Parameters   : NULL
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
sensorHandler(void *para)
{
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

	//disable interrupt
	gpio_pin_intr_state_set(GPIO_ID_PIN(SENSOR_IO_NUM), GPIO_PIN_INTR_DISABLE);

	//clear interrupt status
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

     /* 延时20ms，去抖动 */
    os_timer_disarm(&sensor_timer);
    os_timer_setfn(&sensor_timer, (os_timer_func_t *)sensor_20ms_cb, 1);
    os_timer_arm(&sensor_timer, 20, 0);
}

/******************************************************************************
 * FunctionName : user_SensorDetection_Init
 * Description  : sensor GPIO interrupt init
 * Parameters   : void
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_SensorDetection_Init(void)
{
    PIN_FUNC_SELECT(SENSOR_IO_MUX, SENSOR_IO_FUNC);
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(SENSOR_IO_NUM));

	
	ETS_GPIO_INTR_ATTACH(sensorHandler, NULL);
	
	ETS_GPIO_INTR_DISABLE();

    //enable interrupt
    gpio_pin_intr_state_set(GPIO_ID_PIN(SENSOR_IO_NUM), GPIO_PIN_INTR_NEGEDGE);

    ETS_GPIO_INTR_ENABLE();	

	os_printf("sensor init\r\n");
}


