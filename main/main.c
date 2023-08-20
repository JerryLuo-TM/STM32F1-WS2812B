#include "Struct.h"

Status_Params Status;


#define SPI_BYTE_LEN_PER_PIXEL 3
#define PIXEL_NUM 16
volatile bool dma_tx_flag = false;

const unsigned int RGB_Buf[8][PIXEL_NUM] = {
	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,	0x00ff0000,
	0x00ff0000, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00,	0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00, 0x0000ff00,
	0x0000ff00, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff,	0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff,

	0x000000ff, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080,	0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080, 0x00808080,
	0x00808080, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000,	0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000, 0x00400000,
	0x00400000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000,	0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000, 0x00004000,
	0x00004000, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,	0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
	0x00000040, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF,	0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF,
};

uint32_t g_rgb_buffer[PIXEL_NUM];

unsigned char SPI_TxBuf[PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL];


void Breath_mode(void)
{
	static bool turn;
	static unsigned int i;
	static unsigned char r_val, g_val, b_val, color;
	if (turn == true) {
		color += 1;
		if (color == 0xF0) {
			turn = !turn;
		}
	} else {
		color -= 1;
		if (color == 0x10) {
			turn = !turn;
		}
	}

	for (i=0; i<PIXEL_NUM; i++) {
		r_val = color;
		g_val = color;
		b_val = color;
		g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
	}

	if (dma_tx_flag == false) {
		dma_tx_flag = true;
		WS2812_Pixels_Packet(PIXEL_NUM, (uint32_t *)g_rgb_buffer, SPI_TxBuf);
		SPI_DMA_Enable(SPI_TX_DMA_CHANNEL,PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);
	}
}

#define MAX_LAP_LEN		12
void Waterlamp(void)
{
	static uint8_t index = 0;
	uint8_t i, j;
	uint8_t r_val, g_val, b_val, color;

	memset((uint8_t*)&g_rgb_buffer[0], 0, PIXEL_NUM * 4);

	index += 1; 
	if(index > (PIXEL_NUM + MAX_LAP_LEN)) {
		index = 0;
	}
	if (index < MAX_LAP_LEN) {
		for (j=1,i=0; i<index; i++,j++) {
			color = j * (0xFF / MAX_LAP_LEN);
			r_val = color;
			g_val = color;
			b_val = 0x0;
			g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
		}
	} else if (index < PIXEL_NUM) {
		for (j=1,i=(index-MAX_LAP_LEN); i<index; i++,j++) {
			color = j * (0xFF / MAX_LAP_LEN);
			r_val = color;
			g_val = color;
			b_val = 0x0;
			g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
		}
	} else {
		for (j=1,i=(index-MAX_LAP_LEN); i<PIXEL_NUM; i++,j++) {
			color =  j * (0xFF / MAX_LAP_LEN);
			r_val = color;
			g_val = color;
			b_val = 0x0;
			g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
		}
	}

	if (dma_tx_flag == false) {
		dma_tx_flag = true;
		WS2812_Pixels_Packet(PIXEL_NUM, (uint32_t *)g_rgb_buffer, SPI_TxBuf);
		SPI_DMA_Enable(SPI_TX_DMA_CHANNEL,PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);
	}
}

#define MAX_LAP_LEN_1		16
void Waterlamp_1(void)
{
	static uint8_t index = 0;
	uint8_t i;
	uint8_t r_val, g_val, b_val, color;

	g_rgb_buffer[0] = 0x101000; 
	memset((uint8_t*)&g_rgb_buffer[1], 0, PIXEL_NUM * 4);

	index += 1; 
	if(index > MAX_LAP_LEN_1) {
		vTaskDelay(200);
		index = 0;
	}

	for (i=1; i<index; i++) {
		color =  i * (0xFF / MAX_LAP_LEN_1);
		r_val = color;
		g_val = color;
		b_val = 0x0;
		g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
	}

	if (dma_tx_flag == false) {
		dma_tx_flag = true;
		WS2812_Pixels_Packet(PIXEL_NUM, (uint32_t *)g_rgb_buffer, SPI_TxBuf);
		SPI_DMA_Enable(SPI_TX_DMA_CHANNEL,PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);
	}
}

void UAV_LED(void)
{
	static uint8_t index = 0;
	uint8_t i;
	uint8_t r_val, g_val, b_val, color;

	memset((uint8_t*)&g_rgb_buffer[1], 0, PIXEL_NUM * 4);

	index += 1;
	if(index > 12) {
		index = 0;
	}
	if (index<6) {
		for (i=0; i<8; i++) {
			if (i<4) {
				r_val = 0xFF;
				g_val = 0x00;
				b_val = 0x0;
			} else {
				r_val = 0xFF;
				g_val = 0xFF;
				b_val = 0x0;
			}
			g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
		}
	} else {
		for (i=0; i<8; i++) {
			if (i<4) {
				r_val = 0xFF;
				g_val = 0x0;
				b_val = 0x0;
			} else {
				r_val = 0x0;
				g_val = 0x0;
				b_val = 0x0;
			}
			g_rgb_buffer[i] = (r_val<<16 | g_val<<8 | b_val);
		}
	}


	if (dma_tx_flag == false) {
		dma_tx_flag = true;
		WS2812_Pixels_Packet(PIXEL_NUM, (uint32_t *)g_rgb_buffer, SPI_TxBuf);
		SPI_DMA_Enable(SPI_TX_DMA_CHANNEL,PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);
	}
}

void Demo_task(void *pvParameters)
{
	unsigned int count = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (1)
	{
		if (++count%2 == 0) {
			LED_1 = 1;
		} else {
			LED_1 = 0;
		}

		//Breath_mode();
		Waterlamp();
		// Waterlamp_1();
		// UAV_LED();

		// dma_tx_flag = true;
		// WS2812_Pixels_Packet(PIXEL_NUM, (uint32_t *)RGB_Buf[cnt], SPI_TxBuf);
		// SPI_DMA_Enable(SPI_TX_DMA_CHANNEL,PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);
		// if(++cnt >= 8)
		// 	cnt = 0;

		vTaskDelayUntil(&xLastWakeTime, configTICK_RATE_HZ/30);
	}
}


//用于创建任务的任务
void create_app_task(void)
{
	//创建LED demo task
    xTaskCreate((TaskFunction_t )Demo_task,
                (const char*    )"Demo_task",
                (uint16_t       )256/sizeof(StackType_t),
                (void*          )NULL,
                (UBaseType_t    )6,
                (TaskHandle_t*  )NULL);
}

int main(void)
{
	__disable_irq();

	Stm32_Clock_Init(9);	//系统时钟设置  8Mhz * 6 = 72MHZ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组4

	//初始化LED灯
	LED_init();

	//初始化串口
	UART1_Init(72,921600);

	//SPI init
	SPI1_Init();
	//DMA init
	SPI_DMA_Config( SPI_TX_DMA_CHANNEL, 
					(uint32_t)&SPI1->DR,
					(uint32_t)SPI_TxBuf,
					PIXEL_NUM * 24 * SPI_BYTE_LEN_PER_PIXEL);

	SPI1_DMA_IT_Config();

	//10khz的计数频率，中断一次  100us一次
	TIM3_Int_Init(999,7199);

	create_app_task();

	vTaskStartScheduler();//开启任务调度
	return 0;
}

void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3) != RESET) {
		dma_tx_flag = false;
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}



