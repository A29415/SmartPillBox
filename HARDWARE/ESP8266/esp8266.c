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
	函数名称：	ESP8266_Clear
	函数功能：	清空缓存
	入口参数：	无
	返回参数：	无
	说明：
==========================================================*/
void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

/*==========================================================
	函数名称：	ESP8266_WaitRecive
	函数功能：	等待接收完成
	入口参数：	无
	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
	说明：		循环调用检测是否接收完成
==========================================================*/
_Bool ESP8266_WaitRecive(void)
{
    if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return REV_WAIT;
    if(esp8266_cnt == esp8266_cntPre)	//如果上一次的值和这次相同，则说明接收完毕
    {
        esp8266_cnt = 0;							//清0接收计数
        return REV_OK;								//返回接收完成标志
    }
    esp8266_cntPre = esp8266_cnt;					//置为相同
    return REV_WAIT;								//返回接收未完成标志
}

/*==========================================================
	函数名称：	ESP8266_SendCmd
	函数功能：	发送命令
	入口参数：	cmd：命令
				      res：需要检查的返回指令
	返回参数：	0-成功	1-失败
	说明：
==========================================================*/
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200;
    Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
        {
            if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
            {
                ESP8266_Clear();									//清空缓存
                return 0;
            }
        }
        delay_ms(10);
    }
    return 1;
}

/*==========================================================
	函数名称：	ESP8266_SendData
	函数功能：	发送数据
	入口参数：	data：数据
				      len：长度
	返回参数：	无
	说明：
==========================================================*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdBuf[128]={0};
    ESP8266_Clear();								//清空接收缓存
    //先发送要发送数据的指令做准备
//    sprintf(cmdBuf, "AT+CIPSEND\r\n");		//发送命令
	printf("7. AT+CIPSEND\r\n");
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	
    if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
    {
        //既然准备完毕即可开始发送数据
        Usart_SendString(USART2, data, len);		//发送设备连接请求数据
    }
}

/*==========================================================
	函数名称：	ESP8266_GetIPD
  函数功能：	获取平台返回的数据
	入口参数：	等待的时间(乘以10ms)
	返回参数：	平台返回的原始数据
	说明：		  不同网络设备返回的格式不同，需要去调试
	如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
==========================================================*/
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
    char *ptrIPD = NULL;
    do
    {
        if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
        {
            ptrIPD = strstr((char *)esp8266_buf, "+MQTTSUBRECV");				//搜索“IPD”头
            if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
            {
                //UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strstr(ptrIPD, "{");							//找到':'
                if(ptrIPD != NULL)
                {
//                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                    return NULL;
            }
        }
        delay_ms(5);													//延时等待
    } while(timeOut--);
    return NULL;														//超时还未找到，返回空指针
}

/* 解析NOWapi返回的json数据 */
static void ParseDateTime(unsigned char *Time_Buff)
{
	char dateTime[20] = {0};
    int year, month, day, hour, minute, second, week;

	char *datetime_1 = strstr((const char *)Time_Buff, "datetime_1");	//定位到"datetime_1"起始位置
	if(datetime_1 != NULL)
	{
		datetime_1 += (strlen("datetime_1") + 3);
		strncpy(dateTime, datetime_1, 19);
		dateTime[19] = '\0';

        sscanf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
		DS3231_SetDate(year-2000, month, day);
        DS3231_SetTime(hour, minute, second+1);     //获取时间会慢两秒
	}
	
	
	char *week_1 = strstr((const char *)Time_Buff, "week_1");	//定位到"week_1"起始位
	if(week_1 != NULL)
	{
		week_1 += (strlen("week_1") + 3);
		week = *week_1 - '0';
		
		DS3231_SetWeek(week);
	}
	
	flag_getTime = 1;
}

/* 获取当前日期时间 */
void Get_CurrentDateTime(void)
{
	ESP8266_Clear();

	printf("5. AT+CIPSTART  NOWAPI conn...\r\n");
	while(ESP8266_SendCmd(TIME_TCP, "CONNECT"))	//建立TCP连接
		delay_ms(500);

	printf("6. AT+CIPMODE=0\r\n");
	while(ESP8266_SendCmd("AT+CIPMODE=0\r\n", "OK"))	//开启透传模式
		delay_ms(500);
	
	ESP8266_SendData((unsigned char *)TIME_GET, sizeof(TIME_GET));
	delay_ms(500);
	
//    strcpy((char *)Time_Buff, (char *)esp8266_buf);		//将返回的json格式的时间复制给Time_Buff
//    
//    ESP8266_Clear();

	ParseDateTime(esp8266_buf);

	ESP8266_Clear();
//	while(ESP8266_SendCmd("+++", ""));
}


/*==========================================================
	函数名称：	ESP8266_Init
	函数功能：	初始化ESP8266
	入口参数：	无
	返回参数：	无
	说明：
==========================================================*/
void ESP8266_Init(void)
{   
    ESP8266_Clear();

    printf("0. AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))//AT测试
        delay_ms(500);
    
    printf("1. AT+RST\r\n");
    ESP8266_SendCmd("AT+RST\r\n", "");		//ESP8266重启
    delay_ms(500);
	
//	printf("AT+CIPCLOSE\r\n");
//	ESP8266_SendCmd("AT+CIPCLOSE\r\n", "");
//	delay_ms(500);

    printf("2. AT+CWMODE=1\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))		//设置为STA模式
        delay_ms(500);
	
	printf("3. AT+CIPMUX=0\r\n");
	while(ESP8266_SendCmd("AT+CIPMUX=0\r\n", "OK"))		//设置单连接
		delay_ms(500);

    printf("4. AT+CWJAP\r\n");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))	//连接WIFI
        delay_ms(500);
}


