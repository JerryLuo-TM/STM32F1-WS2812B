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

//操作系统相关头文件
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" 
#include "semphr.h"

//设置FLASH 保存地址
//(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
//64K 开始的位置
#define FLASH_SAVE_ADDR   0X08010000   //频率参数
#define FLASH_ID_ADDR     0X08010400   //数传版本号

typedef union
{
		float d;
		unsigned char outuchar[4];
}Data;

typedef struct 
{
	unsigned long long CPU_ADD; //用于统计CPU占用率
	float Occupancy_Rate; 			//CPU占用率
}Status_Params;




#endif
