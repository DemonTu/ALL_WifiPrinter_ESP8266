#ifndef __PRN__DOT__BUF__RING__DRV__H__
#define __PRN__DOT__BUF__RING__DRV__H__
/////////////////////////////////////////////////////////////////
#define R_TOT		(PrnDrv.Prn_Rave + PrnDrv.Prn_Ric)
#define SYS_VOLT	(SysVolt/10)	/* ϵͳ��ѹ *10mv */

/////////////////////////////////////////////////////////////////
extern ALSTR_PRN_INITARG PrnDrv;
extern const u8 dotcnttbl[];


/*
** �õײ�����ӡ�����Ѿ����ã����¼��������ǽӿ�
*/
/* ������󻺳���һ�����ݣ�*/
extern  void CopyOneDotLineToPrn(u8 *buffer);
#endif

