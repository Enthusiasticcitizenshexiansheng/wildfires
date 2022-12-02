#ifndef __BSP_I2C_H_
#define __BSP_I2C_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x_i2c.h"
#define STM32_I2C_OWN_ADDR   0x5f

//EEPROM 设备地址  写地址
#define EEPROM_ADDR  0xA0


//PB6 -SCL        PB7-SDA
#define EEPROM_I2C                I2C1  
#define EEPROM_I2C_CLK            RCC_APB1Periph_I2C1
#define EEPROM_I2C_APBxC1kCmd     RCC_APB1PeriphClockCmd
#define EEPROM_I2C_BAUDRATE       400000

//i2c GPIO引脚宏定义
#define EEPROM_I2C_SCL_GPIO_CLK                   (RCC_APB2Periph_GPIOB)
#define EEPROM_I2C_SDA_GPIO_CLK                   (RCC_APB2Periph_GPIOB)

#define EEPROM_IIC_GPIO_APBxClkCmd            RCC_APB2PeriphClockCmd


#define EEPROM_I2C_SCL_GPIO_PORT        GPIOB
#define EEPROM_I2C_SCL_GPIO_PIN         GPIO_Pin_6
#define EEPROM_I2C_SDA_GPIO_PORT        GPIOB
#define EEPROM_I2C_SDA_GPIO_PIN         GPIO_Pin_7


void I2C_EE_Config(void);
void EEPROM_Byte_Write(uint8_t addr,uint8_t data);
void EEPROM_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead);
void EEPROM_Page_Write(uint8_t addr,uint8_t *data,uint8_t numByteToWrite);
void EEPROM_WaitForWriteEnd(void);
#endif

