#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "systick.h"
  /**
  * @函数名       delay
  * @功  能       延迟
  * @参  数       延迟num
  * @返回值       无
  */
static void delay(unsigned int num)//延迟函数
{
  while(num--);
}


/**
  * @函数名       IIC_Init
  * @功  能       配置IIC
  * @参  数       无
  * @返回值       无
  */
void IIC_GPIO_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);		//使能GPIOA外设时钟

  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_set(GPIOA, GPIO_PIN_11);
	gpio_bit_set(GPIOA, GPIO_PIN_12);
	
	OLED_SCL_Set();	//拉高SCL
	OLED_SDA_Set();	//拉高SDA
}


/**
  * @函数名       IIC_Start
  * @功  能       IIC起始信号
  * @参  数       无
  * @返回值       无
  */
void IIC_Start(void)
{

  OLED_SCL_Set();	//时钟线置高
  OLED_SDA_Set();	//信号线置高
  delay(1);	//延迟1us
  OLED_SDA_Clr();	//信号线置低
  delay(1);	//延迟1us
  OLED_SCL_Clr();	//时钟线置低
  delay(1);	//延迟1us
}


/**
  * @函数名       IIC_Stop
  * @功  能       IIC停止信号
  * @参  数       无
  * @返回值       无
  */
void IIC_Stop(void)
{
  OLED_SDA_Clr();	//信号线置低
  delay(1);	//延迟1us
  OLED_SCL_Set();	//时钟线置高
  delay(1);	//延迟1us
  OLED_SDA_Set();	//信号线置高
  delay(1);	//延迟1us
}


/**
  * @函数名       IIC_Wait_Ack
  * @功  能       IIC检测应答
  * @参  数       无
  * @返回值       ack：0-有应答、1-无应答
  */
unsigned char IIC_Wait_Ack(void)
{
  unsigned char ack;

  OLED_SCL_Clr();	//时钟线置低
  delay(1);	//延迟1us
  OLED_SDA_Set();	//信号线置高
  delay(1);	//延迟1us
  OLED_SCL_Set();	//时钟线置高
  delay(1);

  if(OLED_READ_SDA())	//读取SDA的电平
    ack = IIC_NO_ACK;	//如果为1，则从机没有应答
   else
    ack = IIC_ACK;		//如果为0，则从机应答

  OLED_SCL_Clr();//时钟线置低
  delay(1);

  return ack;
}


/**
  * @函数名       IIC_Send_Ack
  * @功  能       IIC发送应答
  * @参  数       ack：0-应答、1-不应答
  * @返回值       无
  */
void IIC_Send_Ack(unsigned char ack)
{
  OLED_SCL_Clr();	//时钟线置低
  delay(1);	//延迟1us

  if(ack == IIC_ACK)	//根据情况发送应答/不应答
    OLED_SDA_Clr();	//应答
  else
    OLED_SDA_Set();	//不应答
  delay(1);	//延迟1us

  OLED_SCL_Set();	//时钟线置高，发送应答/不应答信号
  delay(1);	//延迟1us
  OLED_SCL_Clr();	//时钟线置低
  delay(1);	//延迟1us
}


/**
  * @函数名       Write_IIC_Byte
  * @功  能       IIC写一字节数据
  * @参  数       IIC_Byte：数据
  * @返回值       无
  */
void Write_IIC_Byte(unsigned char IIC_Byte)
{
  unsigned char i;  //定义变量
  for(i=0;i<8;i++) //for循环8次
  {
    OLED_SCL_Clr();	//时钟线置低，为传输数据做准备
    delay(1);	//延迟1us

    if(IIC_Byte & 0x80){OLED_SDA_Set();}
    else	OLED_SDA_Clr();	

    IIC_Byte <<= 1;  //数据左移1位
    delay(1);	//延迟1us
    OLED_SCL_Set(); //时钟线置高，产生上升沿，把数据发送出去
    delay(1);	//延迟1us
  }
  OLED_SCL_Clr();	//时钟线置低
  delay(1);	//延迟1us

  if(IIC_Wait_Ack()){};	//从机应答
	
}


/**
  * @函数名       Write_IIC_Command
  * @功  能       OLED写入命令
  * @参  数       IIC_Command：命令
  * @返回值       无
  */
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);//写入从机地址，SD0 = 0
   Write_IIC_Byte(0x00);//写入命令
   Write_IIC_Byte(IIC_Command);//数据
   IIC_Stop();  //发送停止信号
}


/**
  * @函数名       Write_IIC_Data
  * @功  能       OLED写入数据
  * @参  数       IIC_Data：数据
  * @返回值       无
  */
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);	//写入从机地址，SD0 = 0
   Write_IIC_Byte(0x40);	//写入数据
   Write_IIC_Byte(IIC_Data);//数据
   IIC_Stop();		//发送停止信号
}


/**
  * @函数名       OLED_WR_Byte
  * @功  能       OLED写入数据/命令
  * @参  数       dat：数据；cmd：0-命令、1-数据
  * @返回值       无
  */
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
  if(cmd) 
    {
      Write_IIC_Data(dat); //写入数据
    }
  else {
      Write_IIC_Command(dat); //写入命令
    }
}

