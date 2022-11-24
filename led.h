#ifndef __LED_H__
#define __LED_H__


#include "stm32f10x.h"
//����� ������ �ô������� 0����1����

//һ��PB5 һ��PE5        �˿� ʱ�� GPIO ������
#define LED0_GPIO_PIN       GPIO_Pin_5
#define LED0_GPIO_CLK       RCC_APB2Periph_GPIOB
#define LED0_GPIO_PORT      GPIOB


#define LED1_GPIO_PIN       GPIO_Pin_5
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOE
#define LED1_GPIO_PORT      GPIOE






#define ON  1
#define OFF 0

#define LED0(a) \
                 if(a)GPIO_ResetBits(LED0_GPIO_PORT ,LED0_GPIO_PIN );\
                    else GPIO_SetBits(LED0_GPIO_PORT,LED0_GPIO_PIN  );
										
										
#define LED1(a) \
if(a)GPIO_ResetBits(LED1_GPIO_PORT ,LED1_GPIO_PIN );\
else GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN  );



//ֱ�ӼĴ�������IO
#define digitalHi(p,i)  {p->BSRR=i;} //���Ϊ�ߵ�ƽ
#define digitalLo(p,i)  {p->BRR=i;} //���Ϊ�͵�ƽ
#define digitalToggle(p,i)  {p->ODR=i;} //���Ϊ��ת״̬



//�������IO�ĺ�
#define LED0_TOGGLE  digitalToggle(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_OFF     digitalHi(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_ON      digitalLo(LED0_GPIO_PORT,LED0_GPIO_PIN)

#define LED1_TOGGLE  digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF     digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON      digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define  LED_RED \
               LED0_ON;\
							 LED1_OFF
							

#define  LED_BULE \
               LED0_OFF;\
							 LED1_ON

//ȫ���ر�
#define LED_RGBOFF \
LED1_OFF;\
LED0_ON


void LED_GPIO_Config(void);


#endif


