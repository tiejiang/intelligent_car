#include "infrared.h"
void INFRARED_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	//RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	//JTAG_Set(SWD_ENABLE);	//�ر�JTAG,����SWD
	GPIOA->CRH&=0X0F000FFF;	//PA12 13 11 15���ó�����	  
	GPIOA->CRH|=0X80888000;
	
	GPIOA->CRL&=0XFF0FFFFF;
	GPIOA->CRL|=0X00800000;
	
	GPIOA->ODR&=0XFFFF47DF;	   	//PA5 PA12 13 11 15Ĭ������
}
