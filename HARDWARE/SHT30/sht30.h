#ifndef __SHT30_H
#define __SHT30_H
#include "myiic.h"   	


#define SHT30_ADDR    0x44  //ADDR (pin 2)connected to VDD

void Read_sht30(void);
void Cmd_Write_sht30(uint8_t msb,uint8_t lsb);
void Convert_sht30(void);

#endif
