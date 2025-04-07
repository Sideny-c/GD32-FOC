#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "systick.h"
  /**
  * @������       delay
  * @��  ��       �ӳ�
  * @��  ��       �ӳ�num
  * @����ֵ       ��
  */
static void delay(unsigned int num)//�ӳٺ���
{
  while(num--);
}


/**
  * @������       IIC_Init
  * @��  ��       ����IIC
  * @��  ��       ��
  * @����ֵ       ��
  */
void IIC_GPIO_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);		//ʹ��GPIOA����ʱ��

  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_set(GPIOA, GPIO_PIN_11);
	gpio_bit_set(GPIOA, GPIO_PIN_12);
	
	OLED_SCL_Set();	//����SCL
	OLED_SDA_Set();	//����SDA
}


/**
  * @������       IIC_Start
  * @��  ��       IIC��ʼ�ź�
  * @��  ��       ��
  * @����ֵ       ��
  */
void IIC_Start(void)
{

  OLED_SCL_Set();	//ʱ�����ø�
  OLED_SDA_Set();	//�ź����ø�
  delay(1);	//�ӳ�1us
  OLED_SDA_Clr();	//�ź����õ�
  delay(1);	//�ӳ�1us
  OLED_SCL_Clr();	//ʱ�����õ�
  delay(1);	//�ӳ�1us
}


/**
  * @������       IIC_Stop
  * @��  ��       IICֹͣ�ź�
  * @��  ��       ��
  * @����ֵ       ��
  */
void IIC_Stop(void)
{
  OLED_SDA_Clr();	//�ź����õ�
  delay(1);	//�ӳ�1us
  OLED_SCL_Set();	//ʱ�����ø�
  delay(1);	//�ӳ�1us
  OLED_SDA_Set();	//�ź����ø�
  delay(1);	//�ӳ�1us
}


/**
  * @������       IIC_Wait_Ack
  * @��  ��       IIC���Ӧ��
  * @��  ��       ��
  * @����ֵ       ack��0-��Ӧ��1-��Ӧ��
  */
unsigned char IIC_Wait_Ack(void)
{
  unsigned char ack;

  OLED_SCL_Clr();	//ʱ�����õ�
  delay(1);	//�ӳ�1us
  OLED_SDA_Set();	//�ź����ø�
  delay(1);	//�ӳ�1us
  OLED_SCL_Set();	//ʱ�����ø�
  delay(1);

  if(OLED_READ_SDA())	//��ȡSDA�ĵ�ƽ
    ack = IIC_NO_ACK;	//���Ϊ1����ӻ�û��Ӧ��
   else
    ack = IIC_ACK;		//���Ϊ0����ӻ�Ӧ��

  OLED_SCL_Clr();//ʱ�����õ�
  delay(1);

  return ack;
}


/**
  * @������       IIC_Send_Ack
  * @��  ��       IIC����Ӧ��
  * @��  ��       ack��0-Ӧ��1-��Ӧ��
  * @����ֵ       ��
  */
void IIC_Send_Ack(unsigned char ack)
{
  OLED_SCL_Clr();	//ʱ�����õ�
  delay(1);	//�ӳ�1us

  if(ack == IIC_ACK)	//�����������Ӧ��/��Ӧ��
    OLED_SDA_Clr();	//Ӧ��
  else
    OLED_SDA_Set();	//��Ӧ��
  delay(1);	//�ӳ�1us

  OLED_SCL_Set();	//ʱ�����øߣ�����Ӧ��/��Ӧ���ź�
  delay(1);	//�ӳ�1us
  OLED_SCL_Clr();	//ʱ�����õ�
  delay(1);	//�ӳ�1us
}


/**
  * @������       Write_IIC_Byte
  * @��  ��       IICдһ�ֽ�����
  * @��  ��       IIC_Byte������
  * @����ֵ       ��
  */
void Write_IIC_Byte(unsigned char IIC_Byte)
{
  unsigned char i;  //�������
  for(i=0;i<8;i++) //forѭ��8��
  {
    OLED_SCL_Clr();	//ʱ�����õͣ�Ϊ����������׼��
    delay(1);	//�ӳ�1us

    if(IIC_Byte & 0x80){OLED_SDA_Set();}
    else	OLED_SDA_Clr();	

    IIC_Byte <<= 1;  //��������1λ
    delay(1);	//�ӳ�1us
    OLED_SCL_Set(); //ʱ�����øߣ����������أ������ݷ��ͳ�ȥ
    delay(1);	//�ӳ�1us
  }
  OLED_SCL_Clr();	//ʱ�����õ�
  delay(1);	//�ӳ�1us

  if(IIC_Wait_Ack()){};	//�ӻ�Ӧ��
	
}


