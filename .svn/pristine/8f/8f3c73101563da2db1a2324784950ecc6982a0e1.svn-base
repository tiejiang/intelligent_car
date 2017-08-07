#include "ultrasonic.h"
#include "usart.h"
void TRIG_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1;       //TIM2时钟使能    
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能      
	GPIOA->CRL&=0XFFFFF0FF;//
	GPIOA->CRL|=0X00000B00;//PA2复用
	
	TIM2->ARR=arr;//设定计数器自动重装值 
	TIM2->PSC=psc;//预分频器不分频
	//TIM2->CCMR1|=6<<12;  //CH2 PWM2模式	
	//TIM2->CCMR1|=6<<4;  //CH1 PWM2模式	
	//TIM2->CCMR2|=6<<12;  //CH4 PWM2模式	
	TIM2->CCMR2|=6<<4;  //CH3 PWM2模式	
	//TIM2->CCMR1|=1<<11; //CH2预装载使能	 
	//TIM2->CCMR1|=1<<3; //CH1预装载使能	  
	//TIM2->CCMR2|=1<<11; //CH4预装载使能	 
	TIM2->CCMR2|=1<<3; //CH3预装载使能	  
	//TIM2->CCER|=1<<4;   //CH2 输出使能	   
	//TIM2->CCER|=1;   //CH1 输出使能	
	//TIM2->CCER|=1<<12;   //CH4 输出使能	   
	TIM2->CCER|=1<<8;   //CH3 输出使能	
	TIM2->CR1=0x8000;   //ARPE使能 
	TIM2->CR1|=0x01;    //使能定时器2 
    TIM2->BDTR|=1<<15;   	//MOE 主输出使能	
}

void ECHO_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;   	//TIM5 时钟使能 
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟  
	 
	GPIOA->CRL&=0XF0FFFFFF;	//PA6 清除之前设置  
	GPIOA->CRL|=0X08000000;	//PA6 输入   
	GPIOA->ODR|=0<<0;		//PA6 下拉
	  
 	TIM3->ARR=arr;  		//设定计数器自动重装值   
	TIM3->PSC=psc;  		//预分频器 

	TIM3->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM3->CCMR1|=1<<4; 		//IC1F=0001 配置输入滤波器 以Fck_int采样，2个事件后有效
 	TIM3->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

	TIM3->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM3->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

	TIM3->DIER|=1<<1;   	//允许捕获中断				
	TIM3->DIER|=1<<0;   	//允许更新中断	
	TIM3->CR1|=0x01;    	//使能定时器1
	MY_NVIC_Init(2,0,TIM3_IRQn,2);//抢占2，子优先级0，组2	
}
u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH1_CAPTURE_VAL;	//输入捕获值
u16 Distance=46;
//定时器3中断服务程序
void TIM3_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM3->SR;
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	    
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL=0XFFFF;
					Distance = TIM3CH1_CAPTURE_VAL*34/2000;
					printf("HIGH:%d cm\r\n",Distance);
					TIM3CH1_CAPTURE_STA=0;			//开启下一次捕获
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//捕获1发生捕获事件
		{	
			if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM3->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			    TIM3CH1_CAPTURE_VAL=TIM3->CCR1;	//获取当前的捕获值.
				Distance = TIM3CH1_CAPTURE_VAL*34/2000;
				printf("HIGH:%d cm\r\n",Distance);
				TIM3CH1_CAPTURE_STA=0;			//开启下一次捕获
			}else  								//还未开始,第一次捕获上升沿
			{ 
				TIM3CH1_CAPTURE_VAL=0;
				TIM3CH1_CAPTURE_STA=0X40;		//标记捕获到了上升沿
				TIM3->CNT=0;					//计数器清空
				TIM3->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获 
			}		    
		}			     	    					   
 	}
	TIM3->SR=0;//清除中断标志位 	    
}
void ULTRASONIC_Init(void)
{
	TRIG_Init(28799,149);//trig信号周期 60ms
	ECHO_Init(0XFFFF,72-1);
	TIM2->CCR3=5;//trig脉冲宽度10us
}
