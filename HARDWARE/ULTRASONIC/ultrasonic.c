#include "ultrasonic.h"
#include "usart.h"
void TRIG_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1;       //TIM2ʱ��ʹ��    
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ��      
	GPIOA->CRL&=0XFFFFF0FF;//
	GPIOA->CRL|=0X00000B00;//PA2����
	
	TIM2->ARR=arr;//�趨�������Զ���װֵ 
	TIM2->PSC=psc;//Ԥ��Ƶ������Ƶ
	//TIM2->CCMR1|=6<<12;  //CH2 PWM2ģʽ	
	//TIM2->CCMR1|=6<<4;  //CH1 PWM2ģʽ	
	//TIM2->CCMR2|=6<<12;  //CH4 PWM2ģʽ	
	TIM2->CCMR2|=6<<4;  //CH3 PWM2ģʽ	
	//TIM2->CCMR1|=1<<11; //CH2Ԥװ��ʹ��	 
	//TIM2->CCMR1|=1<<3; //CH1Ԥװ��ʹ��	  
	//TIM2->CCMR2|=1<<11; //CH4Ԥװ��ʹ��	 
	TIM2->CCMR2|=1<<3; //CH3Ԥװ��ʹ��	  
	//TIM2->CCER|=1<<4;   //CH2 ���ʹ��	   
	//TIM2->CCER|=1;   //CH1 ���ʹ��	
	//TIM2->CCER|=1<<12;   //CH4 ���ʹ��	   
	TIM2->CCER|=1<<8;   //CH3 ���ʹ��	
	TIM2->CR1=0x8000;   //ARPEʹ�� 
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2 
    TIM2->BDTR|=1<<15;   	//MOE �����ʹ��	
}

void ECHO_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;   	//TIM5 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��  
	 
	GPIOA->CRL&=0XF0FFFFFF;	//PA6 ���֮ǰ����  
	GPIOA->CRL|=0X08000000;	//PA6 ����   
	GPIOA->ODR|=0<<0;		//PA6 ����
	  
 	TIM3->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM3->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM3->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM3->CCMR1|=1<<4; 		//IC1F=0001 ���������˲��� ��Fck_int������2���¼�����Ч
 	TIM3->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM3->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM3->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM3->DIER|=1<<1;   	//�������ж�				
	TIM3->DIER|=1<<0;   	//��������ж�	
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��1
	MY_NVIC_Init(2,0,TIM3_IRQn,2);//��ռ2�������ȼ�0����2	
}
u8  TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ
u16 Distance=46;
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM3->SR;
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//���
		{	    
			if(TIM3CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH1_CAPTURE_VAL=0XFFFF;
					Distance = TIM3CH1_CAPTURE_VAL*34/2000;
					printf("HIGH:%d cm\r\n",Distance);
					TIM3CH1_CAPTURE_STA=0;			//������һ�β���
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//����1���������¼�
		{	
			if(TIM3CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM3CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM3->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
			    TIM3CH1_CAPTURE_VAL=TIM3->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
				Distance = TIM3CH1_CAPTURE_VAL*34/2000;
				printf("HIGH:%d cm\r\n",Distance);
				TIM3CH1_CAPTURE_STA=0;			//������һ�β���
			}else  								//��δ��ʼ,��һ�β���������
			{ 
				TIM3CH1_CAPTURE_VAL=0;
				TIM3CH1_CAPTURE_STA=0X40;		//��ǲ�����������
				TIM3->CNT=0;					//���������
				TIM3->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز��� 
			}		    
		}			     	    					   
 	}
	TIM3->SR=0;//����жϱ�־λ 	    
}
void ULTRASONIC_Init(void)
{
	TRIG_Init(28799,149);//trig�ź����� 60ms
	ECHO_Init(0XFFFF,72-1);
	TIM2->CCR3=5;//trig������10us
}
