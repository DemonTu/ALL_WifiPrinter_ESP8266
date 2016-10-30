#ifndef __PRN__COMMON__H__
#define __PRN__COMMON__H__
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    SINGLE_CHAR = 0x01,
    DOUBLE_CHAR =0x02,
    SINGLE_FORCE = 0x81,	/**ǿ��д��**/
    DOUBLE_FORCE =0x82,		/**ǿ��д��**/
} ENUM_CHAR_NUM;

typedef enum
{
    STANDERD_MODE = 0,
    PAGE_MODE,
} PRINTER_MODE;
typedef  struct
{
    uint8_t *src_buf;             // Դbuf
    uint32_t src_buf_size;        // Դ��С
    uint8_t *dest_buf;            // Ŀ��buf
    uint32_t dest_buf_size;       // Ŀ��buf��С
    uint32_t src_shift_start_bit; // Դ��ʼλ
    uint32_t dest_shift_start_bit;// Ŀ�Ŀ�ʼλ
    uint32_t shift_bit_total;     // �ƶ���λ��
}STR_BufFillBufByBit;
///////////////////////////////////////////////////////////////////////////////
#endif
