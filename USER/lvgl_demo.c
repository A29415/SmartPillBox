#include "lvgl_demo.h"
/* LVGL */
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
/* Hardware */
#include "dht11.h"
#include "as608.h"
#include "sg90.h"
#include "esp8266.h"
#include "jq8900.h"
#include "rgb_led.h"
#include "ds3231.h"
/* NET */
#include "mqtt.h"
#include "Drug.h"

uint8_t Flag_SG90 = 0;
uint8_t Flag_TakeDrug = 0;

uint8_t temperature[2], humidity[2];
char temp[30];

char pub_topic[256];	//用来暂存待发布的主题

/************************ FreeRTOS配置 ********************************/
#define START_TASK_PRIO     1           /* 任务优先级*/
#define START_STK_SIZE      256         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */

#define LV_DEMO_TASK_PRIO   2           /* 任务优先线*/
#define LV_DEMO_STK_SIZE    1024        /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;       /* 任务句柄 */
void lv_demo_task(void *pvParameters);  /* 任务函数 */

/* DHT11 TASK 任务 */
#define DHT11_TASK_PRIO		3
#define DHT11_STK_SIZE		512
TaskHandle_t DHT11Task_Handler;
void dht11_task(void *pvParameters);

/* MQTT_PUB TASK 任务 */
#define MQTT_PUB_TASK_PRIO	5
#define MQTT_PUB_STK_SIZE	1024
TaskHandle_t MQTT_PUBTask_Handler;
void mqtt_pub_task(void *pvParameters);

/* MQTT SUB TASK 任务 */
#define MQTT_SUB_TASK_PRIO	5
#define MQTT_SUB_STK_SIZE	2048
TaskHandle_t MQTT_SUBTask_Handler;
void mqtt_sub_task(void *pvParameters);

/* 药物数据处理任务 */
#define DRUG_TASK_PRIO		6                                    
#define DRUG_STK_SIZE		2048
TaskHandle_t DRUGTask_Handler;
void Drug_task(void *pvParameters);

/* AS608 TASK 任务 */
#define AS608_TASK_PRIO		3
#define AS608_STK_SIZE		512
TaskHandle_t AS608Task_Handler;
void as608_task(void *pvParameters);

/* SG90 TASK 任务 */
#define SG90_TASK_PRIO		4
#define SG90_STK_SIZE		512
TaskHandle_t SG90Task_Handler;
void sg90_task(void *pvParameters);

/* JQ8900 TASK 任务 */
#define JQ8900_TASK_PRIO	7
#define JQ8900_STK_SIZE		512
TaskHandle_t JQ8900Task_Handler;
void jq8900_task(void *pvParameters);

/* RGB_LED TASK 任务 */
#define RGB_TASK_PRIO		2
#define RGB_STK_SIZE		256
TaskHandle_t RGBTask_Handler;
void rgb_task(void *pvParameters);

/* DS3231闹钟 TASK 任务 */
#define CLOCK_TASK_PRIO		8
#define CLOCK_STK_SIZE		512
TaskHandle_t CLOCKTask_Handler;
void clock_task(void *pvParameters);


//事件组
EventGroupHandle_t xEventGroup_MQTT_PUB = NULL;
EventGroupHandle_t xEventGroup_SG90 = NULL;
EventGroupHandle_t xEventGroup_DrugInfo = NULL;
EventGroupHandle_t xEventGroup_JQ8900 = NULL;
//互斥量
SemaphoreHandle_t xMQTTMutex = NULL;				

void lvgl_demo(void)
{
	lv_init();                                          /* lvgl系统初始化*/
    lv_port_disp_init();                                /* lvgl显示接口初始化放在lv_init()的后面*/
    lv_port_indev_init();                               /* lvgl输入接口初始化放在lv_init()的后面*/

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数*/
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级*/
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度*/
}