/**
  * @函数名       OLED_Set_Pos
  * @功  能       在坐标X,Y初开始花店
  * @参  数       x：X坐标；y：Y坐标
  * @返回值       无
  */
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);	//写入页地址
  OLED_WR_Byte((x&0x0f),OLED_CMD);	//写入列低地址
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);	//写入列高地址
}   	  


/**
  * @函数名       OLED_Display_On
  * @功  能       开OLED显示
  * @参  数       无
  * @返回值       无
  */
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //设置OLED电荷泵
  OLED_WR_Byte(0X14,OLED_CMD);  //使能，开
  OLED_WR_Byte(0XAF,OLED_CMD);  //开显示
}

/**
  * @函数名       OLED_Display_Off
  * @功  能       关OLED显示
  * @参  数       无
  * @返回值       无
  */ 
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //设置OLED电荷泵
  OLED_WR_Byte(0X10,OLED_CMD);  //失能，关
  OLED_WR_Byte(0XAE,OLED_CMD);  //关显示
}		   			 


/**
  * @函数名       OLED_Clear
  * @功  能       清屏
  * @参  数       无
  * @返回值       无
  */
void OLED_Clear(void)  
{  
  unsigned char i,n;		    //定义变量
  for(i=0;i<8;i++)  
    {  
      OLED_WR_Byte (0xb0+i,OLED_CMD);    //从0~7页依次写入
      OLED_WR_Byte (0x00,OLED_CMD);      //列低地址
      OLED_WR_Byte (0x10,OLED_CMD);      //列高地址  
      for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); //写入 0 清屏
    }
}

/**
  * @函数名       OLED_ShowChar
  * @功  能       在指定位置显示字符
  * @参  数       x：X坐标；y：Y坐标；chr：显示的字符
  * @返回值       无
  */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr)
{      	
  unsigned char c=0,i=0;	
  c=chr-' '; //获取字符的偏移量	
  if(x>Max_Column-1){x=0;y=y+2;} //如果列书超出了范围，就从下2页的第0列开始
  if(SIZE ==16) //字符大小如果为 16 = 8*16
    {
      OLED_Set_Pos(x,y);	//从x y 开始画点
      for(i=0;i<8;i++)  //循环8次 占8列
        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); //找出字符 c 的数组位置，先在第一页把列画完
      OLED_Set_Pos(x,y+1); //页数加1
      for(i=0;i<8;i++)  //循环8次
        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA); //把第二页的列数画完
    }
  else  //字符大小为 6 = 6*8
    {	
      OLED_Set_Pos(x,y+1); //一页就可以画完
      for(i=0;i<6;i++) //循环6次 ，占6列
      OLED_WR_Byte(F6x8[c][i],OLED_DATA); //把字符画完
    }
}

/**
  * @函数名       oled_pow
  * @功  能       计算m的n次方
  * @参  数       无
  * @返回值       result：计算结果
  */
unsigned int oled_pow(unsigned char m,unsigned char n)
{
  unsigned int result=1;	 
  while(n--)result*=m;    
  return result;
}

/**
  * @函数名       OLED_ShowNum
  * @功  能       在指定的位置显示指定长度&大小的数字
  * @参  数       x：X坐标；y：Y坐标；num：显示的数字；len：数字的长度；size2：字体的大小
  * @返回值       无
  */		  
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2)
{         	
  unsigned char t,temp;  //定义变量
  unsigned char enshow=0;		//定义变量
  for(t=0;t<len;t++)
    {
      temp=(num/oled_pow(10,len-t-1))%10;//取出输入数的每个位，由高到低
      if(enshow==0&&t<(len-1)) //enshow：是否为第一个数；t<(len-1)：判断是否为最后一个数
        {
          if(temp==0) //如果该数为0 
            {
              OLED_ShowChar(x+(size2/2)*t,y,' ');//显示 0 ；x+(size2/2)*t根据字体大小偏移的列数（8）
              continue; //跳过剩下语句，继续重复循环（避免重复显示）
            }else enshow=1; 
        }
      OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); //显示一个位；x+(size2/2)*t根据字体大小偏移的列数（8）
    }
} 

/**
  * @函数名       OLED_ShowFNum
  * @功  能       在指定位置显示字符串
  * @参  数       x：X坐标；y：Y坐标；*chr：显示的字符串
  * @返回值       无
  */
void OLED_ShowFNum(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	int z_temp,f_temp;      
	z_temp=(int)num;
	//整数部分
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}
			else
			  enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.'); 
	
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0'); 
	}
}

/**
  * @函数名       OLED_ShowString
  * @功  能       在指定位置显示字符串
  * @参  数       x：X坐标；y：Y坐标；*chr：显示的字符串
  * @返回值       无
  */
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr)
{
  unsigned char j=0; //定义变量
  while (chr[j]!='\0') //如果不是最后一个字符
    {		
      OLED_ShowChar(x,y,chr[j]); //显示字符
      x+=8; //列数加8 ，一个字符的列数占8
      if(x>120){x=0;y+=2;} //如果x超过128，切换页，从该页的第一列显示
        j++; //下一个字符
    }
}

