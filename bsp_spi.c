#include "bsp_spi.h"
#include "bsp_usart.h"

static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



//  初始化SPI的GPIO
static void SPI_GPIO_Config(void)
{
   GPIO_InitTypeDef    GPIO_InitStructure;
	 
	//  SPI 串口外设时钟配置 	// i2c GPIO时钟
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);
	FLASH_SPI_GPIO_APBxClock_FUN(FLASH_SPI_GPIO_CLK, ENABLE);
	

	
	//初始化MISO MOSI SCK
  GPIO_InitStructure.GPIO_Pin =	FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin =	FLASH_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =FLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);


  //初始化CS引脚，使用软件控制，设置推挽输出
	GPIO_InitStructure.GPIO_Pin =FLASH_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

  FLASH_SPI_CS_HIGH;
}

  


//模式
static void SPI_Mode_Config(void)
{

   SPI_InitTypeDef    SPI_InitStructure;
   SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4 ;
  //使用模式三  
	SPI_InitStructure.SPI_CPHA =SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
  SPI_InitStructure.SPI_CRCPolynomial=0;//CRC不用
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	//双线全双工
  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
//32作为主机 
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
//软件模式
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	
	SPI_Init(FLASH_SPIx,&SPI_InitStructure);
	SPI_Cmd(FLASH_SPIx,ENABLE);
}
 




void SPI_FLASH_Init(void)
{
SPI_GPIO_Config();
SPI_Mode_Config();
}	




//80mins  fresh 写入读写校验
//发送 并接收 一个字节
uint8_t SPI_FLASH_Send_Byte(uint8_t data)
{
	
	
	SPITimeout = SPIT_FLAG_TIMEOUT ;
	//检查并等待  向缓冲区写入数据
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE)==RESET)
	{
	if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	//Tx缓存区空
	SPI_I2S_SendData(FLASH_SPIx,data);

	
	SPITimeout = SPIT_FLAG_TIMEOUT ;
  //RXNE  检测发送是否完成   RESET RX=非空
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE)==RESET);
		{
	if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	
	//读取缓冲区 接收这个数据
	return SPI_I2S_ReceiveData(FLASH_SPIx);

}





uint8_t  SPI_FLASH_Read_Byte(void)
{
return SPI_FLASH_Send_Byte(DUMMY);
}


//读取ID号
uint32_t SPI_Read_ID(void)
{
	uint32_t flash_id;
	
	//片选使能 低电平使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_JEDEC_ID);

	//读取三个字节
	flash_id = SPI_FLASH_Send_Byte(DUMMY);
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
	
}







//35min往后
//1.1擦除 FLASH 指定扇区
void SPI_Erase_Sector(uint32_t addr)
{
	
	SPI_Write_Enable();
//片选使能 低电平使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(ERASE_SECTOR);

	//三个字节 发送地址    0x00 00 00 aa
	SPI_FLASH_Send_Byte((addr>>16)&0xff);
  SPI_FLASH_Send_Byte((addr>>8)&0xff);
	SPI_FLASH_Send_Byte(addr);
	FLASH_SPI_CS_HIGH;
	
	SPI_WaiteFowWriteEnd(); //等内部时序
}
 



//2.读取FLASH 内容
void SPI_Read_Data(uint32_t addr,uint8_t *readBuff,uint32_t numByteToRead)
{
//片选使能 低电平使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_DATA);

	//三个字节 发送地址    0x00 00 00 aa
	SPI_FLASH_Send_Byte((addr>>16)&0xff);
  SPI_FLASH_Send_Byte((addr>>8)&0xff);
	SPI_FLASH_Send_Byte(addr&0xff);
	
	while(numByteToRead--)
	{
	*readBuff = SPI_FLASH_Send_Byte(DUMMY);
	readBuff;
	}
	
	FLASH_SPI_CS_HIGH;
	
	
}
 



//3.写入  Page Program 256 0x02 空扇区
void SPI_Write_Data(uint32_t addr,uint8_t *writeBuff,uint32_t numByteToWrite)
{
		SPI_Write_Enable();

//片选使能 低电平使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(WRIYE_DATA);

	//三个字节 发送地址    0x00 00 00 aa
	SPI_FLASH_Send_Byte((addr>>16)&0xff);
  SPI_FLASH_Send_Byte((addr>>8)&0xff);
	SPI_FLASH_Send_Byte(addr&0xff);
	
	while(numByteToWrite--)
	{
 SPI_FLASH_Send_Byte(*writeBuff);
	writeBuff++;
	}
	
	FLASH_SPI_CS_HIGH;
	SPI_WaiteFowWriteEnd();//等待时序命令
	
}




//1.3FLASH 使能

void SPI_Write_Enable(void)
{
	FLASH_SPI_CS_LOW;
  SPI_FLASH_Send_Byte(WRITE_ENABLE);
	FLASH_SPI_CS_HIGH;	
}

//1.2等待内部时序操作完成
void SPI_WaiteFowWriteEnd(void)
{
	uint8_t status_reg=0;  //状态寄存器
	//片选使能 低电平使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_STATUS);

	do{
	//读取 看busy位 是否忙碌
	status_reg = SPI_FLASH_Send_Byte(DUMMY);
	}
	while((status_reg &0x01) == 1); //忙
	
	FLASH_SPI_CS_HIGH;

}

//超时检测
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
FLASH_ERROR("SPI 等待超时！errorCode= %d",errorCode);

	return 0;
}