/* 开始任务 */
void start_task(void *pvParameters)
{
	pvParameters = pvParameters;
	
	xEventGroup_MQTT_PUB = xEventGroupCreate();		//创建事件组
	xEventGroup_SG90 = xEventGroupCreate();
	xEventGroup_DrugInfo = xEventGroupCreate();
	xEventGroup_JQ8900 = xEventGroupCreate();
	
	xMQTTMutex = xSemaphoreCreateMutex();			//创建互斥量
	
	taskENTER_CRITICAL();           /* 进入临界区*/
	
	/* 创建LVGL任务 */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);
				
	/* DHT11获取温湿度任势*/
	xTaskCreate((TaskFunction_t)dht11_task,
				(const char*   )"dht11_task",
				(uint16_t      )DHT11_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )DHT11_TASK_PRIO,
				(TaskHandle_t* )&DHT11Task_Handler);
	
	/* MQTT_PUB任务 */
	xTaskCreate((TaskFunction_t)mqtt_pub_task,
				(const char*   )"mqtt_pub_task",
				(uint16_t      )MQTT_PUB_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )MQTT_PUB_TASK_PRIO,
				(TaskHandle_t* )&MQTT_PUBTask_Handler);
				
	/* MQTT_SUB任务 */
	xTaskCreate((TaskFunction_t)mqtt_sub_task,
				(const char*   )"mqtt_sub_task",
				(uint16_t      )MQTT_SUB_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )MQTT_SUB_TASK_PRIO,
				(TaskHandle_t* )&MQTT_SUBTask_Handler);
				
		/* 药物数据处理任务 */
	xTaskCreate((TaskFunction_t)Drug_task,
				(const char*   )"Drug_task",
				(uint16_t      )DRUG_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )DRUG_TASK_PRIO,
				(TaskHandle_t* )&DRUGTask_Handler);
				
	/* AS608指纹验证任务 */
	xTaskCreate((TaskFunction_t)as608_task,
				(const char*   )"as608_task",
				(uint16_t      )AS608_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )AS608_TASK_PRIO,
				(TaskHandle_t* )&AS608Task_Handler);
				
	/* SG90舵机模拟开关任势*/
	xTaskCreate((TaskFunction_t)sg90_task,
				(const char*   )"sg90_task",
				(uint16_t      )SG90_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )SG90_TASK_PRIO,
				(TaskHandle_t* )&SG90Task_Handler);
				
	/* JQ8900语音播报任势*/
	xTaskCreate((TaskFunction_t)jq8900_task,
				(const char*   )"jq8900_task",
				(uint16_t      )JQ8900_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )JQ8900_TASK_PRIO,
				(TaskHandle_t* )&JQ8900Task_Handler);
				
	/* RGB灯任势*/
	xTaskCreate((TaskFunction_t)rgb_task,
				(const char*   )"rgb_task",
				(uint16_t      )RGB_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )RGB_TASK_PRIO,
				(TaskHandle_t* )&RGBTask_Handler);
				
	/* DS3231闹钟任势*/
	xTaskCreate((TaskFunction_t)clock_task,
				(const char*   )"clock_task",
				(uint16_t      )CLOCK_STK_SIZE,
				(void*         )NULL,
				(UBaseType_t   )CLOCK_TASK_PRIO,
				(TaskHandle_t* )&CLOCKTask_Handler);
				
	taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(StartTask_Handler); /* 删除开始任务*/
}

/* LVGL任务 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
	
	ui_init();		//LVGL UI界面初始化
	
	_ui_screen_change(&ui_MainScr, LV_SCR_LOAD_ANIM_FADE_ON, 0, 100, &ui_MainScr_screen_init);
	_ui_screen_delete(&ui_StartScr);
    
    while(1)
    {
        lv_timer_handler(); /* LVGL计时器*/
		vTaskDelay(100);
	}
}

