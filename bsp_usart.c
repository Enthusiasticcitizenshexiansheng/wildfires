#include "bsp_usart.h"

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
static void NVIC_Configuration(void)
{
	//��ʱ����
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//�ж�Դ
  NVIC_InitStructure.NVIC_IRQChannel =DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void USART_Config(void)
{
   GPIO_InitTypeDef    GPIO_InitStructure;
	 USART_InitTypeDef   USART_InitStructure;
	
	//GPIOʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	
	//��������ʱ��
	 DEBUG_USART_APBxC1kCmd(DEBUG_USART_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =	DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin =	DEBUG_USART_RX_GPIO_PIN;
   GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_IN_FLOATING;
   GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);





 
   //���ô�����ز���
	 //������
	 USART_InitStructure.USART_BaudRate=DEBUG_USART_BAUDRATE;
	 
	 //�����������ֳ�
	 USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	 
	 //ֹͣ
	 USART_InitStructure.USART_StopBits=USART_StopBits_1;
	 
	 //У��
	 USART_InitStructure.USART_Parity=USART_Parity_No;
 
   //Ӳ��������
	 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	 
	 
	 //����ģʽ
	 USART_InitStructure.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
	 
   USART_Init(DEBUG_USARTx,&USART_InitStructure);


 
 
 
   //�����ж����ȼ�
 NVIC_Configuration();
 
   //ʹ�ܴ��ڽ����ж�
 USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE,ENABLE);
 
  //ʹ�ܴ���
 USART_Cmd(DEBUG_USARTx,ENABLE);
}




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



