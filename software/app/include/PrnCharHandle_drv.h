#ifndef __PRN__DOT__HANDLE__
#define __PRN__DOT__HANDLE__
extern const unsigned char  Clr2HBit[8];
extern const unsigned char  Clr2LBit[8];//{0x01,0x03,0x07,0x0f,0x1f,0x2f,0x7f,0xff}

extern const unsigned char  bitmap[8];
extern const u8 bitmap_H[8];

//==============================================================================
void Emphasize(uint8_t *map, uint8_t w, uint8_t h);
void RollLeft90(unsigned char  *map, unsigned char w, unsigned char h);
void RollRight90(unsigned char  *map,unsigned  char w, unsigned char h);
void RollBack(unsigned char  *map,unsigned  char w, unsigned char h);
void memxrl438(unsigned char  *buf,unsigned char value,unsigned int length);
void DoubleWide(unsigned char  *map,unsigned  char w,unsigned  char h);
void DoubleTall(unsigned char  *map,unsigned char w, unsigned char h);
void  Del_Mark(unsigned char *map, unsigned char w, unsigned char h);
void ReverseDisplay(unsigned char *map, unsigned char w, unsigned char h);
void  Under_Line(unsigned char  *map, unsigned char w, unsigned char h, unsigned char line);
void CharmapDoubleWidth(unsigned char *chdot, unsigned int w, unsigned int h);
extern void PosFillxByte(unsigned char *map,unsigned int xpos,unsigned int ypos, unsigned int w, unsigned int h);
extern uint8_t BufFillBufByBit(STR_BufFillBufByBit *p);
/* 水平字节放大 */
extern u8 ZoomMaxNum(u8 *buf,u8 ch,u8 zoom);
/* 水平buf放大 */
extern uint8_t ZoomBufH(uint8_t *destbuf, uint8_t *srcbuf, uint8_t srcbuf_bytes, uint8_t zoom);
extern uint8_t ZoomX(uint8_t *srcbuf, uint8_t w, uint8_t h, uint8_t zoom);
extern uint8_t ZoomY(uint8_t *srcbuf, uint8_t w, uint8_t h, uint8_t zoom);
extern void FillBarByteBit(u8 * buffer,uint32_t bytes_buf_len,u32 offset,u8 val);
//==============================================================================
#endif

