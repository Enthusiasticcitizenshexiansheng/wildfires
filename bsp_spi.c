#include "bsp_spi.h"
#include "bsp_usart.h"

static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



//  ��ʼ��SPI��GPIO
static void SPI_GPIO_Config(void)
{
   GPIO_InitTypeDef    GPIO_InitStructure;
	 
	//  SPI ��������ʱ������ 	// i2c GPIOʱ��
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);
	FLASH_SPI_GPIO_APBxClock_FUN(FLASH_SPI_GPIO_CLK, ENABLE);
	

	
	//��ʼ��MISO MOSI SCK
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


  //��ʼ��CS���ţ�ʹ��������ƣ������������
	GPIO_InitStructure.GPIO_Pin =FLASH_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

  FLASH_SPI_CS_HIGH;
}

  


//ģʽ
static void SPI_Mode_Config(void)
{

   SPI_InitTypeDef    SPI_InitStructure;
   SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4 ;
  //ʹ��ģʽ��  
	SPI_InitStructure.SPI_CPHA =SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
  SPI_InitStructure.SPI_CRCPolynomial=0;//CRC����
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	//˫��ȫ˫��
  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
//32��Ϊ���� 
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
//���ģʽ
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	
	SPI_Init(FLASH_SPIx,&SPI_InitStructure);
	SPI_Cmd(FLASH_SPIx,ENABLE);
}
 




void SPI_FLASH_Init(void)
{
SPI_GPIO_Config();
SPI_Mode_Config();
}	




//80mins  fresh д���дУ��
//���� ������ һ���ֽ�
uint8_t SPI_FLASH_Send_Byte(uint8_t data)
{
	
	
	SPITimeout = SPIT_FLAG_TIMEOUT ;
	//��鲢�ȴ�  �򻺳���д������
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE)==RESET)
	{
	if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	//Tx��������
	SPI_I2S_SendData(FLASH_SPIx,data);

	
	SPITimeout = SPIT_FLAG_TIMEOUT ;
  //RXNE  ��ⷢ���Ƿ����   RESET RX=�ǿ�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE)==RESET);
		{
	if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	
	//��ȡ������ �����������
	return SPI_I2S_ReceiveData(FLASH_SPIx);

}





uint8_t  SPI_FLASH_Read_Byte(void)
{
return SPI_FLASH_Send_Byte(DUMMY);
}


//��ȡID��
uint32_t SPI_Read_ID(void)
{
	uint32_t flash_id;
	
	//Ƭѡʹ�� �͵�ƽʹ��
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_JEDEC_ID);

	//��ȡ�����ֽ�
	flash_id = SPI_FLASH_Send_Byte(DUMMY);
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	flash_id <<=8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
	
}




//��ʱ���
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
FLASH_ERROR("SPI �ȴ���ʱ��errorCode= %d",errorCode);

	return 0;
}