/**
  * @函数名       OLED_ShowCHinese
  * @功  能       在指定的位置显示汉字
  * @参  数       x：X坐标；y：Y坐标；no：汉字的数组位置
  * @返回值       无
  */
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)
{      			    
  unsigned char t; //定义变量
  OLED_Set_Pos(x,y);	//从 x y 开始画点，先画第一页
  for(t=0;t<16;t++) //循环16次，画第一页的16列
    {
      OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);//画no在数组位置的第一页16列的点
    }	
  OLED_Set_Pos(x,y+1); //画第二列
  for(t=0;t<16;t++)	//循环16次，画第二页的16列
    {	
      OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);//画no在数组位置的第二页16列的点
    }					
}


/**
  * @函数名       OLED_DrawBMP
  * @功  能       在指定的范围显示图片
  * @参  数       x0：起始X坐标；y0：起始X坐标；x1：起始X坐标；y1：起始X坐标；BMP[]：图片的数组起始地址
  * @返回值       无
  */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0; //定义变量
 unsigned char x,y; //定义变量
  
  if(y1%8==0) y=y1/8;   //判断终止页是否为8的整数倍
  else y=y1/8+1;
  
  for(y=y0;y<y1;y++) //从起始页开始，画到终止页
    {
      OLED_Set_Pos(x0,y); //在页的起始列开始画
      for(x=x0;x<x1;x++) //画x1 - x0 列
        {
          OLED_WR_Byte(BMP[j++],OLED_DATA);	//画图片的点    	
        }
    }
} 
//反显函数
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}


//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}
/**
  * @函数名       OLED_Init
  * @功  能       OLED初始化
  * @参  数       无
  * @返回值       无
  */				    
void OLED_Init(void)
{
	IIC_GPIO_Init();				//初始化模拟IIC的IO口
  delay(500);        //延迟，由于单片机上电初始化比OLED快，所以必须加上延迟，等待OLED上电初始化完成

  OLED_WR_Byte(0xAE,OLED_CMD);	//关闭显示
  OLED_WR_Byte(0x2e,OLED_CMD);	//关闭滚动

  OLED_WR_Byte(0x00,OLED_CMD);	//设置低列地址
  OLED_WR_Byte(0x10,OLED_CMD);	//设置高列地址
  OLED_WR_Byte(0x40,OLED_CMD);	//设置起始行地址
  OLED_WR_Byte(0xB0,OLED_CMD);	//设置页地址

  OLED_WR_Byte(0x81,OLED_CMD); 	// 对比度设置，可设置亮度
  OLED_WR_Byte(0xFF,OLED_CMD);	//  265  

  OLED_WR_Byte(0xA1,OLED_CMD);	//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址
  OLED_WR_Byte(0xA6,OLED_CMD);	//正常显示；0xa7逆显示

  OLED_WR_Byte(0xA8,OLED_CMD);	//设置驱动路数
  OLED_WR_Byte(0x1F,OLED_CMD);	//1/32 duty
          
  OLED_WR_Byte(0xC8,OLED_CMD);	//重映射模式，COM[N-1]~COM0扫描

  OLED_WR_Byte(0xD3,OLED_CMD);	//设置显示偏移
  OLED_WR_Byte(0x00,OLED_CMD);	//无偏移
          
  OLED_WR_Byte(0xD5,OLED_CMD);	//设置震荡器分频（默认）大概370KHz
  OLED_WR_Byte(0x80,OLED_CMD);	
          
  OLED_WR_Byte(0xD9,OLED_CMD);	//设置 Pre-Charge Period（默认）
  OLED_WR_Byte(0x1F,OLED_CMD);
          
  OLED_WR_Byte(0xDA,OLED_CMD);	//设置 com pin configuartion（默认）
  OLED_WR_Byte(0x00,OLED_CMD);
          
  OLED_WR_Byte(0xDB,OLED_CMD);	//设置 Vcomh，可调节亮度（默认）
  OLED_WR_Byte(0x30,OLED_CMD);
          
  OLED_WR_Byte(0x8D,OLED_CMD);	//设置OLED电荷泵
  OLED_WR_Byte(0x14,OLED_CMD);	//开显示

  OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 

  OLED_WR_Byte(0xAF,OLED_CMD);//开启OLED面板显示
  OLED_Clear();        //清屏
  OLED_Set_Pos(0,0); 	 //画点
}  

/**
  * @函数名       OLED_Scroll
  * @功  能       滚动效果配置函数
  * @参  数       无
  * @返回值       无
  */	
void OLED_Scroll(void)
{
  OLED_WR_Byte(0x2E,OLED_CMD);	//关闭滚动
  OLED_WR_Byte(0x27,OLED_CMD);	//水平向左滚动
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x00,OLED_CMD);	//起始页 0
  OLED_WR_Byte(0x00,OLED_CMD);	//滚动时间间隔
  OLED_WR_Byte(0x01,OLED_CMD);	//终止页 1
  OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0xFF,OLED_CMD);	//虚拟字节
  OLED_WR_Byte(0x2F,OLED_CMD);	//开启滚动
}


