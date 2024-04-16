#ifndef __RGB_LED_H
#define __RGB_LED_H
#include "sys.h"

#define RGB1	1
#define RGB2	2

#define RGB_RED		1
#define RGB_BLUE	2
#define RGB_GREEN	3
#define RGB_BLACK	4

#define PIN_RED		GPIO_Pin_6
#define PIN_BLUE	GPIO_Pin_7
#define PIN_GREEN	GPIO_Pin_8

void RGB_SetColor(uint8_t RGBX, uint8_t Color);
void RGB_Init(void);


#endif

