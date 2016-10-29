#ifndef __PRNESCHANDLE__H__
#define __PRNESCHANDLE__H__

typedef struct strcmd
{
    u8   codevalue;
    u16  waitcount;                // �������������
    u8  (*codefunc)(u8 *buffer);   // ��Ӧcodeִ�е�����
} __packed STRPRNCMD;
////////////////////////////////////////////////////////////////////////////////
extern u8 HzAreaF;

////////////////////////////////////////////////////////////////////////////////
/*
** �ⲿ���ú���
*/
void PrnEscInit(void);
/*
** Ӧ�ýӿں���
*/
uint32_t PrnEscHandle(void);                    /* ESC������غ�������ִ��Ƶ��Խ��Խ�� */
/*
** �ײ�ӿں���
*/
#endif
