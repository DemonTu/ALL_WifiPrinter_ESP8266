/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_devicefind.c
 *
 * Description: Find your hardware's information while working any mode.
 *
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

#include "espconn.h"
//#include "user_json.h"
#include "user_devicefind.h"

// user device
#include "UserFlashProcessAPI.h"
#include "UserKeyDeviceAPI.h"
#include "userSensorDetection.h"

/*---------------------------------------------------------------------------*/
LOCAL struct espconn ptrespconn;

/*--------------------------------------------------------------------
*  crc(2byte) head(1byte)	cmd(1byte)	len(2byte)	data(len byte)
*  ע: crcֻ����data
*--------------------------------------------------------------------*/
typedef struct
{
	uint16_t crc;
	uint8_t  head;
	uint8_t  cmd;
	uint16_t len;
//    uint8_t *dat; 	
}DataStr;

#define FIND_DEVICE			0x80
#define SET_DEVICE_ID		0x81
#define SYNC_SYSTEM_TIME	0x82
#define READ_DEVICE_PARA	0x83

#define ACK_OK				0x10
#define ACK_ERROR			0x11
/******************************************************************************
 * FunctionName : udpDataPacket
 * Description  : Packeting udp data to send
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
udpDataPacket(uint8_t *sendBuf, uint16_t len, uint8_t cmd)
{
	remot_info *premot = NULL;
	uint16_t length=0;
	uint8_t sendDeviceBuffer[60];

	/* crc */
	
	sendDeviceBuffer[2] = 0xAA;
	sendDeviceBuffer[3] = cmd;
	sendDeviceBuffer[4] = len;
	length += sizeof(DataStr);
	if (len)
	{
		os_memcpy(&sendDeviceBuffer[length], sendBuf, len);
		length += len;
	}
	if (espconn_get_connection_info(&ptrespconn, &premot, 0) != ESPCONN_OK)
	{
		return;
	}
	os_memcpy(ptrespconn.proto.udp->remote_ip, premot->remote_ip, 4);
	ptrespconn.proto.udp->remote_port = premot->remote_port;
	espconn_sent(&ptrespconn, sendDeviceBuffer, length);
}

/******************************************************************************
 * FunctionName : user_devicefind_recv
 * Description  : Processing the received data from the host
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
user_devicefind_recv(void *arg, char *pusrdata, unsigned short length)
{
    char DeviceBuffer[60] = {0};
	unsigned short datLen=0;
    char hwaddr[6];
    struct ip_info ipconfig;
	DataStr datAnalyze;

	char sendFlag=0;

    if (wifi_get_opmode() != STATION_MODE) {
        wifi_get_ip_info(SOFTAP_IF, &ipconfig);
        wifi_get_macaddr(SOFTAP_IF, hwaddr);

        if (!ip_addr_netcmp((struct ip_addr *)ptrespconn.proto.udp->remote_ip, &ipconfig.ip, &ipconfig.netmask)) {
            wifi_get_ip_info(STATION_IF, &ipconfig);
            wifi_get_macaddr(STATION_IF, hwaddr);
        }
    } else {
        wifi_get_ip_info(STATION_IF, &ipconfig);
        wifi_get_macaddr(STATION_IF, hwaddr);
    }

    if (pusrdata == NULL) {
        return;
    }
	else if (length < sizeof(DataStr))
	{
		return;
	}
	datAnalyze.head = pusrdata[2];
	datAnalyze.cmd = pusrdata[3];
	datAnalyze.len = (pusrdata[4]<<8)|pusrdata[5];
	os_printf("len=%d\r\n", datAnalyze.len);
	switch(datAnalyze.cmd)
	{
		case FIND_DEVICE:
			if (0 == datAnalyze.len)
			{
				/* ��ʼͨ���� */
				os_memcpy(DeviceBuffer, sysPara.deviceID, sizeof(sysPara.deviceID));		
				os_memcpy(&DeviceBuffer[sizeof(sysPara.deviceID)], hwaddr, 6);		
				os_memcpy(&DeviceBuffer[sizeof(sysPara.deviceID)+6], &ipconfig.ip, 4);		
				datLen = sizeof(sysPara.deviceID)+10;
				sendFlag = 1;
			}
			else if ((datAnalyze.len == sizeof(sysPara.deviceID)) &&
				(0==os_memcmp(sysPara.deviceID, &pusrdata[6], datAnalyze.len)))
			{
				/* Ӧ�� */
				os_memcpy(&DeviceBuffer[0], hwaddr, 6);		
				os_memcpy(&DeviceBuffer[6], &ipconfig.ip, 4);	
				datLen = 10;
				sendFlag = 1;
			}
			break;
		case SET_DEVICE_ID:
			os_memcpy(sysPara.deviceID, &pusrdata[sizeof(datAnalyze)], datAnalyze.len);
			sysTemParaSave();
			datLen = 0;
			sendFlag = 1;
			break;
		case SYNC_SYSTEM_TIME:
			
			break;
		case READ_DEVICE_PARA:
			break;

		default:
			break;
	}
    if (sendFlag) 
    {
		udpDataPacket(DeviceBuffer, datLen, ACK_OK);
	}

}

/******************************************************************************
 * FunctionName : user_devicefind_init
 * Description  : the espconn struct parame init
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_devicefind_init(void)
{
    ptrespconn.type = ESPCONN_UDP;
    ptrespconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    ptrespconn.proto.udp->local_port = 1025;
    espconn_regist_recvcb(&ptrespconn, user_devicefind_recv);
    espconn_create(&ptrespconn);
}
