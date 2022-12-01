#include "bsp_i2c.h"


/*
//����1-USART1
#define DEBUG_USARTx             USART1   
#define DEBUG_USART_CLK          RCC_APB2Periph_USART1
#define DEBUG_USART_APBxC1kCmd   RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE     115200

//USART GPIO���ź궨��
#define DEBUG_USART_GPIO_CLK  (RCC_APB2Periph_GPIOA)
#define DEBUG_USART            RCC_APB2PeriphClockCmd


#define DEBUG_USART_TX_GPIO_PORT   GPIOA
#define DEBUG_USART_TX_GPIO_PIN    GPIO_Pin_9
#define DEBUG_USART_TX_GPIO_PORT   GPIOA
#define DEBUG_USART_TX_GPIO_PIN    GPIO_Pin_10


#define DEBUG_USART_IRQ           USART1_IRQn
#define DEBUG_USART_IRQHandler    USART1_IRQHandler

*/
//static void NVIC_Configuration(void)
//{
//	//��ʱ����
//	NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	//�ж�Դ
//  NVIC_InitStructure.NVIC_IRQChannel =DEBUG_USART_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}


void I2C_EE_Config(void)
{
   GPIO_InitTypeDef    GPIO_InitStructure;
	 I2C_InitTypeDef     I2C_InitStructure;
	
	// i2c GPIOʱ��
	EEPROM_I2C_APBxC1kCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
	
	
	//  i2c ��������ʱ������  scl pb6
	 EEPROM_I2C_APBxC1kCmd(EEPROM_I2C_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =	EEPROM_I2C_SCL_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	
	//sda  pb7
	 GPIO_InitStructure.GPIO_Pin =	EEPROM_I2C_SDA_GPIO_PIN;
   GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_OD;
   GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);





 
   //2����i2c�Ĺ���ģʽ
	 //ʹ��Ӧ��  7λ��ַ ������(scl)
	I2C_InitStructure.I2C_Ack= I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed=EEPROM_I2C_BAUDRATE;
  
	
	 //���� ģʽ iic�����ַ
  I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2 ;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C; 
	I2C_InitStructure.I2C_OwnAddress1=STM32_I2C_OWN_ADDR;
	
	
	
	
	
	
 I2C_Init(EEPROM_I2C,&I2C_InitStructure);
 I2C_Cmd(EEPROM_I2C,ENABLE);
}




//�� EEPROM д��һ���ֽ�
//�豸��ַ���̶��� �ڴ��ַ ����
void EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
  //��ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
	//��� �ɹ�����
	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
  //EV5����⵽  Ϊ�����豸��ַ
  
  //����  д����
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter); 

	
	//���ʱ��  EV6  ����Ҫ�����Ĵ洢��Ԫ��ַ
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);

// ���� ���� �ڴ浥Ԫ��ַ
  I2C_SendData(EEPROM_I2C,addr);

	
//���ʱ��  EV8  
  while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);

// ���� ���� �ڴ浥Ԫ��ַ
  I2C_SendData(EEPROM_I2C,addr);

//���EV8-2   
 while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);


//����
  I2C_GenerateSTOP(EEPROM_I2C,ENABLE);


}




/*

//1������ �� ����  
// ���ͺ��ж��Ƿ���
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data)
{
     USART_SendData(pUSARTx,data);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0����

}



//2�����������ֽ�����
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint8_t data)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (data&0xff00)>>8;
	temp_l = data&0xff;
	
    USART_SendData(pUSARTx,temp_h);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0����

	  USART_SendData(pUSARTx,temp_l);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0����

	
}





//3�����Ͱ�λ���ݵ�����
void Usart_Sendshuzu(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
 { Usart_SendByte(pUSARTx,*array++);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);//==0����
 }
}




//4�������ַ���
void Usart_SendStr(USART_TypeDef* pUSARTx,uint8_t *str)
{
	uint8_t i=0;
	//���㲻������ ����ѭ��
do
 { 
	 Usart_SendByte(pUSARTx,*(str+i));
	 i++;
 }while(*(str+i) != '\0');
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);//==0����
}


//5���printf�Ĺ̼�����ú���
int fputc(int ch,FILE *f)
{

	//���� �� �ȴ�
	USART_SendData(DEBUG_USARTx,(uint8_t)ch);
	
	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET)

	return (ch);
}	


//6�ض���c��scanf������ �����scanf getchar
int fgetc(FILE *f)
{

	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE)==RESET);
	
	return (int)USART_ReceiveData(DEBUG_USARTx);

}

*/

