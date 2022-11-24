#ifndef __LED_H__
#define __LED_H__


#include "stm32f10x.h"
//我这就 两个灯 用串口输入 0或者1控制

//一个PB5 一个PE5        端口 时钟 GPIO 三件套
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



//直接寄存器控制IO
#define digitalHi(p,i)  {p->BSRR=i;} //输出为高电平
#define digitalLo(p,i)  {p->BRR=i;} //输出为低电平
#define digitalToggle(p,i)  {p->ODR=i;} //输出为反转状态



//定义控制IO的宏
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

//全部关闭
#define LED_RGBOFF \
LED1_OFF;\
LED0_ON


void LED_GPIO_Config(void);


#endif


