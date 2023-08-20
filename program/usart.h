#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 

void UART1_Init(u32 pclk2,u32 bound);
void uart1_sendbyte(unsigned char dat);
void uart1_send_string(unsigned char *buf,unsigned short length);

#endif


