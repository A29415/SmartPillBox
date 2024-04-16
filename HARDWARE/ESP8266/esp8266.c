#include "esp8266.h"
#include "ds3231.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#define TIME_TCP				"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\r\n"
#define TIME_GET				"GET http://api.k780.com/?app=life.time&appkey=71788&sign=d82205afc67641ae0150e34a86db5dd2&format=json\r\n"

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"ljxx\",\"12345678\"\r\n"
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"LZW\",\"lzw123456\"\r\n"


extern DateTime dt;

uint8_t flag_getTime = 0;

unsigned char esp8266_buf[256];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
//unsigned char Time_Buff[100];

/*==========================================================
	�������ƣ�	ESP8266_Clear
	�������ܣ�	��ջ���
	��ڲ�����	��
	���ز�����	��
	˵����
==========================================================*/
void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

/*==========================================================
	�������ƣ�	ESP8266_WaitRecive
	�������ܣ�	�ȴ��������
	��ڲ�����	��
	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
	˵����		ѭ�����ü���Ƿ�������
==========================================================*/
_Bool ESP8266_WaitRecive(void)
{
    if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
        return REV_WAIT;
    if(esp8266_cnt == esp8266_cntPre)	//�����һ�ε�ֵ�������ͬ����˵���������
    {
        esp8266_cnt = 0;							//��0���ռ���
        return REV_OK;								//���ؽ�����ɱ�־
    }
    esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
    return REV_WAIT;								//���ؽ���δ��ɱ�־
}

/*==========================================================
	�������ƣ�	ESP8266_SendCmd
	�������ܣ�	��������
	��ڲ�����	cmd������
				      res����Ҫ���ķ���ָ��
	���ز�����	0-�ɹ�	1-ʧ��
	˵����
==========================================================*/
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200;
    Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
        {
            if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
            {
                ESP8266_Clear();									//��ջ���
                return 0;
            }
        }
        delay_ms(10);
    }
    return 1;
}

/*==========================================================
	�������ƣ�	ESP8266_SendData
	�������ܣ�	��������
	��ڲ�����	data������
				      len������
	���ز�����	��
	˵����
==========================================================*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdBuf[128]={0};
    ESP8266_Clear();								//��ս��ջ���
    //�ȷ���Ҫ�������ݵ�ָ����׼��
//    sprintf(cmdBuf, "AT+CIPSEND\r\n");		//��������
	printf("7. AT+CIPSEND\r\n");
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	
    if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
    {
        //��Ȼ׼����ϼ��ɿ�ʼ��������
        Usart_SendString(USART2, data, len);		//�����豸������������
    }
}

/*==========================================================
	�������ƣ�	ESP8266_GetIPD
  �������ܣ�	��ȡƽ̨���ص�����
	��ڲ�����	�ȴ���ʱ��(����10ms)
	���ز�����	ƽ̨���ص�ԭʼ����
	˵����		  ��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
	��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
==========================================================*/
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
    char *ptrIPD = NULL;
    do
    {
        if(ESP8266_WaitRecive() == REV_OK)								//����������
        {
            ptrIPD = strstr((char *)esp8266_buf, "+MQTTSUBRECV");				//������IPD��ͷ
            if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
            {
                //UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strstr(ptrIPD, "{");							//�ҵ�':'
                if(ptrIPD != NULL)
                {
//                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                    return NULL;
            }
        }
        delay_ms(5);													//��ʱ�ȴ�
    } while(timeOut--);
    return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
}

/* ����NOWapi���ص�json���� */
static void ParseDateTime(unsigned char *Time_Buff)
{
	char dateTime[20] = {0};
    int year, month, day, hour, minute, second, week;

	char *datetime_1 = strstr((const char *)Time_Buff, "datetime_1");	//��λ��"datetime_1"��ʼλ��
	if(datetime_1 != NULL)
	{
		datetime_1 += (strlen("datetime_1") + 3);
		strncpy(dateTime, datetime_1, 19);
		dateTime[19] = '\0';

        sscanf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
		DS3231_SetDate(year-2000, month, day);
        DS3231_SetTime(hour, minute, second+1);     //��ȡʱ���������
	}
	
	
	char *week_1 = strstr((const char *)Time_Buff, "week_1");	//��λ��"week_1"��ʼλ
	if(week_1 != NULL)
	{
		week_1 += (strlen("week_1") + 3);
		week = *week_1 - '0';
		
		DS3231_SetWeek(week);
	}
	
	flag_getTime = 1;
}

/* ��ȡ��ǰ����ʱ�� */
void Get_CurrentDateTime(void)
{
	ESP8266_Clear();

	printf("5. AT+CIPSTART  NOWAPI conn...\r\n");
	while(ESP8266_SendCmd(TIME_TCP, "CONNECT"))	//����TCP����
		delay_ms(500);

	printf("6. AT+CIPMODE=0\r\n");
	while(ESP8266_SendCmd("AT+CIPMODE=0\r\n", "OK"))	//����͸��ģʽ
		delay_ms(500);
	
	ESP8266_SendData((unsigned char *)TIME_GET, sizeof(TIME_GET));
	delay_ms(500);
	
//    strcpy((char *)Time_Buff, (char *)esp8266_buf);		//�����ص�json��ʽ��ʱ�临�Ƹ�Time_Buff
//    
//    ESP8266_Clear();

	ParseDateTime(esp8266_buf);

	ESP8266_Clear();
//	while(ESP8266_SendCmd("+++", ""));
}


/*==========================================================
	�������ƣ�	ESP8266_Init
	�������ܣ�	��ʼ��ESP8266
	��ڲ�����	��
	���ز�����	��
	˵����
==========================================================*/
void ESP8266_Init(void)
{   
    ESP8266_Clear();

    printf("0. AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))//AT����
        delay_ms(500);
    
    printf("1. AT+RST\r\n");
    ESP8266_SendCmd("AT+RST\r\n", "");		//ESP8266����
    delay_ms(500);
	
//	printf("AT+CIPCLOSE\r\n");
//	ESP8266_SendCmd("AT+CIPCLOSE\r\n", "");
//	delay_ms(500);

    printf("2. AT+CWMODE=1\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))		//����ΪSTAģʽ
        delay_ms(500);
	
	printf("3. AT+CIPMUX=0\r\n");
	while(ESP8266_SendCmd("AT+CIPMUX=0\r\n", "OK"))		//���õ�����
		delay_ms(500);

    printf("4. AT+CWJAP\r\n");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))	//����WIFI
        delay_ms(500);
}


