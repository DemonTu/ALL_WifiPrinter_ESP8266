#ifndef __USERSENSORDETECTION_H
	#define __USERSENSORDETECTION_H

	/**��ӡ���Ƿ����**/
#define	_PRINTER_OVERHEAT()					(0)	// tqy

#ifdef PAPER_SENSOR_COM		// �������ӷ�
#define _ISLACKOFPAPER()		0			
#else
#define _ISLACKOFPAPER()		1			
#endif

extern void user_SensorDetection_Init(void);


#endif	

