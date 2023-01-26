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
  IIC_Start();                  	//��ʼ�ź�
  IIC_Send_Byte(0x88+0);   //�����豸��ַ+д�ź�
	while(IIC_Wait_Ack());						//�ӻ�Ӧ��
  IIC_Send_Byte(msb);    					//��������
	while(IIC_Wait_Ack());
	IIC_Send_Byte(lsb);    					//��������
	while(IIC_Wait_Ack());
  IIC_Stop();                   //����ֹͣ�ź�
	delay_ms(5);
}


u8 crc1=0;
u8 crc2=0;
extern float tem ;
extern float hum ;
void Read_sht30(void)
{   	
		Cmd_Write_sht30(0x21,0x26); //���ݲɼ�Ƶ��1 m/s
		//IIC_SCL=1;
		IIC_Start();   	//��ʼ�ź�
    IIC_Send_Byte(0x88+1);         //�����豸��ַ+���ź�
		while(IIC_Wait_Ack());
		BUFt[0]=IIC_Read_Byte(1);  //��ȡ�¶ȸ߰�λ���ҷ���һ��ack
		BUFt[1]=IIC_Read_Byte(1);  //�������Ͱ�λ������
	  BUFt[2]=IIC_Read_Byte(1);  //У��λ����ACK8wei
    BUFt[3]=IIC_Read_Byte(1);  //ʪ�ȷ���ACK�߰�λ
	  BUFt[4]=IIC_Read_Byte(1);  //ʪ�ȵڰ�λ
	  BUFt[5]=IIC_Read_Byte(0);  //У��λ֮����nack����������ֲ�
    IIC_Stop();                //����ֹͣ�ź�
    delay_ms(5);
}

//����ת��
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
