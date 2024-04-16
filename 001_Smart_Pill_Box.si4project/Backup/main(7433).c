/* ���߱� */
/**
*   DS3231ʱ��ģ�飺
*       32K <--> ����
*       SQW <--> ����
*       SCL <--> PB12
*       SDA <--> PB13
*       VCC <--> 3.3V
*       GND <--> GND
*
*   ESP8266WiFiģ�飺
*       3V3 <--> 3.3V
*       RX  <--> PA2(USART2_TX)
*       TX  <--> PA3(USART2_RX)
*       GND <--> GND
*
*	DHT11��ʪ��ģ�飺
*		VCC <--> 5V
*		DAT <--> PG11
*		GND <--> GND
*
*	SG90���ģ�飺
*		VCC(��) 	<--> 5V
*		GND(��) 	<--> GND
*		�ź���(��) 	<--> PA7
*
*	AS608ָ��ģ�飺
*		V+/Vi	<--> 3.3V
*		TX		<--> PB11(USART3_RX)
*		RX		<--> PB10(USART3_TX)
*		GND		<--> GND
*		TCH/WAK <--> PD3
*		VA/Vt	<--> 3.3V
*		D+/U+	<--> ����
*		D-/U-	<--> ����
*
*	JQ8900-16P��������ģ�飺
*		VPP		<--> PC0
*		GND		<--> GND
*		DC-5V	<--> 5V
*		SPK-	<--> ���ȸ���(����)
*		SPK+	<--> ��������(����)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);			//��ʼ����ʱ����
	uart_init(115200);     		//��ʼ������1�����ڴ�ӡ��Ϣ
    uart2_init(115200);			//��ʼ������2��������ESP8266ͨѶ
	uart3_init(57600);			//��ʼ������3��������ָ��ģ��ͨѶ
	LED_Init();		        	//��ʼ��LED�˿�
	RGB_Init();					//��ʼ��RGB��ģ��
	LCD_Init();					//��ʼ��4.3��TFTLCD��
	DHT11_Init();				//��ʼ��DHT11ģ��
	SG90_Init(40000-1, 42-1);	//��ʼ��SG90ģ��
	PS_StaGPIO_Init();			//��ʼ��FR��״̬����
	AS608_Init(); 				//��ʼ��AS608ģ��
	JQ8900_Init();				//��ʼ��JQ8900����ģ��
	DS3231_Init();				//��ʼ��DS3231ʱ��ģ��
    ESP8266_Init();         	//��ʼ��ESP8266ģ��
	Get_CurrentDateTime();		//��ȡ����ʱ��
	MQTT_Conn();				//����MQTT������
	
	lvgl_demo();				//����FreeRTOS
                         
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




