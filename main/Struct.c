#include "Struct.h"

void Data_Conversion(float dat,Data d1,uint8_t *buf)
{
		d1.d=dat; //向共用体中传递浮点型数据
		*(buf++)=d1.outuchar[0];
		*(buf++)=d1.outuchar[1];
		*(buf++)=d1.outuchar[2];
		*(buf++)=d1.outuchar[3];
}
