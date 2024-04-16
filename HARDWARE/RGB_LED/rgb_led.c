#include "rgb_led.h"

void RGB_SetColor(uint8_t RGBX, uint8_t Color)
{
	if(RGBX == RGB1)
	{
		if(Color == RGB_RED)
		{
			GPIO_ResetBits(GPIOF, PIN_RED);
			GPIO_SetBits(GPIOF, PIN_BLUE);
			GPIO_SetBits(GPIOF, PIN_GREEN);
		}
		else if(Color == RGB_BLUE)
		{
			GPIO_SetBits(GPIOF, PIN_RED);
			GPIO_ResetBits(GPIOF, PIN_BLUE);
			GPIO_SetBits(GPIOF, PIN_GREEN);
		}
		else if(Color == RGB_GREEN)
		{
			GPIO_SetBits(GPIOF, PIN_RED);
			GPIO_SetBits(GPIOF, PIN_BLUE);
			GPIO_ResetBits(GPIOF, PIN_GREEN);
		}
		else
		{
			GPIO_SetBits(GPIOF, PIN_RED);
			GPIO_SetBits(GPIOF, PIN_BLUE);
			GPIO_SetBits(GPIOF, PIN_GREEN);
		}
	}
	else if(RGBX == RGB2)
	{
		if(Color == RGB_RED)
		{
			GPIO_ResetBits(GPIOG, PIN_RED);
			GPIO_SetBits(GPIOG, PIN_BLUE);
			GPIO_SetBits(GPIOG, PIN_GREEN);
		}
		else if(Color == RGB_BLUE)
		{
			GPIO_SetBits(GPIOG, PIN_RED);
			GPIO_ResetBits(GPIOG, PIN_BLUE);
			GPIO_SetBits(GPIOG, PIN_GREEN);
		}
		else if(Color == RGB_GREEN)
		{
			GPIO_SetBits(GPIOG, PIN_RED);
			GPIO_SetBits(GPIOG, PIN_BLUE);
			GPIO_ResetBits(GPIOG, PIN_GREEN);
		}
		else
		{
			GPIO_SetBits(GPIOG, PIN_RED);
			GPIO_SetBits(GPIOG, PIN_BLUE);
			GPIO_SetBits(GPIOG, PIN_GREEN);
		}
	}
}

/* RGB灯初始化 */
void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	//初始化RGB1引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;	//R B G
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//初始化RGB2引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;	//R B G
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RGB_SetColor(RGB1, RGB_BLACK);
	RGB_SetColor(RGB2, RGB_BLACK);
}