/* DHT11获取温湿度*/
void dht11_task(void *pvParameters)
{
	while(1)
	{	
		DHT11_Read_Data(temperature, humidity);		//获取温湿度
		
		sprintf(temp, "%d.%d", temperature[0], temperature[1]);
		lv_label_set_text(ui_Temperature, temp);
		lv_obj_set_style_text_font(ui_Temperature, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		sprintf(temp, "%d.%d", humidity[0], humidity[1]);
		lv_label_set_text(ui_Humidity, temp);
		lv_obj_set_style_text_font(ui_Humidity, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		xEventGroupSetBits(xEventGroup_MQTT_PUB, PUB_DHT11);
		
		vTaskDelay(2000);
	}
}

/* AS608指纹验证 */
void as608_task(void *pvParameters)
{
	while(1)
	{
		if(PS_Sta)	//如果手指按下
		{
			if(press_FR() == 1)	//如果识别成功
			{
				xEventGroupSetBits(xEventGroup_SG90, SG90_ON);
				xEventGroupClearBits(xEventGroup_JQ8900, JQ8900_ON);
				xEventGroupSetBits(xEventGroup_JQ8900, JQ8900_OFF);
				xEventGroupClearBits(xEventGroup_JQ8900, JQ8900_ON | JQ8900_OFF);
				printf("指纹识别成功");
			}
			else if(press_FR() == 0)
			{
				xEventGroupSetBits(xEventGroup_SG90, SG90_OFF);
				printf("指纹识别失败");
			}
		}
		vTaskDelay(100);
	}
}

/* 计算下一次服药时间 */
static uint8_t Computing_Time(char *timeStr)
{
	int hours, minutes, seconds;
	int temp_numberOfTimes = Drug[Current_Index].numberOfTimes;		//获取每日服用次数
	int addNumer = 24 / temp_numberOfTimes;		//获取增量
	
	uint8_t res = 0;

	sscanf(Drug[Current_Index].nextTakeTime, "%d:%d:%d", &hours, &minutes, &seconds);

	if(hours + addNumer >= 24) 
	{
		res = 1;
	}
	
	hours = (hours + addNumer) % 24;

	sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
	
	return res;	//返回值表示：下一次服药时间是否是明天，1是0否
}

/* 更新药物信息 */
static void Update_DrugInfo(void)
{
	uint8_t result;
	
	int temp_drugID = 0;
	char temp_drugName[50];
	char temp_nextTakeTime[30];
	int temp_nextDate;
	int temp_drugQuantity, temp_storeNumber, temp_numberOfTimes, temp_numberOfPer;

	temp_drugID = Drug[Current_Index].drugID;			//获取当前药物ID
	strcpy(temp_drugName, Drug[Current_Index].drugName);	//获取当前药物名
	
	result = Computing_Time(Drug[Current_Index].nextTakeTime);
	strcpy(temp_nextTakeTime, Drug[Current_Index].nextTakeTime);	//获取计算后的下一次服药时间
	
	if(result == 1)	//如果下一次时间为明天
	{
		temp_nextDate = 1;
	}
	else
	{
		temp_nextDate = 0;
	}

	temp_drugQuantity = Drug[Current_Index].drugQuantity - Drug[Current_Index].numberOfPer;	//获取减少后药物数量
	temp_storeNumber = Drug[Current_Index].storeNumber;		//获取存储的药仓编号
	temp_numberOfTimes = Drug[Current_Index].numberOfTimes;	//获取每日服药次数
	temp_numberOfPer = Drug[Current_Index].numberOfPer;		//获取每次服药粒数

	Drug_Update(temp_drugID, temp_drugName, temp_nextTakeTime, temp_nextDate, temp_drugQuantity, temp_storeNumber, temp_numberOfTimes, temp_numberOfPer);
	
	//同步更新DrugManageScr界面UI
	xEventGroupSetBits(xEventGroup_DrugInfo, DRUG_DISPLAY);
}

/* SG90模拟药盒开兿*/
void sg90_task(void *pvParameters)
{
	EventBits_t eventBit;
	int i = 0;
	
	while(1)
	{	
		eventBit = xEventGroupWaitBits(xEventGroup_SG90, SG90_ON | SG90_OFF, pdTRUE, pdFALSE, portMAX_DELAY);
		if(eventBit == SG90_ON)
		{
			Flag_SG90 = 1;
			
			SG90_SetAngle(180);
			lv_label_set_text(ui_BoxState, "开启");
			lv_obj_set_style_text_font(ui_BoxState, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		}
		else if(eventBit == SG90_OFF)
		{
			Flag_SG90 = 0;
			
			SG90_SetAngle(0);
			lv_label_set_text(ui_BoxState, "关闭");
		 	lv_obj_set_style_text_font(ui_BoxState, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);

			if(Flag_TakeDrug == 1)	//如果是在服用药物后关闭药盒，则更新药物信息，并更新DrugManageScr界面
			{
				Flag_TakeDrug = 0;

				//更新药物信息
				Update_DrugInfo();
			}

			//药盒关闭，所有RGB灯熄灭
			for(i = 0; i < DrugNum; i++)
			{
				RGB_SetColor(Drug[i].storeNumber, RGB_BLACK);
			}
		}
		//发布药盒开关的状态
		xEventGroupSetBits(xEventGroup_MQTT_PUB, PUB_SG90);
		
		vTaskDelay(100);
	}
}

/* JQ8900语音播放任务 */
void jq8900_task(void *pvParameters)
{
	EventBits_t eventBit;
	
	while(1)
	{
		eventBit = xEventGroupWaitBits(xEventGroup_JQ8900, JQ8900_ON | JQ8900_OFF, pdFALSE, pdFALSE, portMAX_DELAY);
		if(eventBit == JQ8900_ON)
		{
			JQ8900_Start();
		}
		else if(eventBit == JQ8900_OFF)
		{
			JQ8900_Stop();
		}
		
		vTaskDelay(5000);
	}
}

/* RGB灯显示任务 */
void rgb_task(void *pvParameters)
{
	int drug_count = 0;
	int storeID = 0;
	int i = 0;
	
	while(1)
	{
		xEventGroupWaitBits(xEventGroup_SG90, 
							SG90_ON, 
							pdTRUE, 
							pdFALSE, 
							portMAX_DELAY);
		storeID = Drug[Current_Index].storeNumber;	//获取当前服药药物的药仓编号

		if(Flag_SG90 && Flag_TakeDrug)	//如果药盒已打开并且到达吃药时间，对应药仓显示绿色
		{
			if(storeID == RGB1)
			{
				RGB_SetColor(RGB1, RGB_GREEN);
			}
			else if(storeID == RGB2)
			{
				RGB_SetColor(RGB2, RGB_GREEN);
			}
		}
		else if(Flag_SG90 && !Flag_TakeDrug)	//如果药盒已打但是没有到达吃药时间，药仓显示不同程度的颜色
		{
			for(i = 0; i < DrugNum; i++)
			{
				drug_count = Drug[i].drugQuantity;
				if(drug_count < 4)	//如果药仓药物数量小于4，亮起红灯，否则蓝灯
				{
					RGB_SetColor(Drug[i].storeNumber, RGB_RED);
				}
				else
				{
					RGB_SetColor(Drug[i].storeNumber, RGB_BLUE);
				}
			}
		}
	}
}

/* MQTT_PUB发布主题任务 */
void mqtt_pub_task(void *pvParameters)
{
	EventBits_t eventBit;
	
	char t[5], h[5];
	
	while(1)
	{
		eventBit = xEventGroupWaitBits(xEventGroup_MQTT_PUB, 
										PUB_DHT11 | PUB_SG90 | PUB_ALARM,
										pdTRUE,
										pdFALSE,
										portMAX_DELAY);
		
		//发布与订阅处理进行互斥
		if(xSemaphoreTake(xMQTTMutex, portMAX_DELAY) == pdPASS)
		{
			if(eventBit == PUB_DHT11)
			{
				sprintf(t, "%d.%1d", temperature[0], temperature[1]);
				sprintf(h, "%d.%1d", humidity[0], humidity[1]);
				
				sprintf(pub_topic, "{\"temperature\":%s,\"humidity\":%s}", t, h);
				MQTT_Pub(pub_topic, "STM32F407/DHT11");
			}
			else if(eventBit == PUB_SG90)
			{
				if(Flag_SG90)
				{
					sprintf(pub_topic, "{\"boxSwitch\":1}");
					MQTT_Pub(pub_topic, "STM32F407/SG90");
				}
				else
				{
					sprintf(pub_topic, "{\"boxSwitch\":0}");
					MQTT_Pub(pub_topic, "STM32F407/SG90");
				}
			}
			else if(eventBit ==  PUB_ALARM)
			{
				sprintf(pub_topic, "{\"alarm_clock\":1,\"drugID\":%d}", Drug[Current_Index].drugID);
				MQTT_Pub(pub_topic, "STM32F407/ALARM");
			}
			xSemaphoreGive(xMQTTMutex);
		}
		
		vTaskDelay(200);
	}
}

/* MQTT_SUB订阅处理任务 */
void mqtt_sub_task(void *pvParameters)
{
	char *mqtt_dat, *item;
	
	while(1)
	{
		if(xSemaphoreTake(xMQTTMutex, portMAX_DELAY) == pdPASS)	//获取互斥量
		{
			mqtt_dat = (char*)ESP8266_GetIPD(100);
			
			item = strstr((const char*)mqtt_dat, "boxSwitch");	//药盒开关
			if(item != NULL)
			{
				MQTT_BoxSwitch(mqtt_dat);
			}
			
			item = strstr((const char*)mqtt_dat, "operation");	//药物信息增删改
			if(item != NULL)
			{
				MQTT_DrugInfo(mqtt_dat);	//解析药物json数据
				xEventGroupSetBits(xEventGroup_DrugInfo, DRUG_DISPLAY);		//解析完毕后，准备更新药物UI
			}
			
			item = strstr((const char*)mqtt_dat, "alarm_clock");	//提醒服药
			if(item != NULL)
			{
				Flag_TakeDrug = 1;
				MQTT_AlarmClock(mqtt_dat);
			}
			
			xSemaphoreGive(xMQTTMutex);		//释放互斥量
		}
		
		vTaskDelay(200);
	}
}

/* 更新药物信息展示界面UI */
static void Update_DrugManageScr(void)
{
	int i = 0;
	
	lv_table_set_row_cnt(ui_DrugTable, DrugNum+1);
    lv_table_set_col_cnt(ui_DrugTable, 5);
	
	for(i = 1; i <= DrugNum; i++)	//序号
	{
		sprintf(temp, "%d", i);
		lv_table_set_cell_value(ui_DrugTable, i, 0, temp);
	}
	
	for(i = 0; i < DrugNum; i++)	//药物名
	{
		lv_table_set_cell_value(ui_DrugTable, i+1, 1, Drug[i].drugName);
	}
	
	for(i = 0; i < DrugNum; i++)	//数量
	{
		sprintf(temp, "%d粒", Drug[i].drugQuantity);
		lv_table_set_cell_value(ui_DrugTable, i+1, 2, temp);
	}
	
	for(i = 0; i < DrugNum; i++)	//药仓编号
	{
		sprintf(temp, "%d号", Drug[i].storeNumber);
		lv_table_set_cell_value(ui_DrugTable, i+1, 3, temp);
	}
	
	for(i = 0; i < DrugNum; i++)	//服用剂量
	{
		sprintf(temp, "每日%d次\n每次%d粒", Drug[i].numberOfTimes, Drug[i].numberOfPer);
		lv_table_set_cell_value(ui_DrugTable, i+1, 4, temp);
	}
	
	lv_obj_set_style_text_font(ui_DrugTable, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_PART_ITEMS);
}

/* 药物信息UI更新 */
void Drug_task(void *pvParameters)
{
	int hours, minutes, seconds;
	
	while(1)
	{
		//等待药物json数据解析完毕
		xEventGroupWaitBits(xEventGroup_DrugInfo, DRUG_DISPLAY, pdTRUE, pdFALSE, portMAX_DELAY);
		
		//按照服药时间排序
		Drug_GetMinTime();
		
		//设置闹钟，到点提醒
		sscanf(Drug[Current_Index].nextTakeTime, "%d:%d:%d", &hours, &minutes, &seconds);
		DS3231_SetAlarm1Second(seconds);
		DS3231_SetAlarm1Minute(minutes);
		DS3231_SetAlarm1Hour(hours);
		
		//更新MainScr界面UI，显示最早服药时间的药物信息
		sprintf(temp, "%s", Drug[Current_Index].nextTakeTime);
		lv_label_set_text(ui_NextTime, temp);
		lv_obj_set_style_text_font(ui_NextTime, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		sprintf(temp, "%s", Drug[Current_Index].drugName);
		lv_label_set_text(ui_DrugName, temp);
		lv_obj_set_style_text_font(ui_DrugName, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		sprintf(temp, "%d粒", Drug[Current_Index].numberOfPer);
		lv_label_set_text(ui_DrugNumber, temp);
		lv_obj_set_style_text_font(ui_DrugNumber, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		//更新DrugManageScr界面UI
		Update_DrugManageScr();
		
		vTaskDelay(100);
	}
}

void clock_task(void *pvParameters)
{
	while(1)
	{
		if(DS3231_IsAlarm1Triggered() == 1)	//闹钟到达
		{
			//开启语音播报
			xEventGroupSetBits(xEventGroup_JQ8900, JQ8900_ON);
			Flag_TakeDrug = 1;
			
			//准备向服务器发送提醒
			xEventGroupSetBits(xEventGroup_MQTT_PUB, PUB_ALARM);
			
			DS3231_ClearAlarm1Flag();
		}
		
		
		vTaskDelay(500);
	}
}
