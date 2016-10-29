#ifndef __PRNESCHANDLE__H__
#define __PRNESCHANDLE__H__

typedef struct strcmd
{
    u8   codevalue;
    u16  waitcount;                // 该命令参数个数
    u8  (*codefunc)(u8 *buffer);   // 对应code执行的命令
} __packed STRPRNCMD;
////////////////////////////////////////////////////////////////////////////////
extern u8 HzAreaF;

////////////////////////////////////////////////////////////////////////////////
/*
** 外部引用函数
*/
void PrnEscInit(void);
/*
** 应用接口函数
*/
uint32_t PrnEscHandle(void);                    /* ESC处理挂载函数，被执行频率越高越好 */
/*
** 底层接口函数
*/
#endif
