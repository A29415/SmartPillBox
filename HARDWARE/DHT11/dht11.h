#ifndef __DHT11_H
#define __DHT11_H
#include "sys.h"

//IO方向设置
#define DHT11_IO_IN()  {GPIOG->MODER&=~(3<<(11*2));GPIOG->MODER|=0<<(11*2);}	//PG11输入模式
#define DHT11_IO_OUT() {GPIOG->MODER&=~(3<<(11*2));GPIOG->MODER|=1<<(11*2);} 	//PG11输出模式
 
//IO操作函数											   
#define	DHT11_DQ_OUT    PGout(11)//数据端口	PG11
#define	DHT11_DQ_IN     PGin(11) //数据端口	PG11

#define DHT11_RCC 	RCC_AHB1Periph_GPIOG
#define DHT11_PORT 	GPIOG
#define DHT11_PIN 	GPIO_Pin_11

#define dht11_high() 	GPIO_SetBits(DHT11_PORT, DHT11_PIN)
#define dht11_low() 	GPIO_ResetBits(DHT11_PORT, DHT11_PIN)
#define read_data() 	GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)

void DHT11_Rst(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);
u8 DHT11_Init(void);

#endif

