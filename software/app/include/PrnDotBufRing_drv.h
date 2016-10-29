#ifndef __PRN__DOT__BUF__RING__DRV__H__
#define __PRN__DOT__BUF__RING__DRV__H__
/////////////////////////////////////////////////////////////////
#define R_TOT		(PrnDrv.Prn_Rave + PrnDrv.Prn_Ric)
#define SYS_VOLT	(SysVolt/10)	/* 系统电压 *10mv */

/////////////////////////////////////////////////////////////////
extern ALSTR_PRN_INITARG PrnDrv;
extern const u8 dotcnttbl[];


/*
** 该底层点阵打印缓存已经开好，以下几个函数是接口
*/
/* 放入点阵缓冲区一行数据，*/
extern  void CopyOneDotLineToPrn(u8 *buffer);
#endif

