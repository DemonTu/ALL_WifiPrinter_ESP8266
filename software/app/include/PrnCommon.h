#ifndef __PRN__COMMON__H__
#define __PRN__COMMON__H__
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    SINGLE_CHAR = 0x01,
    DOUBLE_CHAR =0x02,
    SINGLE_FORCE = 0x81,	/**强行写入**/
    DOUBLE_FORCE =0x82,		/**强行写入**/
} ENUM_CHAR_NUM;

typedef enum
{
    STANDERD_MODE = 0,
    PAGE_MODE,
} PRINTER_MODE;
typedef  struct
{
    uint8_t *src_buf;             // 源buf
    uint32_t src_buf_size;        // 源大小
    uint8_t *dest_buf;            // 目的buf
    uint32_t dest_buf_size;       // 目的buf大小
    uint32_t src_shift_start_bit; // 源开始位
    uint32_t dest_shift_start_bit;// 目的开始位
    uint32_t shift_bit_total;     // 移动总位数
}STR_BufFillBufByBit;
///////////////////////////////////////////////////////////////////////////////
#endif
