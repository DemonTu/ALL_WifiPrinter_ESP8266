#ifndef __PRN__GETBUF__HANDLE__H__
#define __PRN__GETBUF__HANDLE__H__
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/* 该函数主要负责从串口底层获取数据，正常设备速度慢的  */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#define	_WAIT_ZERO_EXIT		1
#define	_WAIT_NBYTE_EXIT	0

//------------------------------------------------------//
/*
** 应用接口
*/
//------------------------------------------------------//

/* 数据输出函数 */
extern uint32_t WaitPrnBufBytes(uint32_t len, uint8_t *buffer, uint32_t timeout,u8 bZeroExit);
///////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t GetRecvDatSize(void);
uint8_t GetRecvByte(void);

#endif

