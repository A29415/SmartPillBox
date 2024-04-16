#ifndef __ESP8266_H
#define __ESP8266_H

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

extern unsigned char esp8266_buf[];


void ESP8266_Init(void);
void ESP8266_Clear(void);
_Bool ESP8266_WaitReceive(void);
_Bool ESP8266_SendCmd(char *cmd, char *res);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);
void Get_CurrentDateTime(void);
void MQTT_Conn(void);
void MQTT_Pub(const char *msg,const char* topic);

#endif


