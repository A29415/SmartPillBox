#include "drug.h"
#include <stdio.h>

DrugInfo Drug[5];		//���ش洢�����5��ҩ��
int DrugNum = 0;		//��ǰҩ����������
int Current_Index = 0;	//���ȷ���ҩ�������

/* ���ҩ�� */
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

/* ɾ��ҩ�� */
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

/* �޸�ҩ����Ϣ */
void Drug_Update(int drugID, char *drugName, char *nextTakeTime, int  nextDate, int drugQuantity, int storeNumber, 
				int numberOfTimes, int numberOfPer)
{
	int i = 0, j = 0, k = 0, index = 0;
	
	for(i = 0; i < DrugNum; i++)	//ͨ��ҩ��ID�ҵ������±�
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

/* �Ƚ�ʱ���ַ�����С */
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

/* �������ʱ���ҩ������ */
void Drug_GetMinTime(void)
{
	int i = 0, result = 0;
	
	for(i = 0; i < DrugNum; i++)
	{
		if(Drug[i].nextDate < Drug[Current_Index].nextDate) //����ǽ����ʱ���������ʱ��Ƚϣ�ѡ������ʱ��
		{
			Current_Index = i;
		}
		else if(Drug[i].nextDate == Drug[Current_Index].nextDate)	//������߶���ͬһ���ڣ���Ƚ�ʱ���ַ���
		{
			result = compare_time(Drug[i].nextTakeTime, Drug[Current_Index].nextTakeTime);
		
			if(result < 0)	//�������ʾ��ʱ����
			{
				Current_Index = i;
			}
		}
	}
}


