//This procedure is open source procedure.
//Wuhan Zhengyuan Geotechnical Technology Co., LTD
//
//
//  文 件 名   : main.c
//  版 本 号   : v1.0
//  作    者   : ZhangFeng
//  生成日期   : 2022-01-26
//  最近修改   : 
//  功能描述   : Stm32f103c8t6+sht30+OLED(SPI接口)
//              OLED接线：
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   PA0（SCL）
//              D1   PA1（SDA）
//              RES  PA2
//              DC   PA3
//              CS   PA4 
//              ----------------------------------------------------------------
//              sht30接线：
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   3.3v电源
//              SCL   PB10（SCL）
//              SDA   PB11（SDA）
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 

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
	  delay_init(72);	          //延时初始化
	  OLED_Init();
	  OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
		NVIC_Configuration();
		uart_init(9600);         	//初始化串口波特率为9600
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

