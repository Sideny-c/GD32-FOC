#include "iic.h"

//I2C软件延时
void delay_s(uint32_t i)
{
	while(i--);
}

void IIC_Start_AS5600()
{

	SDA_OUT();
	AS5600_SCLK_Set();
	AS5600_SDIN_Set();
	delay_s(4);
	AS5600_SDIN_Clr();
	delay_s(4);
	AS5600_SCLK_Clr();
}


void IIC_Stop_AS5600()
{
	SDA_OUT();																												
	AS5600_SDIN_Clr();
	AS5600_SCLK_Clr();																										
	delay_s(4);
	AS5600_SCLK_Set();
	AS5600_SDIN_Set();																											
	delay_s(4);
	
}

uint8_t IIC_Wait_Ack_AS5600()
{

	uint8_t ucErrTime=0;
	SDA_IN();      																				
	AS5600_SDIN_Set();	
	delay_s(1);
	AS5600_SCLK_Set();
	delay_s(1);	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_AS5600();
			return 1;
		}
	}
	AS5600_SCLK_Clr();																					
	return 0;  
}



void IIC_Ack_AS5600(void)
{
	AS5600_SCLK_Clr();
	SDA_OUT();
	AS5600_SCLK_Clr();
	delay_1ms(2);
	AS5600_SCLK_Set() ;
	delay_1ms(2);
	AS5600_SCLK_Clr();
}


void IIC_NAck_AS5600(void)
{
	AS5600_SCLK_Clr();
	SDA_OUT();
	AS5600_SDIN_Set();
	delay_1ms(2);
	AS5600_SCLK_Set() ;
	delay_1ms(2);
	AS5600_SCLK_Clr();
}	

/*
void Write_IIC_Byte_AS5600(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	AS5600_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		m=m&0x80;
		if(m==0x80)
		{AS5600_SCLK_Set();}
		else AS5600_SDIN_Clr();
			da=da<<1;
		AS5600_SCLK_Set();
		AS5600_SCLK_Clr();
	}


}

void Write_IIC_Command_AS5600(unsigned char IIC_Command)
{
   IIC_Start_AS5600();
   Write_IIC_Byte_AS5600(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack_AS5600();	
   Write_IIC_Byte_AS5600(0x00);			//write command
	IIC_Wait_Ack_AS5600();	
   Write_IIC_Byte_AS5600(IIC_Command); 
	IIC_Wait_Ack_AS5600();	
   IIC_Stop_AS5600();
}

void Write_IIC_Data_AS5600(unsigned char IIC_Data)
{
   IIC_Start_AS5600();
   Write_IIC_Byte_AS5600(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack_AS5600();	
   Write_IIC_Byte_AS5600(0x40);			//write data
	IIC_Wait_Ack_AS5600();	
   Write_IIC_Byte_AS5600(IIC_Data);
	IIC_Wait_Ack_AS5600();	
   IIC_Stop_AS5600();
}
*/
void IIC_Send_Byte_AS5600(uint8_t txd)
{                        
    uint8_t t;   
		SDA_OUT(); 	    
    AS5600_SCLK_Clr();															
    for(t=0;t<8;t++)
    {              
																								
		if((txd&0x80)>>7)
			AS5600_SDIN_Set();
		else
			AS5600_SDIN_Clr();
		txd<<=1; 	  
		delay_s(2);
		AS5600_SCLK_Set();
		delay_s(2);
		AS5600_SCLK_Clr();	
		delay_s(2);
    }	 
} 

uint8_t IIC_Read_Byte_AS5600(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();																																														//SDA设置为输入
  for(i=0;i<8;i++ )
	{
    AS5600_SCLK_Clr();
		delay_s(2);
		AS5600_SCLK_Set();
    receive<<=1;
    if(READ_SDA)receive++;   
		delay_s(1);
  }					 
    if (!ack)
        IIC_NAck_AS5600();																
    else
        IIC_Ack_AS5600(); 																		
    return receive;
}
