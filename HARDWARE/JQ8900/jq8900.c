#include "jq8900.h"
#include "delay.h"

/* JQ8900初始化 */
void JQ8900_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;			//下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//初始化GPIOC
}
///
//函	  数：JQ8900_SendData(u8 addr)
//功	  能：语音模块一线串口
//输入参数: addr要发送的0x数
//输出参数: void
//说	  明：
//
void JQ8900_SendData(u8 addr)//发送函数。
{
    u8 i;
 
     /*发送时关掉中断，防止中断影响时序  */
    SDA = 1; /*开始拉高*/
    delay_us ( 1000 );
    SDA = 0; /*开始引导码*/
    delay_us ( 3200 );/*此处延时最少要大于2ms*/
    for ( i = 0; i < 8; i++ ) /*总共8位数据  */
    {
        SDA = 1;
        if ( addr & 0x01 ) /*3:1表示数据位1,每个位用两个脉冲表示  */
        {
            delay_us ( 600 );
            SDA = 0;
            delay_us ( 210 );
        }
        else              /*1：3表示数据位0 ,每个位用两个脉冲表示  */
        {
            delay_us ( 210 );
            SDA = 0;
            delay_us ( 600 );
        }
        addr >>= 1;
    }
    SDA = 1;
    
		//恢复中断
}

/* 设置音量 */
//volume取值：10、20、30...10的倍数
void JQ8900_SetVolume(uint8_t volume)
{
	uint8_t temp = 0;
	temp = volume / 10;
	
	JQ8900_SendData(0x0a);    //清空数字
    JQ8900_SendData(temp);    //分两次发送音量值
    JQ8900_SendData(0x00);
    JQ8900_SendData(0x0c);    //设置音量
    delay_ms(2000);    //延时
}

/* 停止播放 */
void JQ8900_Stop(void)
{
	JQ8900_SendData(0x0a);
	JQ8900_SendData(0x13);
	delay_ms(2000);
}

/* 开始播放 */
void JQ8900_Start(void)
{
    //设置音量为20
    JQ8900_SetVolume(10);
	
	//播放音频00001.mp3
	JQ8900_SendData(0x0a);
	JQ8900_SendData(0x01);
    JQ8900_SendData(0x0b);//选曲确认
    delay_ms(2000);
}



