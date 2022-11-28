#ifndef __BSP_I2C_H_
#define __BSP_I2C_H_

#include "stm32f10x.h"
#include <stdio.h>


#define STM32_I2C_OWN_ADDR   0x5f





//PB6 -SCL        PB7-SDA
#define EEPROM_I2C                I2C1  
#define EEPROM_I2C_CLK            RCC_APB1Periph_I2C1
#define EEPROM_I2C_APBxC1kCmd     RCC_APB1PeriphClockCmd
#define EEPROM_I2C_BAUDRATE       400000

//i2c GPIOÒý½Åºê¶¨Òå
#define EEPROM_I2C_SCL_GPIO_CLK                   (RCC_APB2Periph_GPIOB)
#define EEPROM_I2C_SDA_GPIO_CLK                   (RCC_APB2Periph_GPIOB)

#define EEPROM_IIC_GPIO_APBxClkCmd            RCC_APB2PeriphClockCmd


#define EEPROM_I2C_SCL_GPIO_PORT        GPIOB
#define EEPROM_I2C_SCL_GPIO_PIN         GPIO_Pin_6
#define EEPROM_I2C_SDA_GPIO_PORT        GPIOB
#define EEPROM_I2C_SDA_GPIO_PIN         GPIO_Pin_7



#endif

