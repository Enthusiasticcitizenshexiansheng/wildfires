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

 








//超时检测
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
FLASH_ERROR("I2C 等待超时！errorCode= %d",errorCode);

	return 0;
}



