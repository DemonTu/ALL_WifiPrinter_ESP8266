#ifndef __USERRFIDAPI_H
	#define __USERRFIDAPI_H

/******************************************************************* 
*	flash size 4MByte 
*	前1024K-16K 为代码区 
*	接下去3072k为用户参数区 
*	最后16k为自身系统参数区 
*******************************************************************/ 
// 用户使用区划分
// 1 用户系统参数
#define USER_SYSPARA_ADDR		0xFC000L	//((1024-16)*1024)
#define USER_SYSPARA_SIZE		0x4000L		// 16k	
	// 2 字库参数
#define USER_FONT_ADDR			0x100000L	// 1M的地址开始
#define USER_FONT_SIZE			0x2FC000L	// 3056k(3M-16k)


extern u8 WriteNByteToSst(u8 *p,u16  len,u32  addr,u8 para);
extern u8 ReadNByteFromSst(u8 *p,u16  len,u32  addr);


#endif	
