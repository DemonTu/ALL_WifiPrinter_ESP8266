#ifndef __PRN__DOT__BUF__RING__H__
#define __PRN__DOT__BUF__RING__H__

typedef enum
{
    PARA_NONE = 0,
    PARA_FULL_CUT_PAPER,
    PARA_HALF_CUT_PAPER,
}ENUM_PARA;
/*
** Ӧ�ýӿ�
*/
void PrnPutOneLineToPrintBuf(uint8_t *buf);

#endif
