#include "sht30.h"
#include "sys.h"
#include "delay.h" 
#include "myiic.h"


u8 BUFt[6]={0};


/*unsigned char crc8(char *buf)
{
 unsigned char i,j,crc; 
 
  crc=0;
 for(j=1;j<7;j++)
 {
        crc=crc^(*buf);
        for(i = 8; i > 0; i--)
        {
            if(crc & 0x80)
            {
             crc = (crc<< 1)^0x31;
            }
    else
            {
             crc = crc<< 1;
    }
        }
    buf++;
 }
 return crc;
}*/

u8 Get_Crc8(u8 *ptr,u16 len)
{
	u8 crc;
	u8 i;
	crc=0;
	while(len--)
	{
		crc^=*ptr++;
		for(i=0;i<8;i++)
		{
			if(crc&0x01)crc=(crc>>1)^0x8C;
			else crc >>= 1;
		}
	}
	return crc;
}
void Cmd_Write_sht30(uint8_t msb,uint8_t lsb)
{
  IIC_Start();                  	//起始信号
  IIC_Send_Byte(0x88+0);   //发送设备地址+写信号
	while(IIC_Wait_Ack());						//从机应答
  IIC_Send_Byte(msb);    					//发送命令
	while(IIC_Wait_Ack());
	IIC_Send_Byte(lsb);    					//发送命令
	while(IIC_Wait_Ack());
  IIC_Stop();                   //发送停止信号
	delay_ms(5);
}


u8 crc1=0;
u8 crc2=0;
extern float tem ;
extern float hum ;
void Read_sht30(void)
{   	
		Cmd_Write_sht30(0x21,0x26); //数据采集频率1 m/s
		//IIC_SCL=1;
		IIC_Start();   	//起始信号
    IIC_Send_Byte(0x88+1);         //发送设备地址+读信号
		while(IIC_Wait_Ack());
		BUFt[0]=IIC_Read_Byte(1);  //读取温度高八位并且发送一个ack
		BUFt[1]=IIC_Read_Byte(1);  //。。。低八位。。。
	  BUFt[2]=IIC_Read_Byte(1);  //校验位发送ACK8wei
    BUFt[3]=IIC_Read_Byte(1);  //湿度发送ACK高把位
	  BUFt[4]=IIC_Read_Byte(1);  //湿度第八位
	  BUFt[5]=IIC_Read_Byte(0);  //校验位之后发送nack具体见数据手册
    IIC_Stop();                //发送停止信号
    delay_ms(5);
}

//数据转换
void Convert_sht30(void)
{

		tem=175.0*((BUFt[0]<<8)+BUFt[1])/65535.0-45.0;
		hum=100.0*((BUFt[3]<<8)+BUFt[4])/65535.0;
	
		BUFt[0]=0;
		BUFt[1]=0;
		BUFt[2]=0;
		BUFt[3]=0;
		BUFt[4]=0;
		BUFt[5]=0;
}
