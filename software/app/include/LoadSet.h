#ifndef __LOADSET_H__
#define __LOADSET_H__

      	#define SETTING             USER_SYSPARA_ADDR  // PC 下载参数区  (04k)

        #define ECRIPADDR           0x00    //  4 bytes
        #define ECRIDADDR           0x04    //  4 bytes
        #define HOSTIPADDR          0x08    //  4 bytes
        #define SBARDRATE           0x0C    //  1 bytes
        #define SDENSITY            0x0D    //  1 bytes
        #define SFONT               0x0E    //  1 bytes
        #define SHEIGHT             0x0F    //  1 bytes
        #define SLINESPACE          0x10    //  1 bytes
        #define SFONTPROP           0x11    //  1 bytes
        #define SBARCODEWIDTH       0x12    //  1 bytes
        #define SBARCODEHEIGHT      0x13    //  1 bytes
        #define SBARCODEHRIPOS      0x14    //  1 bytes
        #define SBARCODELEFT        0x15    //  1 bytes
        #define STABPOSITION        0x16    // 32bytes

        #define HaveLabelAddr       0x40    // 1 byte A6: print label
        #define LabelHeightAddr     0x41    // 2 bytes,Uint, Unit Dot(0.125mm)
        #define LabelLedHeightAddr  0x43    // 1 byte
        #define ScomBps             0x44    // 1 byte ,defuault bps; 0: 9600,1:19200,...
        #define AlarmStatus         0x46    // 警报状态
        #define AlarmSndInterval    0x47    // 警报声音间隔 2byte
        #define AlarmSndInterval_reserve 0x48

        #define EthernetSpeed       0x50    // 1 byte =0 为10M 反之为100M

        #define boardnum            0x60    // 电路板号
        #define swtype              0x61    // 对应软件版本号
        #define VersionNum          0x62    //  版本号
        #define VersionNum_H        0x63    //  版本号高位信息
        #define VersionNum_L        0x64    //  版本号低位信息

        #define HighPrnAddr         0x66    //  220mm|150mm/sec  非零为高速
        #define CookPrnAddr         0x67    //  厨房打印标志     非零为厨打
        #define ChinessTypeAddr     0x68    //  中文简/繁体
        #define ClrParaAddr         0x69    //  = 0xaa 表示清参数 (注:只清一次)
        #define CodePageAddr        0x6A    //  国家代码 (     =0或0xff, PC437 美国
                                            //               =1， PC860 葡萄牙文
                                            //               =2， WPC1255(Israel)希伯来文
                                            //               =3， PC866(Cyrillic #2)俄罗斯文
                                            //               =4， 罗马尼亚文
                                            //               =5， Farsi 伊朗文
                                            //               =6， PC864(Arabic) 阿拉伯文
                                            //               =7， 越南文
                                            //               =8， PC857 土耳其文
                                            //               =9， 西班牙文
                                            //               =10，PC863(Canada-French)
                                            //               =11，PC850 多国语言
                                            //               =12，亚美尼亚文
                                            //               =13，PC865(Nordic)
                                            //               =14，PC852(Latin 2)
                                            //               =15，WPC1254(Turkey)

        #define BtNameAddr          0x70    // 蓝牙设备地址


        #define WirelessIPAddr        0xa0  // 无线打印机IP地址
        #define WirelessStationIPAddr 0xa4  // 无线基站IP地址
        #define WirelessIDAddr        0xa8  // 无线打印机ID地址
        #define USBTypeAddr           0xC0  // USB类型
//		XXX 		 XX 	  XXXX		   XXXX
//	  机器名称	   方案名 	 年/周		  流水号
		#define MachineIdAddr		  0xC1  // 13 位编码：


        #define LogoStartAddr1      0x70000  // SST040 LOGO 开始地址
        #define LogoStartAddr       0x1c0000 // SST016 LOGO 开始地址

///////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	USBTYPE_VCOM = 0,
	USBTYPE_PRINTER,
	MAX_USBTYPE,
}ENUM_USB_TYPE;

///////////////////////////////////////////////////////////////////////////////////////////
// 中文
#define FONT_ENGLISH	0	// 英文
#define FONT_JCHINESE	1	// 简体
#define FONT_FCHINESE	2	// 繁体
#define FONT_JAPANESE	3	// 日文

extern uint8_t PrintFontType;
extern uint8_t CodePage;
extern uint8_t Density;
extern u32 MachineNo;
extern u8 MachineIdStr[14];
///////////////////////////////////////////////////////////////////////////////////////////
extern void LoadSetting(void);

#endif


