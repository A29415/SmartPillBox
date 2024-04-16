#ifndef __DS3231_H
#define __DS3231_H
#include "sys.h"

#define DS3231_ADDRESS			0x68	//DS3231��7λ��ַ��1101000
#define DS3231_ADDRESS_Write	0xD0	//DS3231��8λд��ַ��1101 0000
#define DS3231_ADDRESS_Read		0xD1	//DS3231��8λ����ַ��1101 0001

#define DS3231_SEC_REG			0x00	//��Ĵ�����ַ
#define DS3231_MIN_REG			0x01	//�ּĴ�����ַ
#define DS3231_HOUR_REG			0x02	//ʱ�Ĵ�����ַ
#define DS3231_WEEK_REG			0x03	//�ܼĴ�����ַ
#define DS3231_DAY_REG			0x04	//�ռĴ�����ַ
#define DS3231_MONTH_REG		0x05	//�¼Ĵ�����ַ
#define DS3231_YEAR_REG			0x06	//��Ĵ�����ַ

#define DS3231_AL1SEC_REG		0x07	//����1��Ĵ�����ַ
#define DS3231_AL1MIN_REG		0x08	//����1�ּĴ�����ַ
#define DS3231_AL1HOUR_REG		0x09	//����1ʱ�Ĵ�����ַ
#define DS3231_AL1DATE_REG		0x0A	//����1�ռĴ�����ַ

#define DS3231_AL2MIN_REG		0x0B	//����2�ּĴ�����ַ
#define DS3231_AL2HOUR_REG		0x0C	//����2ʱ�Ĵ�����ַ
#define DS3231_AL2WDAY_REG		0x0D	//����2�ռĴ�����ַ


#define DS3231_CONTROL_REG		0x0E	//���ƼĴ�����ַ
#define DS3231_STATUS_REG		0x0F	//״̬�Ĵ�����ַ
#define DS3231_AGING_OFFSET_REG	0x10	//�����ϻ�ƫ�ƼĴ�����ַ
#define DS3231_TEMP_MSB_REG		0x11	//�¶ȼĴ�����λ��ַ
#define DS3231_TEMP_LSB_REG		0x12	//�¶ȼĴ�����λ��ַ

#define DS3231_AXMY			7
#define DS3231_DYDT			6

#define DS3231_REG_CONTROL 	0x0e		//���ƼĴ�����ַ
	#define DS3231_EOSC			7
	#define DS3231_BBSQW		6
	#define DS3231_CONV			5
	#define DS3231_RS2			4
	#define DS3231_RS1			3
	#define DS3231_INTCN		2
	#define DS3231_A2IE			1
	#define DS3231_A1IE			0

#define DS3231_REG_STATUS	0x0f		//״̬�Ĵ�����ַ
	#define DS3231_OSF			7
	#define DS3231_EN32KHZ		3
	#define DS3231_BSY			2
	#define DS3231_A2F			1
	#define DS3231_A1F			0


typedef enum DS3231_State
{
	DS3231_DISABLED, 
	DS3231_ENABLED
}DS3231_State;

typedef enum DS3231_InterruptMode
{
	DS3231_SQUARE_WAVE_INTERRUPT, 
	DS3231_ALARM_INTERRUPT
}DS3231_InterruptMode;

typedef enum D3231_Alarm1Mode{
	DS3231_A1_EVERY_S = 0x0f, 
	DS3231_A1_MATCH_S = 0x0e, 
	DS3231_A1_MATCH_S_M = 0x0c, 
	DS3231_A1_MATCH_S_M_H = 0x08, 
	DS3231_A1_MATCH_S_M_H_DATE = 0x00, 
	DS3231_A1_MATCH_S_M_H_DAY = 0x80,
}DS3231_Alarm1Mode;


typedef struct DateTime{
    uint8_t hour_t;
    uint8_t minute_t;
    uint8_t second_t;
    uint8_t week_t;
    uint8_t month_t;
    uint8_t day_t;
    uint16_t year_t;
}DateTime;

void DS3231_Init(void);
void DS3231_Write_Byte(uint8_t RegAddr, uint8_t data);
uint8_t DS3231_Read_Byte(uint8_t ReadAddr);
void DS3231_SetDate(uint16_t year, uint8_t month, uint8_t mday);
void DS3231_SetTime(uint8_t hour, uint8_t minute, uint8_t second);
void DS3231_SetWeek(uint8_t week);
static uint8_t BCD_To_DEC(uint8_t byte);
uint8_t DS3231_GetTime(DateTime *dt);
uint8_t DS3231_GetDate(DateTime *dt);
uint8_t DS3231_GetWeek(DateTime *dt);

void DS3231_SetInterruptMode(DS3231_InterruptMode mode);
void DS3231_EnableAlarm1(DS3231_State enable);
void DS3231_ClearAlarm1Flag(void);
void DS3231_SetAlarm1Second(uint8_t second);
void DS3231_SetAlarm1Minute(uint8_t minute);
void DS3231_SetAlarm1Hour(uint8_t hour_24mode);
void DS3231_SetAlarm1Mode(DS3231_Alarm1Mode alarmMode);
uint8_t DS3231_CheckINT(void);
uint8_t DS3231_IsAlarm1Triggered(void);

#endif


