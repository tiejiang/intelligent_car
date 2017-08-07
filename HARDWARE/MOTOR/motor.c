#include "motor.h"
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：PWM 以及电机控制的IO初始化
入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
void MOTOR_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;       //TIM4时钟使能    
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能     
	//RCC->APB2ENR|=1<<2;       //PORTA时钟使能   
	GPIOB->CRL&=0X00FFFFFF;//
	GPIOB->CRL|=0XBB000000;//初始化 PB67为复用功能输出 PB1213为推挽输出
	GPIOB->CRH&=0XFF0000FF;
	GPIOB->CRH|=0X00333300;//初始化 PB10,11为推挽输出
	//GPIOB->ODR|=1<<6;//PB6上拉	
	//GPIOB->ODR|=1<<7;//PB7上拉
	
	TIM4->ARR=arr;//设定计数器自动重装值 
	TIM4->PSC=psc;//预分频器不分频
	TIM4->CCMR1|=6<<12;  //CH2 PWM2模式	
	TIM4->CCMR1|=6<<4;  //CH1 PWM2模式	
	//TIM4->CCMR2|=6<<12;  //CH4 PWM2模式	
	//TIM4->CCMR2|=6<<4;  //CH3 PWM2模式	
	TIM4->CCMR1|=1<<11; //CH2预装载使能	 
	TIM4->CCMR1|=1<<3; //CH1预装载使能	  
	//TIM4->CCMR2|=1<<11; //CH4预装载使能	 
	//TIM4->CCMR2|=1<<3; //CH3预装载使能	  
	TIM4->CCER|=1<<4;   //CH2 输出使能	   
	TIM4->CCER|=1;   //CH1 输出使能	
	//TIM4->CCER|=1<<12;   //CH4 输出使能	   
	//TIM4->CCER|=1<<8;   //CH3 输出使能	
	TIM4->CR1=0x8000;   //ARPE使能 
	TIM4->CR1|=0x01;    //使能定时器2 
    TIM4->BDTR|=1<<15;   	//MOE 主输出使能		
}
int PWM_SPEED=MOTOR_SPEED;
void Set_PWM(int moto1,int moto2)//PWM输出
{
	if(moto1<-7100)
		moto1=-7100;
	else if(moto1>7100)
		moto1=7100;
	if(moto2<-7100)
		moto2=-7100;
	else if(moto2>7100)
		moto2=7100;
	
	if(moto1<0)
		MOTORA1=1,MOTORA2=0;
	else
		MOTORA1=0,MOTORA2=1;
	
	TIM4->CCR2=myabs(moto1);
	if(moto2<0)
		MOTORB1=0,MOTORB2=1;
	else
		MOTORB1=1,MOTORB2=0;
	TIM4->CCR1= myabs(moto2);	
}
