#ifndef _Struct_H_
#define _Struct_H_

#include "stm32f10x.h"
#include <string.h>
#include "sys.h"

#include "ring_buffer.h"
#include "stdbool.h"
#include "timer.h"
#include "LED.h"
#include "delay.h"
#include "spi.h"
#include "stmflash.h"
#include "usart.h"

#include "TX_Module.h"

//����ϵͳ���ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" 
#include "semphr.h"

//����FLASH �����ַ
//(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
//64K ��ʼ��λ��
#define FLASH_SAVE_ADDR   0X08010000   //Ƶ�ʲ���
#define FLASH_ID_ADDR     0X08010400   //�����汾��

typedef union
{
		float d;
		unsigned char outuchar[4];
}Data;

typedef struct 
{
	unsigned long long CPU_ADD; //����ͳ��CPUռ����
	float Occupancy_Rate; 			//CPUռ����
}Status_Params;




#endif
