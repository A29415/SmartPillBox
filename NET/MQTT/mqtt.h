#ifndef __MQTT_H
#define __MQTT_H
#include "drug.h"



void MQTT_Conn(void);
void MQTT_Pub(const char *msg,const char* topic);
void MQTT_BoxSwitch(char *json);
void MQTT_AlarmClock(char *json);
void MQTT_DrugInfo(char *json);

#endif

