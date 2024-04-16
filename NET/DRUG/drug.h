#ifndef __DRUG_H
#define __DRUG_H
#include "sys.h"

typedef struct DrugInfo{
	int  drugID;				//ҩ����
	char drugName[50];			//ҩ����
	char nextTakeTime[30];		//��һ�γ�ҩʱ��
	int  nextDate;				//��ʾ��һ�γ�ҩʱ���ǽ��ջ�������	0����,1����
	int  drugQuantity;			//ҩ������
	int	 storeNumber;			//�洢ҩ�ֱ��
	int  numberOfTimes;			//ÿ�մ���
	int  numberOfPer;			//ÿ������
}DrugInfo;

extern DrugInfo Drug[];
extern int DrugNum;
extern int Current_Index;

void Drug_Add(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, int numberOfTimes, int numberOfPer);
void Drug_Delete(int drugID);
void Drug_Update(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, int numberOfTimes, int numberOfPer);
void Drug_GetMinTime(void);

#endif

