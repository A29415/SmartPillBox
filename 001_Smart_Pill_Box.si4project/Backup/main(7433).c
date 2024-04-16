/* 接线表 */
/**
*   DS3231时钟模块：
*       32K <--> 悬空
*       SQW <--> 悬空
*       SCL <--> PB12
*       SDA <--> PB13
*       VCC <--> 3.3V
*       GND <--> GND
*
*   ESP8266WiFi模块：
*       3V3 <--> 3.3V
*       RX  <--> PA2(USART2_TX)
*       TX  <--> PA3(USART2_RX)
*       GND <--> GND
*
*	DHT11温湿度模块：
*		VCC <--> 5V
*		DAT <--> PG11
*		GND <--> GND
*
*	SG90舵机模块：
*		VCC(红) 	<--> 5V
*		GND(棕) 	<--> GND
*		信号线(橙) 	<--> PA7
*
*	AS608指纹模块：
*		V+/Vi	<--> 3.3V
*		TX		<--> PB11(USART3_RX)
*		RX		<--> PB10(USART3_TX)
*		GND		<--> GND
*		TCH/WAK <--> PD3
*		VA/Vt	<--> 3.3V
*		D+/U+	<--> 不接
*		D-/U-	<--> 不接
*
*	JQ8900-16P语音播报模块：
*		VPP		<--> PC0
*		GND		<--> GND
*		DC-5V	<--> 5V
*		SPK-	<--> 喇叭负极(黑线)
*		SPK+	<--> 喇叭正极(红线)
*/

/* SYSTEM */
#include "sys.h"
#include "delay.h"
#include "usart.h"
/* Hardware */
#include "led.h"
#include "lcd.h"
#include "touch.h"
#include "ds3231.h"
#include "esp8266.h"
#include "dht11.h"
#include "sg90.h"
#include "as608.h"
#include "jq8900.h"
#include "rgb_led.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
/* LVGL */
#include "lvgl_demo.h"
/* NET */
#include "mqtt.h"
#include "drug.h"


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);			//初始化延时函数
	uart_init(115200);     		//初始化串口1，用于打印信息
    uart2_init(115200);			//初始化串口2，用于与ESP8266通讯
	uart3_init(57600);			//初始化串口3，用于与指纹模块通讯
	LED_Init();		        	//初始化LED端口
	RGB_Init();					//初始化RGB灯模块
	LCD_Init();					//初始化4.3寸TFTLCD屏
	DHT11_Init();				//初始化DHT11模块
	SG90_Init(40000-1, 42-1);	//初始化SG90模块
	PS_StaGPIO_Init();			//初始化FR读状态引脚
	AS608_Init(); 				//初始化AS608模块
	JQ8900_Init();				//初始化JQ8900语音模块
	DS3231_Init();				//初始化DS3231时钟模块
    ESP8266_Init();         	//初始化ESP8266模块
	Get_CurrentDateTime();		//获取网络时间
	MQTT_Conn();				//连接MQTT服务器
	
	lvgl_demo();				//启动FreeRTOS
                         
    while(1)
    {	
//		RGB_SetColor(RGB1, RGB_RED);
//		delay_ms(1000);
//		RGB_SetColor(RGB1, RGB_BLUE);
//		delay_ms(1000);
//		RGB_SetColor(RGB1, RGB_GREEN);
//		delay_ms(1000);
//		RGB_SetColor(RGB1, RGB_BLACK);
//		delay_ms(1000);
//		RGB_SetColor(RGB2, RGB_RED);
//		delay_ms(1000);
//		RGB_SetColor(RGB2, RGB_BLUE);
//		delay_ms(1000);
//		RGB_SetColor(RGB2, RGB_GREEN);
//		delay_ms(1000);
    }
	
}




