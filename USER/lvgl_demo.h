/**
 ****************************************************************************************************
 * @file        lvgl_demo.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8操作系统移植 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */


#ifndef __LVGL_DEMO_H
#define __LVGL_DEMO_H
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"

#include "ui.h"

extern EventGroupHandle_t xEventGroup_SG90;
extern EventGroupHandle_t xEventGroup_JQ8900;

extern SemaphoreHandle_t xMutexSemaphore_PUB;
extern SemaphoreHandle_t xUSART2Mutex;

/*
*	bit0：LVGL界面初始化
*	bit1：获取到网络时间
*/
/* EventGroupHandle_t xEventGroup */
#define LVGL_INIT	(1<<0)
#define GET_TIME	(1<<1)
/* EventGroupHandle_t xEventGroup_Sensor */
#define DHT11		(1<<0)
#define AS608		(1<<1)
#define SG90		(1<<2)
/* EventGroupHandle_t xEventGroup_SG90 */
#define SG90_ON		(1<<0)
#define SG90_OFF	(1<<1)
/* EventGroupHandle_t xEventGroup_DrugInfo */
#define DRUG_ADD	(1<<0)
#define DRUG_DELETE	(1<<1)
#define DRUG_UPDATE	(1<<2)
#define DRUG_DISPLAY (1<<3)
/* EventGroupHandle_t xEventGroup_MQTT_PUB */
#define PUB_DHT11	(1<<0)
#define PUB_SG90	(1<<1)
#define PUB_ALARM	(1<<2)
/* EventGroupHandle_t xEventGroup_JQ8900 */
#define JQ8900_ON	(1<<0)
#define JQ8900_OFF	(1<<1)

void lvgl_demo(void);

#endif

