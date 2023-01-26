#include "myiic.h"
#include "delay.h"

 
  //MPU IIC 延时函数
void IIC_Delay(void)
{
	delay_us(2);
}

//初始化IIC
void IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//先使能外设IO PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);  
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
  GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);						 //PB10,PB11 输出高	
	
		
	 
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 50000; 
	/* I2C2 初始化 */
	I2C_Init(I2C2, &I2C_InitStructure);	   
	
	/* 使能 I2C2 */
	I2C_Cmd  (I2C1,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE); 
 
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	IIC_Delay();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	IIC_Delay();							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;IIC_Delay();	   
	IIC_SCL=1;IIC_Delay();	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    IIC_SCL=1;
		    IIC_Delay(); 
		    IIC_SCL=0;	
		    IIC_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        IIC_Delay();
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		IIC_Delay(); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

























