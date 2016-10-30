#ifndef __USERSENSORDETECTION_H
	#define __USERSENSORDETECTION_H

	/**打印机是否过热**/
#define	_PRINTER_OVERHEAT()					(0)	// tqy

#ifdef PAPER_SENSOR_COM		// 共阴极接法
#define _ISLACKOFPAPER()		0			
#else
#define _ISLACKOFPAPER()		1			
#endif

extern void user_SensorDetection_Init(void);


#endif	

