#include "bsp_dma_mtm.h"



//定义要传数据 定义好要存变量
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

//目标寄存器
uint32_t aDST_Buffer[BUFFER_SIZE];




// uint32_t DMA_PeripheralBaseAddr;   //外设地址
//  uint32_t DMA_MemoryBaseAddr;      //存储器地址
//  uint32_t DMA_DIR;                 //传输方向
//  uint32_t DMA_BufferSize;          //传输数目
//  uint32_t DMA_PeripheralInc;       //外设地址增量模式
//  uint32_t DMA_MemoryInc;           //存储器地址增量模式
//  uint32_t DMA_PeripheralDataSize;  //外设数据宽度
//  uint32_t DMA_MemoryDataSize;      //存储器数据宽度

//  uint32_t DMA_Mode;                //模式选择
//  uint32_t DMA_Priority;            //通道优先级
//  uint32_t DMA_M2M;                 //存储器到存储器模式


void  MtM_DMA_Config(void)
{
    DMA_InitTypeDef  DMA_InitStruct;
	
	//开总线
	  RCC_AHBPeriphClockCmd(MTM_DMA_CLK, ENABLE); 
	
	//外设地址//存储器地址 //传输方向
	DMA_InitStruct.DMA_PeripheralBaseAddr =(uint32_t)aSRC_Const_Buffer;
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;
	DMA_InitStruct.DMA_DIR =DMA_DIR_PeripheralSRC;
	
	
	//传输数目  外设地址增量 外设数据宽度   存储器地址增量  存宽(单位)
	DMA_InitStruct.DMA_BufferSize =  BUFFER_SIZE;
  DMA_InitStruct.DMA_PeripheralInc= DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryInc =  DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_MemoryDataSize= DMA_MemoryDataSize_Word;

	
	//模式选择（发一次或循环） 通道优先级 存储器到存储器模式
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High ;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;


  DMA_Init(MTM_DMA_CHANNEL,&DMA_InitStruct);
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);	
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);
	

}


////比较判断函数 同1 不同0
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




