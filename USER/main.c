//This procedure is open source procedure.
//Wuhan Zhengyuan Geotechnical Technology Co., LTD
//
//
//  �� �� ��   : main.c
//  �� �� ��   : v1.0
//  ��    ��   : ZhangFeng
//  ��������   : 2022-01-26
//  ����޸�   : 
//  ��������   : Stm32f103c8t6+sht30+OLED(SPI�ӿ�)
//              OLED���ߣ�
//              ----------------------------------------------------------------
//              GND  ��Դ��
//              VCC  3.3v��Դ
//              D0   PA0��SCL��
//              D1   PA1��SDA��
//              RES  PA2
//              DC   PA3
//              CS   PA4 
//              ----------------------------------------------------------------
//              sht30���ߣ�
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   3.3v��Դ
//              SCL   PB10��SCL��
//              SDA   PB11��SDA��
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 

//******************************************************************************/


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "myiic.h"
#include "sht30.h"
#include "oled.h"
#include "bmp.h"

float tem ;
float hum ;
unsigned char dat[5] = {0};

 int main(void)
 {	 		
	  u8 t=' ';
	  delay_init(72);	          //��ʱ��ʼ��
	  OLED_Init();
	  OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
		NVIC_Configuration();
		uart_init(9600);         	//��ʼ�����ڲ�����Ϊ9600
		IIC_Init();
		 while(1)
		 {          
				
			Read_sht30();
      Convert_sht30();
		//	printf("temp=%.1f;hump=%.1f\r\n",tem,hum);
			 printf("temp=%.1f\r\n",tem);
			 printf("humd=%.1f\r\n",hum);		
//			 printf("%.1f,%.1f,%d,%d\r\n",tem,hum,CO2,pm25);
			 
			LED0=!LED0;	 
			delay_ms(1000);	

			//CO2 =0;
			//pm25 =0;
			 
	  //OLED_ShowPicture(0,0,128,64,BMP1,1);
		//OLED_Refresh();
		//delay_ms(500);	 
		OLED_Clear();
		OLED_ShowString(1,1,"temp:",16,1);
		OLED_Show1fNum(40,1,tem,5,16,1);
		OLED_ShowString(100,1,"C",16,1);
		OLED_ShowString(1,40,"humi:",16,1);
		OLED_Show1fNum(40,40,hum,5,16,1);
		OLED_ShowString(100,40,"%",16,1);
		OLED_Refresh();
		

      delay_ms(1000);		
			tem = 0;
			hum =0;		 
		 }  	 	
 }