/**
  * @������       Write_IIC_Command
  * @��  ��       OLEDд������
  * @��  ��       IIC_Command������
  * @����ֵ       ��
  */
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);//д��ӻ���ַ��SD0 = 0
   Write_IIC_Byte(0x00);//д������
   Write_IIC_Byte(IIC_Command);//����
   IIC_Stop();  //����ֹͣ�ź�
}


/**
  * @������       Write_IIC_Data
  * @��  ��       OLEDд������
  * @��  ��       IIC_Data������
  * @����ֵ       ��
  */
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);	//д��ӻ���ַ��SD0 = 0
   Write_IIC_Byte(0x40);	//д������
   Write_IIC_Byte(IIC_Data);//����
   IIC_Stop();		//����ֹͣ�ź�
}


/**
  * @������       OLED_WR_Byte
  * @��  ��       OLEDд������/����
  * @��  ��       dat�����ݣ�cmd��0-���1-����
  * @����ֵ       ��
  */
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
  if(cmd) 
    {
      Write_IIC_Data(dat); //д������
    }
  else {
      Write_IIC_Command(dat); //д������
    }
}

/**
  * @������       OLED_Set_Pos
  * @��  ��       ������X,Y����ʼ����
  * @��  ��       x��X���ꣻy��Y����
  * @����ֵ       ��
  */
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);	//д��ҳ��ַ
  OLED_WR_Byte((x&0x0f),OLED_CMD);	//д���е͵�ַ
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);	//д���иߵ�ַ
}   	  


/**
  * @������       OLED_Display_On
  * @��  ��       ��OLED��ʾ
  * @��  ��       ��
  * @����ֵ       ��
  */
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //����OLED��ɱ�
  OLED_WR_Byte(0X14,OLED_CMD);  //ʹ�ܣ���
  OLED_WR_Byte(0XAF,OLED_CMD);  //����ʾ
}

/**
  * @������       OLED_Display_Off
  * @��  ��       ��OLED��ʾ
  * @��  ��       ��
  * @����ֵ       ��
  */ 
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //����OLED��ɱ�
  OLED_WR_Byte(0X10,OLED_CMD);  //ʧ�ܣ���
  OLED_WR_Byte(0XAE,OLED_CMD);  //����ʾ
}		   			 


/**
  * @������       OLED_Clear
  * @��  ��       ����
  * @��  ��       ��
  * @����ֵ       ��
  */
void OLED_Clear(void)  
{  
  unsigned char i,n;		    //�������
  for(i=0;i<8;i++)  
    {  
      OLED_WR_Byte (0xb0+i,OLED_CMD);    //��0~7ҳ����д��
      OLED_WR_Byte (0x00,OLED_CMD);      //�е͵�ַ
      OLED_WR_Byte (0x10,OLED_CMD);      //�иߵ�ַ  
      for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); //д�� 0 ����
    }
}

/**
  * @������       OLED_ShowChar
  * @��  ��       ��ָ��λ����ʾ�ַ�
  * @��  ��       x��X���ꣻy��Y���ꣻchr����ʾ���ַ�
  * @����ֵ       ��
  */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr)
{      	
  unsigned char c=0,i=0;	
  c=chr-' '; //��ȡ�ַ���ƫ����	
  if(x>Max_Column-1){x=0;y=y+2;} //������鳬���˷�Χ���ʹ���2ҳ�ĵ�0�п�ʼ
  if(SIZE ==16) //�ַ���С���Ϊ 16 = 8*16
    {
      OLED_Set_Pos(x,y);	//��x y ��ʼ����
      for(i=0;i<8;i++)  //ѭ��8�� ռ8��
        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); //�ҳ��ַ� c ������λ�ã����ڵ�һҳ���л���
      OLED_Set_Pos(x,y+1); //ҳ����1
      for(i=0;i<8;i++)  //ѭ��8��
        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA); //�ѵڶ�ҳ����������
    }
  else  //�ַ���СΪ 6 = 6*8
    {	
      OLED_Set_Pos(x,y+1); //һҳ�Ϳ��Ի���
      for(i=0;i<6;i++) //ѭ��6�� ��ռ6��
      OLED_WR_Byte(F6x8[c][i],OLED_DATA); //���ַ�����
    }
}

