#include "timer.h"
#include "infrared.h"
#include "usart.h"
#include "motor.h"
#include "key.h"
#include "led.h"
//V1.1 20140306 
//增加TIM1_CH1，PWM输出设置相关内容 
int PWM_ANGLE=0;
int ULTRA_ANGLE=0;
extern int MODE;
extern int PWM_SPEED; 
extern int Distance;

void TIM1_UP_IRQHandler(void)
{
	int INFRARED_ANGLE;
	if(TIM1->SR&0X0001)//溢出中断
	{
		LED1=!LED1;
		switch(MODE)
		{
			case INFRARED:
				INFRARED_ANGLE=SENSORL2*500+SENSORL1*100+SENSORL2*SENSORL1*1200-SENSORR1*100-SENSORR2*500-SENSORR2*SENSORR1*1200;
				if(INFRARED_ANGLE!=0)
					PWM_ANGLE+=INFRARED_ANGLE;
				else
					PWM_ANGLE=0;
				Set_PWM(PWM_SPEED+PWM_ANGLE,PWM_SPEED-PWM_ANGLE);
				break;
			case ULTRA:
				if(!SENSORFF)
				{
					//ULTRA_ANGLE+=(PWM_SPEED-Distance*10);
					Set_PWM(PWM_SPEED,-PWM_SPEED);
				}
				else
				{
					Set_PWM(PWM_SPEED,PWM_SPEED);
				}
				break;
			case STOP:
				Set_PWM(0,0);
		}
		//PWM_ANGLE-=PWM_ANGLE/10;
	}				   
	TIM1->SR&=~(1<<0);//清除中断标志位 	    
}
////////////////////////////////////////////////////////////////////////////////// 	  
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
void TIM1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11;	//TIM1时钟使能    
 	TIM1->ARR=arr;  	//设定计数器自动重装值 
	TIM1->PSC=psc;  	//预分频器设置
	TIM1->DIER|=1<<0;   //允许更新中断				
	TIM1->CR1|=0x01;    //使能定时器5
  	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);//抢占1，子优先级3，组2									 
}













