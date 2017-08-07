#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"
#include "key.h"   
#include "timer.h" 
#include "motor.h"
#include "ultrasonic.h"
#include "infrared.h"
int main(void)
{
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,9600);  //串口初始化 
	LED_Init();		  	 //初始化与LED连接的硬件接口
	KEY_Init();
	MOTOR_Init(7199,0);
	INFRARED_Init();
	TIM1_Init(9,7199);
	Set_PWM(MOTOR_SPEED,MOTOR_SPEED);
	//ULTRASONIC_Init();
   	while(1)
	{
		KEY_Scan();
	} 
}














