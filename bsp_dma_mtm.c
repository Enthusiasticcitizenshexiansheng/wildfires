#include "bsp_dma_mtm.h"



//����Ҫ������ �����Ҫ�����
// 4*8 =32
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]={
  0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
  0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
  0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10
};

//Ŀ��Ĵ���
uint32_t aDST_Buffer[BUFFER_SIZE];




// uint32_t DMA_PeripheralBaseAddr;   //�����ַ
//  uint32_t DMA_MemoryBaseAddr;      //�洢����ַ
//  uint32_t DMA_DIR;                 //���䷽��
//  uint32_t DMA_BufferSize;          //������Ŀ
//  uint32_t DMA_PeripheralInc;       //�����ַ����ģʽ
//  uint32_t DMA_MemoryInc;           //�洢����ַ����ģʽ
//  uint32_t DMA_PeripheralDataSize;  //�������ݿ��
//  uint32_t DMA_MemoryDataSize;      //�洢�����ݿ��

//  uint32_t DMA_Mode;                //ģʽѡ��
//  uint32_t DMA_Priority;            //ͨ�����ȼ�
//  uint32_t DMA_M2M;                 //�洢�����洢��ģʽ


void  MtM_DMA_Config(void)
{
    DMA_InitTypeDef  DMA_InitStruct;
	
	//������
	  RCC_AHBPeriphClockCmd(MTM_DMA_CLK, ENABLE); 
	
	//�����ַ//�洢����ַ //���䷽��
	DMA_InitStruct.DMA_PeripheralBaseAddr =(uint32_t)aSRC_Const_Buffer;
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;
	DMA_InitStruct.DMA_DIR =DMA_DIR_PeripheralSRC;
	
	
	//������Ŀ  �����ַ���� �������ݿ��   �洢����ַ����  ���(��λ)
	DMA_InitStruct.DMA_BufferSize =  BUFFER_SIZE;
  DMA_InitStruct.DMA_PeripheralInc= DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryInc =  DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_MemoryDataSize= DMA_MemoryDataSize_Word;

	
	//ģʽѡ�񣨷�һ�λ�ѭ���� ͨ�����ȼ� �洢�����洢��ģʽ
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High ;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;


  DMA_Init(MTM_DMA_CHANNEL,&DMA_InitStruct);
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);	
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);
	

}


////�Ƚ��жϺ��� ͬ1 ��ͬ0
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBufferl,uint16_t BufferLength)
{

   while(BufferLength--)
	 {
	 if(*pBuffer != *pBufferl)
	 {return 0;}
	 pBuffer++;
	 pBufferl++;
	 }
return 1;
}




