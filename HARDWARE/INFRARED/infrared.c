#include "infrared.h"
void INFRARED_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	//RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	//JTAG_Set(SWD_ENABLE);	//关闭JTAG,开启SWD
	GPIOA->CRH&=0X0F000FFF;	//PA12 13 11 15设置成输入	  
	GPIOA->CRH|=0X80888000;
	
	GPIOA->CRL&=0XFF0FFFFF;
	GPIOA->CRL|=0X00800000;
	
	GPIOA->ODR&=0XFFFF47DF;	   	//PA5 PA12 13 11 15默认下拉
}
