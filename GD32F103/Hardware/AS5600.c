
#include "AS5600.h"  
#include "iic.h"  

//原始数据获取0~4096
uint8_t AS5600_IIC_Read_OneByte(uint8_t deviceaddr,uint8_t readaddr)
{
  uint8_t temp;
  IIC_Start_AS5600();
  IIC_Send_Byte_AS5600(deviceaddr&0xfe);
  IIC_Wait_Ack_AS5600();
  IIC_Send_Byte_AS5600(readaddr);
  IIC_Wait_Ack_AS5600();

  IIC_Start_AS5600();
  IIC_Send_Byte_AS5600(deviceaddr|0x01);
  IIC_Wait_Ack_AS5600();
  temp=IIC_Read_Byte_AS5600(0);
  IIC_Stop_AS5600();
  return temp;
}

//角度转换为0~2PI
int value = 0; 
float Angle = 0.0;
float _2PI=6.283185f;
float _PI=3.1415927f;
float angle_get(void)
{
	value =  AS5600_IIC_Read_OneByte((0x36<<1),0x0e); 
	value <<= 8;
	value |= AS5600_IIC_Read_OneByte((0x36<<1),0x0f); 
	Angle = value / 4096.0f * _2PI ;
	if(Angle >= _2PI)
		Angle = _2PI;
	
	return Angle-_PI;
}


//角度累计计算
float val=0.0f,last_val=0.0f,err=0.0f,circle = 0.0f;
/*float angle_get_all(float angle)
{
	val = angle;
	err = val - last_val;
	if( fabs(err) > (0.9f* _2PI) )
		circle += (err > 0) ? -1 : 1;
	last_val = val;
	return (float)circle * _2PI + last_val;
}*/

float angle_get_all(float angle)
{
    float i = angle - last_val;
    if (i < -_PI)
    {
        i += _2PI;
    }
    else if (i > _PI)
    {
        i -= _2PI;
    }
    val += i;
    last_val = angle;
		return val;
}

//速度计算
float Speed = 0;
float now=0.0,pre=0.0;
float get_speed(float angle)
{
	now = angle;
	Speed = (now - pre) / 1;
	pre = now;
	return Speed*100;
}

//低通滤波器
float y_prev;
float LowPass_Filter(float x)
{
	float y = 0.9*y_prev + 0.1*x;
	
	y_prev=y;
	
	return y;
}

