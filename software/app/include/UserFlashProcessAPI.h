#ifndef __USERRFIDAPI_H
	#define __USERRFIDAPI_H

/******************************************************************* 
*	flash size 4MByte 
*	ǰ1024K-16K Ϊ������ 
*	����ȥ3072kΪ�û������� 
*	���16kΪ����ϵͳ������ 
*******************************************************************/ 
// �û�ʹ��������
// 1 �û�ϵͳ����
#define USER_SYSPARA_ADDR		0xFC000L	//((1024-16)*1024)
#define USER_SYSPARA_SIZE		0x4000L		// 16k	
	// 2 �ֿ����
#define USER_FONT_ADDR			0x100000L	// 1M�ĵ�ַ��ʼ
#define USER_FONT_SIZE			0x2FC000L	// 3056k(3M-16k)


extern u8 WriteNByteToSst(u8 *p,u16  len,u32  addr,u8 para);
extern u8 ReadNByteFromSst(u8 *p,u16  len,u32  addr);


#endif	
