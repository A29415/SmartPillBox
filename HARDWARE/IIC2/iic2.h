#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
   	   		   
//IO方向设置
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=0<<13*2;}	//PB11输入模式
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(13*2));GPIOB->MODER|=1<<13*2;} //PB11输出模式
//IO操作函数	 
#define IIC2_SCL    PBout(12) //SCL
#define IIC2_SDA    PBout(13) //SDA	 
#define READ2_SDA   PBin(13)  //输入SDA 

//IIC所有操作函数
void IIC2_Init(void);                //初始化IIC的IO口				 
void IIC2_Start(void);				//发送IIC开始信号
void IIC2_Stop(void);	  			//发送IIC停止信号
void IIC2_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC等待ACK信号
void IIC2_Ack(void);					//IIC发送ACK信号
void IIC2_NAck(void);				//IIC不发送ACK信号

#endif
















