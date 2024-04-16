#include "ds3231.h"
#include "iic2.h"
#include "delay.h"


/* DS3231��ʼ�� */
void DS3231_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	IIC2_Init();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    DS3231_Write_Byte(DS3231_CONTROL_REG, 0x40);
    DS3231_Write_Byte(DS3231_STATUS_REG, 0x00);
	
	DS3231_EnableAlarm1(DS3231_DISABLED);
	DS3231_ClearAlarm1Flag();
	DS3231_SetInterruptMode(DS3231_ALARM_INTERRUPT);
	DS3231_SetAlarm1Mode(DS3231_A1_MATCH_S_M_H);
	
    delay_ms(100);
}

/* ��DS3231д��һ���ֽ� */
void DS3231_Write_Byte(uint8_t RegAddr, uint8_t data)
{
	IIC2_Start();
	IIC2_Send_Byte(DS3231_ADDRESS_Write);
	IIC2_Wait_Ack();
	IIC2_Send_Byte(RegAddr);
	IIC2_Wait_Ack();
	IIC2_Send_Byte(data);
	IIC2_Wait_Ack();
	IIC2_Stop();
}

/* ��DS3231����һ���ֽ� */
uint8_t DS3231_Read_Byte(uint8_t ReadAddr)
{
	uint8_t data = 0;

	IIC2_Start();
	IIC2_Send_Byte(DS3231_ADDRESS_Write);
	IIC2_Wait_Ack();
	IIC2_Send_Byte(ReadAddr);
	IIC2_Wait_Ack();
	IIC2_Start();
	IIC2_Send_Byte(DS3231_ADDRESS_Read);
	IIC2_Wait_Ack();
	data = IIC2_Read_Byte(0);
	IIC2_Stop();

	return data;
}

/* ��������(�꣬�£���) */
void DS3231_SetDate(uint16_t year, uint8_t month, uint8_t mday)
{
	uint8_t temp_H, temp_L;
	
	temp_L = year % 10;
	temp_H = year / 10;
	year = (temp_H << 4) + temp_L;
	DS3231_Write_Byte(DS3231_YEAR_REG, year);

	temp_L = month % 10;
	temp_H = month / 10;
	month = (temp_H << 4) + temp_L;
	DS3231_Write_Byte(DS3231_MONTH_REG, month);

	temp_L = mday % 10;
	temp_H = mday / 10;
	mday = (temp_H << 4) + temp_L;
	DS3231_Write_Byte(DS3231_DAY_REG, mday);
}

/* ����ʱ��(ʱ���֣���) */
void DS3231_SetTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	uint8_t temp_H, temp_L;

	temp_L = hour % 10;
	temp_H = hour / 10;
	hour = (temp_H << 4) | temp_L;
	DS3231_Write_Byte(DS3231_HOUR_REG, hour);

	temp_L = minute % 10;
	temp_H = minute / 10;
	minute = (temp_H << 4) | temp_L;
	DS3231_Write_Byte(DS3231_MIN_REG, minute);

	temp_L = second % 10;
	temp_H = second / 10;
	second = (temp_H << 4) | temp_L;
	DS3231_Write_Byte(DS3231_SEC_REG, second);
}

/* ������ */
void DS3231_SetWeek(uint8_t week)
{
	DS3231_Write_Byte(DS3231_WEEK_REG, week);
}

/* BCD��תʮ���� */
static uint8_t BCD_To_DEC(uint8_t byte)
{
	uint8_t temp_H, temp_L;

	temp_L = byte & 0x0f;
	temp_H = (byte & 0xf0) >> 4;

	return (temp_H * 10) + temp_L;
}

/* ʮ����תBCD�� */
static uint8_t DS3231_EncodeBCD(uint8_t dec) {
	return (dec % 10 + ((dec / 10) << 4));
}

/* ��ȡʱ�� */
uint8_t DS3231_GetTime(DateTime *dt)
{
	uint8_t data = 0;

	data = DS3231_Read_Byte(DS3231_HOUR_REG);
	dt->hour_t = BCD_To_DEC(data);

	data = DS3231_Read_Byte(DS3231_MIN_REG);
	dt->minute_t = BCD_To_DEC(data);

	data = DS3231_Read_Byte(DS3231_SEC_REG);
	dt->second_t = BCD_To_DEC(data);

	return 0;
}

/* ��ȡ���� */
uint8_t DS3231_GetDate(DateTime *dt)
{
	uint8_t data = 0;

	data = DS3231_Read_Byte(DS3231_YEAR_REG);
	dt->year_t = BCD_To_DEC(data) + 2000;

	data = DS3231_Read_Byte(DS3231_MONTH_REG);
	dt->month_t = BCD_To_DEC(data);

	data = DS3231_Read_Byte(DS3231_DAY_REG);
	dt->day_t = BCD_To_DEC(data);

	return 0;
}

