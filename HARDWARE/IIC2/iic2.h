#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
   	   		   
//IO��������
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=0<<13*2;}	//PB11����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=1<<13*2;} //PB11���ģʽ
//IO��������	 
#define IIC2_SCL    PBout(12) //SCL
#define IIC2_SDA    PBout(13) //SDA	 
#define READ2_SDA   PBin(13)  //����SDA 

//IIC���в�������
void IIC2_Init(void);                //��ʼ��IIC��IO��				 
void IIC2_Start(void);				//����IIC��ʼ�ź�
void IIC2_Stop(void);	  			//����IICֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC����ACK�ź�
void IIC2_NAck(void);				//IIC������ACK�ź�

#endif
















