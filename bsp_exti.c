#include "bsp_exti.h"

static void EXTI_NVIC_Config(void)
{
   NVIC_InitTypeDef  NVIC_InitSture;
 
	//�����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitSture.NVIC_IRQChannel= EXTI3_IRQn;
  NVIC_InitSture.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitSture.NVIC_IRQChannelSubPriority  =1;//�������ȼ�
	NVIC_InitSture.NVIC_IRQChannelCmd =ENABLE;
	
	NVIC_Init(&NVIC_InitSture);
}

//��ʼ��GPIO  ��ʼ��EXTI ��ʼ��NVIC ��д�жϷ�����  manin����
void EXTI_Key_Config(void)
{
 GPIO_InitTypeDef GPIO_InitSture;
 EXTI_InitTypeDef EXTI_InitSture;
	
	//�����ж����ȼ�
	
 EXTI_NVIC_Config();
	
	
	
	
 RCC_APB2PeriphClockCmd( KEY1_INT_GPIO_CLK , ENABLE);
 GPIO_InitSture.GPIO_Pin=	KEY1_INT_GPIO_PIN;
 GPIO_InitSture.GPIO_Mode=	GPIO_Mode_IN_FLOATING;
 GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitSture);


	
	//��ʼ��EXTI
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIOE, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

  EXTI_InitSture.EXTI_Line =EXTI_Line3 ;
  EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitSture.EXTI_LineCmd= ENABLE;
	
	EXTI_Init(&EXTI_InitSture);




}


