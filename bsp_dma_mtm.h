#ifndef __BSP_DMA_MTM_H__ 
#define __BSP_DMA_MTM_H__ 


#include "stm32f10x.h"
#define BUFFER_SIZE   32


#define MTM_DMA_CLK       RCC_AHBPeriph_DMA1
#define MTM_DMA_CHANNEL   DMA1_Channel6
#define MTM_DMA_FLAG_TC   DMA1_FLAG_TC6


void MtM_DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBufferl,uint16_t BufferLength);
#endif


