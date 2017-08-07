#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"

#define MOTOR_SPEED 6000
#define MOTORA1  PBout(12)   	//PB4
#define MOTORA2  PBout(13)	 	//PB5 
#define MOTORB1  PBout(10)	 	//PB10 
#define MOTORB2  PBout(11)	 	//PB11 

void MOTOR_Init(u16 arr,u16 psc);		//IO≥ı ºªØ
void Set_PWM(int moto1,int moto2);
#endif

