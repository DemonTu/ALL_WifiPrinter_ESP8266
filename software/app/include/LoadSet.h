#ifndef __LOADSET_H__
#define __LOADSET_H__

      	#define SETTING             USER_SYSPARA_ADDR  // PC ���ز�����  (04k)

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
        #define AlarmStatus         0x46    // ����״̬
        #define AlarmSndInterval    0x47    // ����������� 2byte
        #define AlarmSndInterval_reserve 0x48

        #define EthernetSpeed       0x50    // 1 byte =0 Ϊ10M ��֮Ϊ100M

        #define boardnum            0x60    // ��·���
        #define swtype              0x61    // ��Ӧ����汾��
        #define VersionNum          0x62    //  �汾��
        #define VersionNum_H        0x63    //  �汾�Ÿ�λ��Ϣ
        #define VersionNum_L        0x64    //  �汾�ŵ�λ��Ϣ

        #define HighPrnAddr         0x66    //  220mm|150mm/sec  ����Ϊ����
        #define CookPrnAddr         0x67    //  ������ӡ��־     ����Ϊ����
        #define ChinessTypeAddr     0x68    //  ���ļ�/����
        #define ClrParaAddr         0x69    //  = 0xaa ��ʾ����� (ע:ֻ��һ��)
        #define CodePageAddr        0x6A    //  ���Ҵ��� (     =0��0xff, PC437 ����
                                            //               =1�� PC860 ��������
                                            //               =2�� WPC1255(Israel)ϣ������
                                            //               =3�� PC866(Cyrillic #2)����˹��
                                            //               =4�� ����������
                                            //               =5�� Farsi ������
                                            //               =6�� PC864(Arabic) ��������
                                            //               =7�� Խ����
                                            //               =8�� PC857 ��������
                                            //               =9�� ��������
                                            //               =10��PC863(Canada-French)
                                            //               =11��PC850 �������
                                            //               =12������������
                                            //               =13��PC865(Nordic)
                                            //               =14��PC852(Latin 2)
                                            //               =15��WPC1254(Turkey)

        #define BtNameAddr          0x70    // �����豸��ַ


        #define WirelessIPAddr        0xa0  // ���ߴ�ӡ��IP��ַ
        #define WirelessStationIPAddr 0xa4  // ���߻�վIP��ַ
        #define WirelessIDAddr        0xa8  // ���ߴ�ӡ��ID��ַ
        #define USBTypeAddr           0xC0  // USB����
//		XXX 		 XX 	  XXXX		   XXXX
//	  ��������	   ������ 	 ��/��		  ��ˮ��
		#define MachineIdAddr		  0xC1  // 13 λ���룺


        #define LogoStartAddr1      0x70000  // SST040 LOGO ��ʼ��ַ
        #define LogoStartAddr       0x1c0000 // SST016 LOGO ��ʼ��ַ

///////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	USBTYPE_VCOM = 0,
	USBTYPE_PRINTER,
	MAX_USBTYPE,
}ENUM_USB_TYPE;

///////////////////////////////////////////////////////////////////////////////////////////
// ����
#define FONT_ENGLISH	0	// Ӣ��
#define FONT_JCHINESE	1	// ����
#define FONT_FCHINESE	2	// ����
#define FONT_JAPANESE	3	// ����

extern uint8_t PrintFontType;
extern uint8_t CodePage;
extern uint8_t Density;
extern u32 MachineNo;
extern u8 MachineIdStr[14];
///////////////////////////////////////////////////////////////////////////////////////////
extern void LoadSetting(void);

#endif


