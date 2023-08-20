#include "Struct.h"
#include "sys.h"
#include "usart.h"

//串口1初始化  数据通信
void UART1_Init(u32 pclk2, u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO端口设置
	NVIC_InitTypeDef NVIC_InitStructure; //中断配置

	temp = (float)(pclk2*1000000ull)/(bound*16);//得到USARTDIV
	mantissa = temp;				 //得到整数部分
	fraction = (temp-mantissa) * 16; //得到小数部分
    mantissa <<= 4;
	mantissa += fraction;

	RCC->APB2ENR |= 1<<3;   //使能PORTB口时钟
	RCC->APB2ENR |= 1<<14;  //使能串口1时钟

	RCC->APB2RSTR |= 1<<14;   //复位串口1
	RCC->APB2RSTR &= ~(1<<14);//停止复位

	//端口重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//波特率设置
 	USART1->BRR  = mantissa;    //波特率设置
	USART1->CR1 |= 1<<3 | 1<<2; //使能发送 使能接收
	USART1->CR1 |= 1<<5;        //接收缓冲区非空中断使能
	USART1->CR1 |= 1<<13;  	    //串口使能

	//空闲中断
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

//USART1 发送单字节
void uart1_sendbyte(unsigned char dat)
{
	while((USART1->SR&0x40)==0);
	USART1->DR=(u8) dat;
}

//USART1 发送字符串
void uart1_send_string(unsigned char *buf,unsigned short length)
{
	u16 i;
	for(i=0; i<length; i++)//循环发送数据
	{
		uart1_sendbyte(*buf++);
	}
}

//USART1 接收中断
void USART1_IRQHandler(void) //串口1中断服务程序
{
	unsigned char Res;
	if(USART1->SR&(1<<5)) {
		Res = USART1->DR;    //读出寄存器数据
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

