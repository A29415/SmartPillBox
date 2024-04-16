#ifndef __JQ8900_H
#define __JQ8900_H
#include "sys.h"

#define SDA    PCout(0) //SDA	 
 
void JQ8900_Init(void);
void JQ8900_SendData ( u8 addr );    //·¢ËÍº¯Êý
void JQ8900_SetVolume(uint8_t volume);
void JQ8900_Stop(void);
void JQ8900_Start(void);
#endif
