#include "Struct.h"
#include "sys.h"
#include "usart.h"

//����1��ʼ��  ����ͨ��
void UART1_Init(u32 pclk2, u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO�˿�����
	NVIC_InitTypeDef NVIC_InitStructure; //�ж�����

	temp = (float)(pclk2*1000000ull)/(bound*16);//�õ�USARTDIV
	mantissa = temp;				 //�õ���������
	fraction = (temp-mantissa) * 16; //�õ�С������
    mantissa <<= 4;
	mantissa += fraction;

	RCC->APB2ENR |= 1<<3;   //ʹ��PORTB��ʱ��
	RCC->APB2ENR |= 1<<14;  //ʹ�ܴ���1ʱ��

	RCC->APB2RSTR |= 1<<14;   //��λ����1
	RCC->APB2RSTR &= ~(1<<14);//ֹͣ��λ

	//�˿���ӳ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//����������
 	USART1->BRR  = mantissa;    //����������
	USART1->CR1 |= 1<<3 | 1<<2; //ʹ�ܷ��� ʹ�ܽ���
	USART1->CR1 |= 1<<5;        //���ջ������ǿ��ж�ʹ��
	USART1->CR1 |= 1<<13;  	    //����ʹ��

	//�����ж�
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

//USART1 ���͵��ֽ�
void uart1_sendbyte(unsigned char dat)
{
	while((USART1->SR&0x40)==0);
	USART1->DR=(u8) dat;
}

//USART1 �����ַ���
void uart1_send_string(unsigned char *buf,unsigned short length)
{
	u16 i;
	for(i=0; i<length; i++)//ѭ����������
	{
		uart1_sendbyte(*buf++);
	}
}

//USART1 �����ж�
void USART1_IRQHandler(void) //����1�жϷ������
{
	unsigned char Res;
	if(USART1->SR&(1<<5)) {
		Res = USART1->DR;    //�����Ĵ�������
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

