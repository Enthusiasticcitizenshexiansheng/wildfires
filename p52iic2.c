//p52iic 字节对齐函数的书写
#define I2C_PageSize 8
    //                       地址        传入字节数       一次传入多少字节 可能大于8
void I2C_EE_BufferWrite(u8* pBuffer,u8 WriteAddr,u16 NumByteToWrite)
{
	u8 NumByteToWrite = 0,NumOfSingle=0,Addr =0,count =0;
	
	//看等不等于0（看对齐）  考虑对不对齐
	Addr = WriteAddr%I2C_PageSize;
	count =I2C_PageSize - Addr;//对齐不用这个
	//超8页  10/8 = 1  以及剩下2字节
	NumOfPage = NumByteToWrite /I2C_PageSize;
	NumOfSingle=NumByteToWrite %I2C_PageSize;
	
	if(Addr ==0)
	{
		if(NumOfPage==0)//页数等于0 直接写
		{
			I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			I2C_EE_WaitEepromStandbyState();//等待处理函数
		}
		//地址不对齐的处理
		else
		{
			//页的
			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr +=I2C_PageSize;//size 大小8 或者 12 自定义的
				pBuffer += I2C_PageSize;
			}
			//字节的
			if(NumOfSingle!=0)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
					I2C_EE_WaitEepromStandbyState();
			}
		}
	}
	
	
	
	
	//不对齐
	else
	{
		if(NumOfPage==0)
		{
			I2C_EE_PageWrite(pBuffer,WriteAddr,);
		    I2C_EE_WaitEepromStandbyState();
		}
		//看看有多少页
		else{
			NumByteToWrite -=count;
			NumOfPage =NumByteToWrite / I2C_PageSize;
			NumOfSingle=NumByteToWrite%I2C_PageSize;
			//count是不对齐的地址   从不对齐的地方写入数据
			if(count !=0)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,count);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr +=count;
				pBuffer += count;
			}	
			//页写入  从对齐开始写 看写多少页
			while(NumOfPage--)
			{   I2C_EE_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr +=I2C_PageSize;
			}
			
			//剩余的字节写入
			if(NumOfSingle !=0)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
				I2C_EE_WaitEepromStandbyState();
			}		
		}
	}
}









//等待处理函数
void I2C_EE_WaitEepromStandbyState(void)
{
	vu16 SR1_Tmp=0;
	I2CTimeout =I2CT_LONG_TIMEOUT;
	
	do {
		I2CTimeout--;
		I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
		
		SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx,I2C_Register_SR1);
		I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	}while(!(I2C_ReadRegister(EEPROM_I2Cx,I2C_Register_SR1)&0x0002)&&(I2CTimeout));
		
		I2C_ClearFlag(EEPROM_I2Cx,I2C_FLAG_AF);
		I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	}
	
	
	
	
	
}














