#ifndef __DRUG_H
#define __DRUG_H
#include "sys.h"

typedef struct DrugInfo{
	int  drugID;				//药物编号
	char drugName[50];			//药物名
	char nextTakeTime[30];		//下一次吃药时间
	int  nextDate;				//表示下一次吃药时间是今日还是明日	0今日,1明日
	int  drugQuantity;			//药物数量
	int	 storeNumber;			//存储药仓编号
	int  numberOfTimes;			//每日次数
	int  numberOfPer;			//每次粒数
}DrugInfo;

extern DrugInfo Drug[];
extern int DrugNum;
extern int Current_Index;

void Drug_Add(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, int numberOfTimes, int numberOfPer);
void Drug_Delete(int drugID);
void Drug_Update(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, int numberOfTimes, int numberOfPer);
void Drug_GetMinTime(void);

#endif

