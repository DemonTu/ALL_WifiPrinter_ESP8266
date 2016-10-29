/******************************************************************************
 * Copyright 2016-2017 TUT Systems (TQY)
 *
 * FileName: UserRfidAPI.c
 *
 * Description: flash control
 *
 * Modification history:
 *     2016/1/25, v1.0 create this file.
*******************************************************************************/

#include "includes.h"

/******************************************************************************
 * FunctionName : WriteNByteToSst
 * Description  : write data to flash
 * Parameters   : *p        Ҫд�������	 
 *				   len      ���ݳ���
 *				   addr		д��ĵ�ַ
 *				   para = 0 ���� 
 * Returns      : u8
*******************************************************************************/
u8 WriteNByteToSst(u8 *p,u16  len,u32  addr,u8 para)
{
	u16 i;

	if (len)
	{

		if(!para)					/* para==0 */
		{
			if (!(addr & 0xfff))	/* 4k 0xfff */
			{

				 spi_flash_erase_sector(addr/4096);
			}
		}
		spi_flash_write(addr, (uint32 *)p, len);
	}
	return OK;
}

/******************************************************************************
 * FunctionName : ReadNByteFromSst
 * Description  : read data from flash
 * Parameters   : *p        ��ȡ������	 
 *				   len      ���ݳ���
 *				   addr		��ȡ�ĵ�ַ
 * Returns      : u8
*******************************************************************************/
u8 ReadNByteFromSst(u8 *p,u16  len,u32	addr)
{
	spi_flash_read(addr, (uint32 *)p, len);
	return OK;
}


