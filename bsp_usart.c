#include "bsp_usart.h"

/*
//串口1-USART1
#define DEBUG_USARTx             USART1   
#define DEBUG_USART_CLK          RCC_APB2Periph_USART1
#define DEBUG_USART_APBxC1kCmd   RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE     115200

//USART GPIO引脚宏定义
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
	//延时程序
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//中断源
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
	
	//GPIO时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	
	//串口外设时钟
	 DEBUG_USART_APBxC1kCmd(DEBUG_USART_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =	DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin =	DEBUG_USART_RX_GPIO_PIN;
   GPIO_InitStructure.GPIO_Mode=	GPIO_Mode_IN_FLOATING;
   GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);





 
   //配置串口相关参数
	 //波特率
	 USART_InitStructure.USART_BaudRate=DEBUG_USART_BAUDRATE;
	 
	 //配置针数据字长
	 USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	 
	 //停止
	 USART_InitStructure.USART_StopBits=USART_StopBits_1;
	 
	 //校验
	 USART_InitStructure.USART_Parity=USART_Parity_No;
 
   //硬件控制流
	 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	 
	 
	 //工作模式
	 USART_InitStructure.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
	 
   USART_Init(DEBUG_USARTx,&USART_InitStructure);


 
 
 
   //串口中断优先级
 NVIC_Configuration();
 
   //使能串口接受中断
 USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE,ENABLE);
 
  //使能串口
 USART_Cmd(DEBUG_USARTx,ENABLE);
}




//1、串口 加 数据  
// 发送和判断是否发完
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data)
{
     USART_SendData(pUSARTx,data);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0跳出

}



//2、发送两个字节数据
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint8_t data)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (data&0xff00)>>8;
	temp_l = data&0xff;
	
    USART_SendData(pUSARTx,temp_h);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0跳出

	  USART_SendData(pUSARTx,temp_l);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);//==0跳出

	
}





//3、发送八位数据的数组
void Usart_Sendshuzu(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
 { Usart_SendByte(pUSARTx,*array++);
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);//==0跳出
 }
}




//4、发送字符串
void Usart_SendStr(USART_TypeDef* pUSARTx,uint8_t *str)
{
	uint8_t i=0;
	//满足不等条件 继续循环
do
 { 
	 Usart_SendByte(pUSARTx,*(str+i));
	 i++;
 }while(*(str+i) != '\0');
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);//==0跳出
}


//5针对printf的固件库调用函数
int fputc(int ch,FILE *f)
{

	//发送 和 等待
	USART_SendData(DEBUG_USARTx,(uint8_t)ch);
	
	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET)

	return (ch);
}	


//6重定向c库scanf到串口 后可用scanf getchar
int fgetc(FILE *f)
{

	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE)==RESET);
	
	return (int)USART_ReceiveData(DEBUG_USARTx);

}



