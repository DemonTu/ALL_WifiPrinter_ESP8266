#ifndef __PRN__BMP__HANDLE__H__
#define __PRN__BMP__HANDLE__H__
///////////////////////////////////////////////////////////////////////////////
#define BITMAP_BUF_SIZE 64   /* 1024bytes */
#define RAM_BITMAP_NUMBER 1;
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    BITMAP_DOUBLE_WIDTH = 0x01,
    BITMAP_DOUBLE_HIGH  = 0x02,
} ENUM_MAP_MODE;

typedef struct
{
    uint32_t map_w;                // ͼƬ��
    uint32_t map_h;                // ͼƬ��
    uint32_t map_zoom_x;           // ͼƬ����Ŵ���
    uint32_t map_zoom_y;           // ͼƬ����Ŵ���
    uint8_t *map_buf;              // ͼƬ�����ַ
}STR_MAP,*P_STR_MAP;

///////////////////////////////////////////////////////////////////////////////
extern uint8_t bBitMapFlag;

/*
* λͼ��ӡ��ʼ��
*/
ENUM_OP_STATE BitmapInit(void);
/* ���λͼ��ӡ���� */
extern uint8_t PutDataInMap(uint8_t* map_add, uint32_t w, uint32_t h, uint32_t data_num, uint8_t map_data);
#endif