/**
  * @������       oled_pow
  * @��  ��       ����m��n�η�
  * @��  ��       ��
  * @����ֵ       result��������
  */
unsigned int oled_pow(unsigned char m,unsigned char n)
{
  unsigned int result=1;	 
  while(n--)result*=m;    
  return result;
}

/**
  * @������       OLED_ShowNum
  * @��  ��       ��ָ����λ����ʾָ������&��С������
  * @��  ��       x��X���ꣻy��Y���ꣻnum����ʾ�����֣�len�����ֵĳ��ȣ�size2������Ĵ�С
  * @����ֵ       ��
  */		  
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2)
{         	
  unsigned char t,temp;  //�������
  unsigned char enshow=0;		//�������
  for(t=0;t<len;t++)
    {
      temp=(num/oled_pow(10,len-t-1))%10;//ȡ����������ÿ��λ���ɸߵ���
      if(enshow==0&&t<(len-1)) //enshow���Ƿ�Ϊ��һ������t<(len-1)���ж��Ƿ�Ϊ���һ����
        {
          if(temp==0) //�������Ϊ0 
            {
              OLED_ShowChar(x+(size2/2)*t,y,' ');//��ʾ 0 ��x+(size2/2)*t���������Сƫ�Ƶ�������8��
              continue; //����ʣ����䣬�����ظ�ѭ���������ظ���ʾ��
            }else enshow=1; 
        }
      OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); //��ʾһ��λ��x+(size2/2)*t���������Сƫ�Ƶ�������8��
    }
} 

/**
  * @������       OLED_ShowFNum
  * @��  ��       ��ָ��λ����ʾ�ַ���
  * @��  ��       x��X���ꣻy��Y���ꣻ*chr����ʾ���ַ���
  * @����ֵ       ��
  */
void OLED_ShowFNum(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	int z_temp,f_temp;      
	z_temp=(int)num;
	//��������
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
	//С����
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.'); 
	
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //С������
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0'); 
	}
}

/**
  * @������       OLED_ShowString
  * @��  ��       ��ָ��λ����ʾ�ַ���
  * @��  ��       x��X���ꣻy��Y���ꣻ*chr����ʾ���ַ���
  * @����ֵ       ��
  */
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr)
{
  unsigned char j=0; //�������
  while (chr[j]!='\0') //����������һ���ַ�
    {		
      OLED_ShowChar(x,y,chr[j]); //��ʾ�ַ�
      x+=8; //������8 ��һ���ַ�������ռ8
      if(x>120){x=0;y+=2;} //���x����128���л�ҳ���Ӹ�ҳ�ĵ�һ����ʾ
        j++; //��һ���ַ�
    }
}

/**
  * @������       OLED_ShowCHinese
  * @��  ��       ��ָ����λ����ʾ����
  * @��  ��       x��X���ꣻy��Y���ꣻno�����ֵ�����λ��
  * @����ֵ       ��
  */
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)
{      			    
  unsigned char t; //�������
  OLED_Set_Pos(x,y);	//�� x y ��ʼ���㣬�Ȼ���һҳ
  for(t=0;t<16;t++) //ѭ��16�Σ�����һҳ��16��
    {
      OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);//��no������λ�õĵ�һҳ16�еĵ�
    }	
  OLED_Set_Pos(x,y+1); //���ڶ���
  for(t=0;t<16;t++)	//ѭ��16�Σ����ڶ�ҳ��16��
    {	
      OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);//��no������λ�õĵڶ�ҳ16�еĵ�
    }					
}


/**
  * @������       OLED_DrawBMP
  * @��  ��       ��ָ���ķ�Χ��ʾͼƬ
  * @��  ��       x0����ʼX���ꣻy0����ʼX���ꣻx1����ʼX���ꣻy1����ʼX���ꣻBMP[]��ͼƬ��������ʼ��ַ
  * @����ֵ       ��
  */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0; //�������
 unsigned char x,y; //�������
  
  if(y1%8==0) y=y1/8;   //�ж���ֹҳ�Ƿ�Ϊ8��������
  else y=y1/8+1;
  
  for(y=y0;y<y1;y++) //����ʼҳ��ʼ��������ֹҳ
    {
      OLED_Set_Pos(x0,y); //��ҳ����ʼ�п�ʼ��
      for(x=x0;x<x1;x++) //��x1 - x0 ��
        {
          OLED_WR_Byte(BMP[j++],OLED_DATA);	//��ͼƬ�ĵ�    	
        }
    }
} 
//���Ժ���
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
		}
}


