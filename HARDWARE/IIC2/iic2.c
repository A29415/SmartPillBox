#include "iic2.h"
#include "delay.h"

//初始化IIC
void IIC2_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    //GPIOB10,B11初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
    IIC2_SCL=1;
    IIC2_SDA=1;
}
//产生IIC起始信号
void IIC2_Start(void)
{
    SDA2_OUT();     //sda线输出
    IIC2_SDA=1;
    IIC2_SCL=1;
    delay_us(4);
    IIC2_SDA=0;//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC2_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC2_Stop(void)
{
    SDA2_OUT();//sda线输出
    IIC2_SCL=0;
    IIC2_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC2_SCL=1;
    IIC2_SDA=1;//发送I2C总线结束信号
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC2_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA2_IN();      //SDA设置为输入
    IIC2_SDA=1;
    delay_us(1);
    IIC2_SCL=1;
    delay_us(1);
    while(READ2_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC2_Stop();
            return 1;
        }
    }
    IIC2_SCL=0;//时钟输出0
    return 0;
}
//产生ACK应答
void IIC2_Ack(void)
{
    IIC2_SCL=0;
    SDA2_OUT();
    IIC2_SDA=0;
    delay_us(2);
    IIC2_SCL=1;
    delay_us(2);
    IIC2_SCL=0;
}
//不产生ACK应答
void IIC2_NAck(void)
{
    IIC2_SCL=0;
    SDA2_OUT();
    IIC2_SDA=1;
    delay_us(2);
    IIC2_SCL=1;
    delay_us(2);
    IIC2_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC2_Send_Byte(u8 txd)
{
    u8 t;
    SDA2_OUT();
    IIC2_SCL=0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        IIC2_SDA=(txd&0x80)>>7;
        txd<<=1;
        delay_us(2);   //对TEA5767这三个延时都是必须的
        IIC2_SCL=1;
        delay_us(2);
        IIC2_SCL=0;
        delay_us(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC2_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA2_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC2_SCL=0;
        delay_us(2);
        IIC2_SCL=1;
        receive<<=1;
        if(READ2_SDA)receive++;
        delay_us(1);
    }
    if (!ack)
        IIC2_NAck();//发送nACK
    else
        IIC2_Ack(); //发送ACK
    return receive;
}



