/* ��ȡ�� */
uint8_t DS3231_GetWeek(DateTime *dt)
{
	uint8_t data = 0;
	
	data = DS3231_Read_Byte(DS3231_WEEK_REG);
	dt->week_t = BCD_To_DEC(data);
	
	return 0;
}

/* ����INT/SQW����ģʽ */
void DS3231_SetInterruptMode(DS3231_InterruptMode mode)
{
	uint8_t control = DS3231_Read_Byte(DS3231_REG_CONTROL);
	DS3231_Write_Byte(DS3231_REG_CONTROL, (control & 0xFB) | ((mode & 0x01) << DS3231_INTCN));
}

/* ʹ������1 */
void DS3231_EnableAlarm1(DS3231_State enable)
{
	uint8_t control = DS3231_Read_Byte(DS3231_REG_CONTROL);
	DS3231_Write_Byte(DS3231_REG_CONTROL, (control & 0xFE) | ((enable & 0x01) << DS3231_A1IE));
	DS3231_SetInterruptMode(DS3231_ALARM_INTERRUPT);
}

/* �������1��־λ */
void DS3231_ClearAlarm1Flag(void)
{
	uint8_t status = DS3231_Read_Byte(DS3231_REG_STATUS);
	DS3231_Write_Byte(DS3231_REG_STATUS, status & ~(0x1 << DS3231_A1F));
}

/* ��������1��λ */
void DS3231_SetAlarm1Second(uint8_t second)
{
	uint8_t temp = DS3231_Read_Byte(DS3231_AL1SEC_REG) & 0x80;
	uint8_t a1m1 = temp | (DS3231_EncodeBCD(second) & 0x3F);
	DS3231_Write_Byte(DS3231_AL1SEC_REG, a1m1);
}

/* ��������1��λ */
void DS3231_SetAlarm1Minute(uint8_t minute)
{
	uint8_t temp = DS3231_Read_Byte(DS3231_AL1MIN_REG) & 0x80;
	uint8_t a1m2 = temp | (DS3231_EncodeBCD(minute) & 0x3F);
	DS3231_Write_Byte(DS3231_AL1MIN_REG, a1m2);
}

/* ��������1ʱλ */
void DS3231_SetAlarm1Hour(uint8_t hour_24mode)
{
	uint8_t temp = DS3231_Read_Byte(DS3231_AL1HOUR_REG) & 0x80;
	uint8_t a1m3 = temp | (DS3231_EncodeBCD(hour_24mode) & 0x3F);
	DS3231_Write_Byte(DS3231_AL1HOUR_REG, a1m3);
}

/**
 * @brief ��������1��ģʽ
 * @param alarmMode Alarm 1 mode, 
 *		DS3231_A1_EVERY_S, 			ÿ�뱨��һ��
 *		DS3231_A1_MATCH_S, 			��ƥ��ʱ����
 *		DS3231_A1_MATCH_S_M, 		����ƥ��ʱ����
 *		DS3231_A1_MATCH_S_M_H, 		ʱ����ƥ��ʱ����
 *		DS3231_A1_MATCH_S_M_H_DATE 	���ڡ�ʱ����ƥ��ʱ����
 *		DS3231_A1_MATCH_S_M_H_DAY	�ա�ʱ����ƥ��ʱ����
 */
void DS3231_SetAlarm1Mode(DS3231_Alarm1Mode alarmMode)
{
	uint8_t temp;
	temp = DS3231_Read_Byte(DS3231_AL1SEC_REG) & 0x7f;
	DS3231_Write_Byte(DS3231_AL1SEC_REG, temp | (((alarmMode >> 0) & 0x01) << DS3231_AXMY));
	temp = DS3231_Read_Byte(DS3231_AL1MIN_REG) & 0x7f;
	DS3231_Write_Byte(DS3231_AL1MIN_REG, temp | (((alarmMode >> 1) & 0x01) << DS3231_AXMY));
	temp = DS3231_Read_Byte(DS3231_AL1HOUR_REG) & 0x7f;
	DS3231_Write_Byte(DS3231_AL1HOUR_REG, temp | (((alarmMode >> 2) & 0x01) << DS3231_AXMY));
	temp = DS3231_Read_Byte(DS3231_AL1DATE_REG) & 0x7f;
	DS3231_Write_Byte(DS3231_AL1DATE_REG, temp | (((alarmMode >> 3) & 0x01) << DS3231_AXMY) | (alarmMode & 0x80));
}

uint8_t DS3231_CheckINT(void)
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}

/* ����Ƿ񴥷�����1 */
uint8_t DS3231_IsAlarm1Triggered(void)
{
	return (DS3231_Read_Byte(DS3231_REG_STATUS) >> DS3231_A1F) & 0x01;
}
