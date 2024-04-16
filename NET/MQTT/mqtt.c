/* NET */
#include "mqtt.h"
#include "cJSON.h"
#include "drug.h"
/* HARDWARE */
#include "esp8266.h"
#include "jq8900.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "lvgl_demo.h"
/* SYS */
#include "usart.h"
#include "delay.h"
/* C */
#include <string.h>

#define MQTT_USERCFG			"AT+MQTTUSERCFG=0,1,\"STM32F407\",\"\",\"\",0,0,\"\"\r\n"
#define MQTT_CONN				"AT+MQTTCONN=0,\"broker.emqx.io\",1883,1\r\n"
#define MQTT_SUB				"AT+MQTTSUB=0,\"APP\",0\r\n"

	

/* MQTT�������� */
void MQTT_Pub(const char *msg,const char* topic)
{
	
	char cmdBuf[128];                   //cmd �������ݲ��ܹ����õĹ�С��  ����  ����ʧ�ܱ��η��ͣ����ǳ����ڷ�����֮��Ῠ��
	uint8_t len = strlen(msg);        //strlen ��ͳ�� \0 ָ��AT+MQTTPUBRAW=  �ᷢ�� \0
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+MQTTPUBRAW=0,\"%s\",%d,0,0\r\n",topic ,len);		//��������                      MQTT�̼�AT+MQTTPUBRAW=<LinkID>,<"topic">,<length>,<qos>,<retain>
	
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		ESP8266_Clear();
		
		Usart_SendString(USART2,(unsigned char*)msg, len);		//��������
		
		ESP8266_Clear();
		delay_ms(20); 
		
	}
	
	ESP8266_Clear();
	memset(cmdBuf, 0, sizeof(cmdBuf));
}

/* ҩ�п���json���ݽ��� */
void MQTT_BoxSwitch(char *json)
{
	char *p = strstr((const char*)json, "{");
	cJSON * root = cJSON_Parse(p);
	cJSON * item = cJSON_GetObjectItem(root, "boxSwitch");
	int val = item->valueint;
	
	if(val == 1)
	{
		xEventGroupSetBits(xEventGroup_SG90, SG90_ON);
		ESP8266_Clear();
	}
	if(val == 0)
	{
		xEventGroupSetBits(xEventGroup_SG90, SG90_OFF);
		ESP8266_Clear();
	}
	
	cJSON_Delete(root);
}

/* ҩ����Ϣjson���ݽ��� */
void MQTT_DrugInfo(char *json)
{
	char *p = strstr((const char*)json, "{");
	cJSON * root = cJSON_Parse(p);
	cJSON * item_operation = cJSON_GetObjectItem(root, "operation");
	cJSON * item_drugID = cJSON_GetObjectItem(root, "drugID");
	cJSON * item_drugName = cJSON_GetObjectItem(root, "drugName");
	cJSON * item_nextTakeTime = cJSON_GetObjectItem(root, "nextTakeTime");
	cJSON * item_nextDate = cJSON_GetObjectItem(root, "nextDate");
	cJSON * item_drugQuantity = cJSON_GetObjectItem(root, "drugQuantity");
	cJSON * item_storeNumber = cJSON_GetObjectItem(root, "storeNumber");
	cJSON * item_numberOfTimes = cJSON_GetObjectItem(root, "numberOfTimes");
	cJSON * item_numberOfPer = cJSON_GetObjectItem(root, "numberOfPer");

	int operation = item_operation->valueint;
	int drugID = item_drugID->valueint;
	char * drugName = item_drugName->valuestring;
	char * nextTakeTime = item_nextTakeTime->valuestring;
	int nextDate = item_nextDate->valueint;
	int drugQuantity = item_drugQuantity->valueint;
	int storeNumber = item_storeNumber->valueint;
	int numberOfTimes = item_numberOfTimes->valueint;
	int numberOfPer = item_numberOfPer->valueint;
	
	switch(operation)
	{
		case 1:
			Drug_Add(drugID, drugName, nextTakeTime, nextDate, drugQuantity, storeNumber, numberOfTimes, numberOfPer);	//���ҩ��
			break;
		case 2:
			Drug_Delete(drugID);													//ɾ��ҩ��
			break;
		case 3:
			Drug_Update(drugID, drugName, nextTakeTime, nextDate, drugQuantity, storeNumber, numberOfTimes, numberOfPer);	//�޸�ҩ����Ϣ
			break;
		default :
			break;
	}
	
	cJSON_Delete(root);
}

/* �������� */
void MQTT_AlarmClock(char *json)
{
	char *p = strstr((const char*)json, "{");
	cJSON * root = cJSON_Parse(p);
	cJSON * item_clock = cJSON_GetObjectItem(root, "alarm_clock");
	
	int val = item_clock->valueint;
	
	if(val == 1)
	{
		//������������
		xEventGroupSetBits(xEventGroup_JQ8900, JQ8900_ON);
		 
	}
	
	cJSON_Delete(root);
}

/* MQTT���������� */
void MQTT_Conn(void)
{	
	printf("AT+MQTTUSERCFG\r\n");
	while(ESP8266_SendCmd(MQTT_USERCFG, "OK"))	//����MQTT���û�����
		delay_ms(500);
	
	printf("AT+MQTTCONN\r\n");
	while(ESP8266_SendCmd(MQTT_CONN, "OK"))		//����MQTT Broker
		delay_ms(500);
	
	printf("AT+MQTTSUB\r\n");
	while(ESP8266_SendCmd(MQTT_SUB, "OK"))		//����Topic
		delay_ms(500);
}
