#include "drug.h"
#include <stdio.h>

DrugInfo Drug[5];		//本地存储，最多5种药物
int DrugNum = 0;		//当前药物种类数量
int Current_Index = 0;	//最先服用药物的索引

/* 添加药物 */
void Drug_Add(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, 
			int numberOfTimes, int numberOfPer)
{
	int i = 0, j = 0;
	
	Drug[DrugNum].drugID = drugID;
	
	while(*drugName != '\0')
	{
		Drug[DrugNum].drugName[i++] = *drugName;
		drugName++;
	}
	
	while(*nextTakeTime != '\0')
	{
		Drug[DrugNum].nextTakeTime[j++] = *nextTakeTime;
		nextTakeTime++;
	}
	
	Drug[DrugNum].nextDate = nextDate;
	Drug[DrugNum].drugQuantity = drugQuantity;
	Drug[DrugNum].storeNumber = storeNumber;
	Drug[DrugNum].numberOfTimes = numberOfTimes;
	Drug[DrugNum].numberOfPer = numberOfPer;
	
	DrugNum++;
}

/* 删除药物 */
void Drug_Delete(int drugID)
{
	int i = 0, index = 0;
	
	for(i = 0; i < DrugNum; i++)
	{
		if(Drug[i].drugID == drugID)
		{
			index = i;
			break;
		}
	}
	
	for(i = index; i < DrugNum; i++)
	{
		Drug[i] = Drug[i+1];
	}
	
	DrugNum--;
}

/* 修改药物信息 */
void Drug_Update(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, 
				int numberOfTimes, int numberOfPer)
{
	int i = 0, j = 0, k = 0, index = 0;
	
	for(i = 0; i < DrugNum; i++)	//通过药物ID找到数组下标
	{
		if(Drug[i].drugID == drugID)
		{
			index = i;
			break;
		}
	}
	
	while(*drugName != '\0')
	{
		Drug[index].drugName[j++] = *drugName;
		drugName++;
	}
	
	while(*nextTakeTime != '\0')
	{
		Drug[index].nextTakeTime[k++] = *nextTakeTime;
		nextTakeTime++;
	}
	
	Drug[index].nextDate = nextDate;
	Drug[index].drugQuantity = drugQuantity;
	Drug[index].storeNumber = storeNumber;
	Drug[index].numberOfTimes = numberOfTimes;
	Drug[index].numberOfPer = numberOfPer;
}

/* 比较时间字符串大小 */
static int compare_time(const char *time1, const char *time2)
{
	int h1, m1, s1;
	int h2, m2, s2;
	
	sscanf(time1, "%d:%d:%d", &h1, &m1, &s1);
	sscanf(time2, "%d:%d:%d", &h2, &m2, &s2);
	
	if(h1 != h2)
		return h1-h2;
	
	if(m1 != m2)
		return m1-m2;
	
	return s1-s2;
}

/* 获得最早时间的药物索引 */
void Drug_GetMinTime(void)
{
	int i = 0, result = 0;
	
	for(i = 0; i < DrugNum; i++)
	{
		if(Drug[i].nextDate < Drug[Current_Index].nextDate) //如果是今天的时间与明天的时间比较，选择今天的时间
		{
			Current_Index = i;
		}
		else if(Drug[i].nextDate == Drug[Current_Index].nextDate)	//如果两者都是同一日期，则比较时间字符串
		{
			result = compare_time(Drug[i].nextTakeTime, Drug[Current_Index].nextTakeTime);
		
			if(result < 0)	//如果比显示的时间早
			{
				Current_Index = i;
			}
		}
	}
}


