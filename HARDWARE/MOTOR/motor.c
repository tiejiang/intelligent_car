#include "motor.h"
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�PWM �Լ�������Ƶ�IO��ʼ��
��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void MOTOR_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;       //TIM4ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ��     
	//RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ��   
	GPIOB->CRL&=0X00FFFFFF;//
	GPIOB->CRL|=0XBB000000;//��ʼ�� PB67Ϊ���ù������ PB1213Ϊ�������
	GPIOB->CRH&=0XFF0000FF;
	GPIOB->CRH|=0X00333300;//��ʼ�� PB10,11Ϊ�������
	//GPIOB->ODR|=1<<6;//PB6����	
	//GPIOB->ODR|=1<<7;//PB7����
	
	TIM4->ARR=arr;//�趨�������Զ���װֵ 
	TIM4->PSC=psc;//Ԥ��Ƶ������Ƶ
	TIM4->CCMR1|=6<<12;  //CH2 PWM2ģʽ	
	TIM4->CCMR1|=6<<4;  //CH1 PWM2ģʽ	
	//TIM4->CCMR2|=6<<12;  //CH4 PWM2ģʽ	
	//TIM4->CCMR2|=6<<4;  //CH3 PWM2ģʽ	
	TIM4->CCMR1|=1<<11; //CH2Ԥװ��ʹ��	 
	TIM4->CCMR1|=1<<3; //CH1Ԥװ��ʹ��	  
	//TIM4->CCMR2|=1<<11; //CH4Ԥװ��ʹ��	 
	//TIM4->CCMR2|=1<<3; //CH3Ԥװ��ʹ��	  
	TIM4->CCER|=1<<4;   //CH2 ���ʹ��	   
	TIM4->CCER|=1;   //CH1 ���ʹ��	
	//TIM4->CCER|=1<<12;   //CH4 ���ʹ��	   
	//TIM4->CCER|=1<<8;   //CH3 ���ʹ��	
	TIM4->CR1=0x8000;   //ARPEʹ�� 
	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��2 
    TIM4->BDTR|=1<<15;   	//MOE �����ʹ��		
}
int PWM_SPEED=MOTOR_SPEED;
void Set_PWM(int moto1,int moto2)//PWM���
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
