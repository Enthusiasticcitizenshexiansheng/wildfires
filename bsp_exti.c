#include "bsp_exti.h"

static void EXTI_NVIC_Config(void)
{
   NVIC_InitTypeDef  NVIC_InitSture;
 
	//主优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitSture.NVIC_IRQChannel= EXTI3_IRQn;
  NVIC_InitSture.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitSture.NVIC_IRQChannelSubPriority  =1;//主次优先级
	NVIC_InitSture.NVIC_IRQChannelCmd =ENABLE;
	
	NVIC_Init(&NVIC_InitSture);
}

//初始化GPIO  初始化EXTI 初始化NVIC 编写中断服务函数  manin函数
void EXTI_Key_Config(void)
{
 GPIO_InitTypeDef GPIO_InitSture;
 EXTI_InitTypeDef EXTI_InitSture;
	
	//配置中断优先级
	
 EXTI_NVIC_Config();
	
	
	
	
 RCC_APB2PeriphClockCmd( KEY1_INT_GPIO_CLK , ENABLE);
 GPIO_InitSture.GPIO_Pin=	KEY1_INT_GPIO_PIN;
 GPIO_InitSture.GPIO_Mode=	GPIO_Mode_IN_FLOATING;
 GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitSture);


	
	//初始化EXTI
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIOE, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

  EXTI_InitSture.EXTI_Line =EXTI_Line3 ;
  EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitSture.EXTI_LineCmd= ENABLE;
	
	EXTI_Init(&EXTI_InitSture);




}