//��Ļ��ת180��
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}
/**
  * @������       OLED_Init
  * @��  ��       OLED��ʼ��
  * @��  ��       ��
  * @����ֵ       ��
  */				    
void OLED_Init(void)
{
	IIC_GPIO_Init();				//��ʼ��ģ��IIC��IO��
  delay(500);        //�ӳ٣����ڵ�Ƭ���ϵ��ʼ����OLED�죬���Ա�������ӳ٣��ȴ�OLED�ϵ��ʼ�����

  OLED_WR_Byte(0xAE,OLED_CMD);	//�ر���ʾ
  OLED_WR_Byte(0x2e,OLED_CMD);	//�رչ���

  OLED_WR_Byte(0x00,OLED_CMD);	//���õ��е�ַ
  OLED_WR_Byte(0x10,OLED_CMD);	//���ø��е�ַ
  OLED_WR_Byte(0x40,OLED_CMD);	//������ʼ�е�ַ
  OLED_WR_Byte(0xB0,OLED_CMD);	//����ҳ��ַ

  OLED_WR_Byte(0x81,OLED_CMD); 	// �Աȶ����ã�����������
  OLED_WR_Byte(0xFF,OLED_CMD);	//  265  

  OLED_WR_Byte(0xA1,OLED_CMD);	//���öΣ�SEG������ʼӳ���ַ��column��127��ַ��SEG0�ĵ�ַ
  OLED_WR_Byte(0xA6,OLED_CMD);	//������ʾ��0xa7����ʾ

  OLED_WR_Byte(0xA8,OLED_CMD);	//��������·��
  OLED_WR_Byte(0x1F,OLED_CMD);	//1/32 duty
          
  OLED_WR_Byte(0xC8,OLED_CMD);	//��ӳ��ģʽ��COM[N-1]~COM0ɨ��

  OLED_WR_Byte(0xD3,OLED_CMD);	//������ʾƫ��
  OLED_WR_Byte(0x00,OLED_CMD);	//��ƫ��
          
  OLED_WR_Byte(0xD5,OLED_CMD);	//����������Ƶ��Ĭ�ϣ����370KHz
  OLED_WR_Byte(0x80,OLED_CMD);	
          
  OLED_WR_Byte(0xD9,OLED_CMD);	//���� Pre-Charge Period��Ĭ�ϣ�
  OLED_WR_Byte(0x1F,OLED_CMD);
          
  OLED_WR_Byte(0xDA,OLED_CMD);	//���� com pin configuartion��Ĭ�ϣ�
  OLED_WR_Byte(0x00,OLED_CMD);
          
  OLED_WR_Byte(0xDB,OLED_CMD);	//���� Vcomh���ɵ������ȣ�Ĭ�ϣ�
  OLED_WR_Byte(0x30,OLED_CMD);
          
  OLED_WR_Byte(0x8D,OLED_CMD);	//����OLED��ɱ�
  OLED_WR_Byte(0x14,OLED_CMD);	//����ʾ

  OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 

  OLED_WR_Byte(0xAF,OLED_CMD);//����OLED�����ʾ
  OLED_Clear();        //����
  OLED_Set_Pos(0,0); 	 //����
}  

/**
  * @������       OLED_Scroll
  * @��  ��       ����Ч�����ú���
  * @��  ��       ��
  * @����ֵ       ��
  */	
void OLED_Scroll(void)
{
  OLED_WR_Byte(0x2E,OLED_CMD);	//�رչ���
  OLED_WR_Byte(0x27,OLED_CMD);	//ˮƽ�������
  OLED_WR_Byte(0x00,OLED_CMD);	//�����ֽ�
  OLED_WR_Byte(0x00,OLED_CMD);	//��ʼҳ 0
  OLED_WR_Byte(0x00,OLED_CMD);	//����ʱ����
  OLED_WR_Byte(0x01,OLED_CMD);	//��ֹҳ 1
  OLED_WR_Byte(0x00,OLED_CMD);	//�����ֽ�
  OLED_WR_Byte(0xFF,OLED_CMD);	//�����ֽ�
  OLED_WR_Byte(0x2F,OLED_CMD);	//��������
}


