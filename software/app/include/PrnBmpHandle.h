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
    uint32_t map_w;                // 图片宽
    uint32_t map_h;                // 图片长
    uint32_t map_zoom_x;           // 图片横向放大倍数
    uint32_t map_zoom_y;           // 图片纵向放大倍数
    uint8_t *map_buf;              // 图片缓存地址
}STR_MAP,*P_STR_MAP;

///////////////////////////////////////////////////////////////////////////////
extern uint8_t bBitMapFlag;

/*
* 位图打印初始化
*/
ENUM_OP_STATE BitmapInit(void);
/* 针打位图打印函数 */
extern uint8_t PutDataInMap(uint8_t* map_add, uint32_t w, uint32_t h, uint32_t data_num, uint8_t map_data);
#endif
