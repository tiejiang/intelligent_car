#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//按键初始化函数 
//PA0.15和PC5 设置成输入
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	//RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	//JTAG_Set(SWD_ENABLE);	//关闭JTAG,开启SWD
	GPIOA->CRL&=0X0FFFFFFF;	//PA7设置成输入	  
	GPIOA->CRL|=0X80000000;   
	GPIOA->CRH&=0XFFFFFFF0;	//PA8设置成输入	  
	GPIOA->CRH|=0X00000008; 			 
	GPIOA->ODR&=0XFFFFFE7F;	   	//PA15上拉,PA0默认下拉
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
int MODE=STOP;
void KEY_Scan()
{
	if(START)
	{
		if(LINE)
			MODE=INFRARED;
		else
			MODE=ULTRA;
	}else
	MODE=STOP;
}




















