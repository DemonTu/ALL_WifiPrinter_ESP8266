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
#include "includes.h"
#include "espconn.h"

/*---------------------------------------------------------------------------*/
LOCAL struct espconn ptrespconn;

/*--------------------------------------------------------------------
*  crc(2byte) head(1byte)	cmd(1byte)	len(2byte)	data(len byte)
*  注: crc只包含data
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

#ifdef UART_HW_FLOW
#define ENABLE_RTS()			Uart_RTS_LOW()
#define DISABLE_RTS()			Uart_RTS_HIGH()
#define RXBUF_HIGH_LEVEL		(udp_rx_buf_Number-512)
#define RXBUF_LOW_LEVEL			(udp_rx_buf_Number-512)
#endif

#define udp_rx_buf_Number  	0x800    		/* 接接缓冲区大小,必须为2的n次方 */


typedef struct
{
	volatile u16 head;
	volatile u16 tail;
	volatile u8  timeout;
	volatile u8  error;
} UDP_RX_T;

typedef struct
{
	UDP_RX_T      rx;
} UDP_T;

static UDP_T udp;
static uint8_t udp_rx_buf[udp_rx_buf_Number];

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
 * FunctionName : user_GetUdpChar
 * Description  : get a char from queue of revice data
 * Parameters   : void
 * Returns      : char
*******************************************************************************/
uint8_t ICACHE_FLASH_ATTR
user_GetUdpChar(void)
{
    u8 c;

    c = udp_rx_buf[udp.rx.tail];

   	udp.rx.tail = RBUF_NEXT_PT(udp.rx.tail, 1, udp_rx_buf_Number);
#ifdef UART_HW_FLOW
	if( user_GetUdpBufSize() < RXBUF_LOW_LEVEL)
	{
		ENABLE_RTS();		/* udp流控 */
	}
#endif

    return c;
}

/******************************************************************************
 * FunctionName : user_GetUdpBufSize
 * Description  : get the length of revice data
 * Parameters   : void
 * Returns      : length
*******************************************************************************/
uint16_t ICACHE_FLASH_ATTR
user_GetUdpBufSize(void)
{
	return (udp.rx.head < udp.rx.tail) ? (udp_rx_buf_Number+udp.rx.head-udp.rx.tail):(udp.rx.head - udp.rx.tail);
}

/******************************************************************************
 * FunctionName : put_prn
 * Description  : 填字符打印
 * Parameters   : char
 * Returns      : 1
*******************************************************************************/
uint8_t ICACHE_FLASH_ATTR
put_prn(uint8_t ch)
{
	uint16_t head;
	head = RBUF_NEXT_PT(udp.rx.head, 1, udp_rx_buf_Number);
	while (head == udp.rx.tail)           // 有空间
	{
		system_soft_wdt_feed();
		PrnEscHandle();
	}
	udp_rx_buf[udp.rx.head] = ch;
	udp.rx.head = head;
	return 1;
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
    char hwaddr[6];
    struct ip_info ipconfig;
	uint16_t head;
	
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
	
	while (length)
    {
	    head = RBUF_NEXT_PT(udp.rx.head, 1, udp_rx_buf_Number);
	    if (head != udp.rx.tail)			/* 有空间 */
	    {
			udp_rx_buf[udp.rx.head] = *pusrdata;
	        udp.rx.head = head;
	        udp.rx.timeout = 0;
	    }
	    else                              	/* 满 流量控制 */
	    {
	        udp.rx.error = 1;			/* error */
	    	break;
	    }
		pusrdata++;
		length--;
	}	
#ifdef UART_HW_FLOW
	if( user_GetUdpBufSize() >= RXBUF_HIGH_LEVEL)
	{
		DISABLE_RTS();
	}
	else
	{
		ENABLE_RTS();
	}
#endif

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
