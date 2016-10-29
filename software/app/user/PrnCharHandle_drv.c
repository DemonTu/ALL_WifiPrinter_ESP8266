#include <includes.h>
//点阵处理函数

const unsigned char  Clr2HBit[8]={0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00,};
const unsigned char  Clr2LBit[8]={0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00,};//{0x01,0x03,0x07,0x0f,0x1f,0x2f,0x7f,0xff}

const unsigned char  bitmap[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const u8 bitmap_H[]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
/* 字体加粗 */
void ICACHE_FLASH_ATTR
Emphasize(uint8_t *map, uint8_t w, uint8_t h)
{
    unsigned char  buf[72];
    char  i, j;

    os_memset(buf, 0, w * ((h+7)/8));
    for(i=0; i<w; i++)
    {
        for(j=0; j<h; j++)
        {
            if(map[j*((w+7)/8) + i/8] & bitmap_H[i%8])
            {
                if((i == 0) || (map[j*((w+7)/8) + (i-1)/8] & bitmap_H[(i-1)%8]))
                {
                }
                else
                {
                    buf[j*((w+7)/8) + (i-1)/8] |= bitmap_H[(i-1)%8];
                }
                buf[j*((w+7)/8) + i/8] |= bitmap_H[i%8];
            }
        }
    }
    os_memcpy(map, buf, w * ((h+7)/8));
}
//======================================================================================================

//点阵右转90度
void RollRight90(unsigned char  *map,unsigned  char w, unsigned char h)
{
    unsigned char buf[72];
    char  i, j;

    os_memset(buf, 0, w * ((h+7)/8));
    for(i=0; i<w; i++)
    {
        for(j=0; j<h; j++)
        {
            if(map[j* ((w+7)/8) + i/8] & bitmap[7-i%8])
            {
                buf[i * ((h+7)/8) + (h-j-1)/8] |= bitmap[(j+h%8)%8];
            }
        }
    }
    os_memcpy(map, buf, w * ((h+7)/8));
}

void ICACHE_FLASH_ATTR
DoubleWide(unsigned char  *map,unsigned  char w,unsigned  char h)
{
    unsigned char  buf[0x200];
    unsigned char  i, j,d;

    os_memset(buf, 0, h * ((2*w + 7)/8) );

    for(i=0; i<w; i++)
    {
        for(j=0; j<h; j++)
        {
            if(map[j*((w+7)/8) + i/8] & bitmap[7-i%8])
            {
                d = 2*i;
                buf[j*((2*w+7)/8) + d/8] |= bitmap[7-d%8];
                buf[j*((2*w+7)/8) + (d+1)/8] |= bitmap[7-(d+1)%8];
            }
        }
    }
    os_memcpy(map, buf,  h * ((2*w + 7)/8));
}

void ICACHE_FLASH_ATTR
Del_Mark(unsigned char *map, unsigned char w, unsigned char h)
{
    unsigned int midbyte1,midbyte2;
    midbyte1 = (h/2)*((w+7)/8);
    midbyte2 = ((h-1)/2)*((w+7)/8);
    os_memset(&map[midbyte2],0xff,(w+7)/8);
    os_memset(&map[midbyte1],0xff,(w+7)/8);
}


void ICACHE_FLASH_ATTR
ReverseDisplay(unsigned char *map, unsigned char w, unsigned char h)
{
    unsigned int i;
    unsigned int loop;
    loop = h*((w+1)/2);
    for(i=0;  i<loop; i++)
    {
        map[i] = ~map[i];
    }
}

void ICACHE_FLASH_ATTR
Under_Line(unsigned char  *map, unsigned char w, unsigned char h, unsigned char line)
{
    unsigned char   i,j;
    if (line > h)
    {
        return;
    }
    for(j=0; j<w; j++)
    {
        for (i=0; i<(line+1); i++)   // 发现一个点行不明显，所以这里直接加一了
        {
            map[(h-i)*((w+7)/8) + j/8] |= bitmap[7-j%8];
        }
    }
}

//==============================================================================
/*
*函数功能: 将源buf的数据从目标buf的第shift_bit位开始，移入shift_bit_total位
*输入参数: dest_buf(目的地址),src_buf(源地址),shitf_bit(偏移量),shift_bit_total(移动位数)
*输出参数: 无
*返回值  : TRUE (成功)or FALSE(失败),
*/
//==============================================================================
/*
typedef struct
{
    uint8_t *src_buf;             // 源buf
    uint32_t src_buf_size;        // 源大小
    uint8_t *dest_buf;            // 目的buf
    uint32_t dest_buf_size;       // 目的buf大小
    uint32_t src_shift_start_bit; // 源开始位
    uint32_t dest_shift_start_bit;// 目的开始位
    uint32_t shift_bit_total;     // 移动总位数
}STR_BufFillBufByBit;
*/
uint8_t ICACHE_FLASH_ATTR
BufFillBufByBit(STR_BufFillBufByBit *p)
{
    uint32_t x,y,i;
    x = p->src_shift_start_bit%8;
    y = p->src_shift_start_bit>>3;
    if ((p->dest_shift_start_bit+p->shift_bit_total) > (p->dest_buf_size<<3))
    {
        return FALSE;
    }
    else if ((p->shift_bit_total+p->src_shift_start_bit) > (p->src_buf_size<<3))
    {
        return FALSE;
    }
    else
    {
        if (x)
        {
            FillBarByteBit(p->dest_buf, \
                           p->dest_buf_size,\
                           p->dest_shift_start_bit, \
                           (p->src_buf[y]<<x));
            for (i=0; i<((p->shift_bit_total-x+7)/8); i++)
            {
                FillBarByteBit(p->dest_buf, \
                               p->dest_buf_size,\
                               p->dest_shift_start_bit+(8-x)+(i<<3), \
                               p->src_buf[(y+1)+i]);
            }
        }
        else // 刚好从整数开始复制
        {
            for (i=0; i<((p->shift_bit_total+7)/8); i++)
            {
                FillBarByteBit(p->dest_buf, \
                               p->dest_buf_size,\
                               p->dest_shift_start_bit+(i<<3), \
                               p->src_buf[y+i]);
            }
        }
        return TRUE;
    }
}
// 函数 :
//              FillBarByteBit
// 功能 :
//              基于buffer指向的地址，偏移offset个点后，填入val所表示的8个点
// 输入 :
//              buffer : 基本地址()
//              offset : 偏移量，点
//              val    : 要填入的8个点
// 输出 :
//              无
// 返回 :
//              无
void ICACHE_FLASH_ATTR
FillBarByteBit(u8 * buffer,uint32_t bytes_buf_len,u32 offset,u8 val)
{
    u8  temp;
    u8  ch,j;
    u32  z;

    temp = offset % 8;
    z =  offset / 8;
    if(temp)
    {
        // 前几个bit
        ch = buffer[z];
        ch &= Clr2LBit[8 - temp - 1]; //清一个字的低位
        j = (val >> temp);
        buffer[z] = ch | j;
        // 后几个bit
        ch = buffer[z + 1];
        ch &= Clr2HBit[temp - 1];
        j = (val << (8 - temp));
        if ((z+1) >= bytes_buf_len)
        {
            return;
        }
        else
        {
            buffer[z + 1] = ch | j;
        }
    }
    else
    {
        // 可以整除
        buffer[z] = val;
    }
}

