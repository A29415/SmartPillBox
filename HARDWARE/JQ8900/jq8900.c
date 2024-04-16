#include "jq8900.h"
#include "delay.h"

/* JQ8900��ʼ�� */
void JQ8900_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;			//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��ʼ��GPIOC
}
///
//��	  ����JQ8900_SendData(u8 addr)
//��	  �ܣ�����ģ��һ�ߴ���
//�������: addrҪ���͵�0x��
//�������: void
//˵	  ����
//
void JQ8900_SendData(u8 addr)//���ͺ�����
{
    u8 i;
 
     /*����ʱ�ص��жϣ���ֹ�ж�Ӱ��ʱ��  */
    SDA = 1; /*��ʼ����*/
    delay_us ( 1000 );
    SDA = 0; /*��ʼ������*/
    delay_us ( 3200 );/*�˴���ʱ����Ҫ����2ms*/
    for ( i = 0; i < 8; i++ ) /*�ܹ�8λ����  */
    {
        SDA = 1;
        if ( addr & 0x01 ) /*3:1��ʾ����λ1,ÿ��λ�����������ʾ  */
        {
            delay_us ( 600 );
            SDA = 0;
            delay_us ( 210 );
        }
        else              /*1��3��ʾ����λ0 ,ÿ��λ�����������ʾ  */
        {
            delay_us ( 210 );
            SDA = 0;
            delay_us ( 600 );
        }
        addr >>= 1;
    }
    SDA = 1;
    
		//�ָ��ж�
}

/* �������� */
//volumeȡֵ��10��20��30...10�ı���
void JQ8900_SetVolume(uint8_t volume)
{
	uint8_t temp = 0;
	temp = volume / 10;
	
	JQ8900_SendData(0x0a);    //�������
    JQ8900_SendData(temp);    //�����η�������ֵ
    JQ8900_SendData(0x00);
    JQ8900_SendData(0x0c);    //��������
    delay_ms(2000);    //��ʱ
}

/* ֹͣ���� */
void JQ8900_Stop(void)
{
	JQ8900_SendData(0x0a);
	JQ8900_SendData(0x13);
	delay_ms(2000);
}

/* ��ʼ���� */
void JQ8900_Start(void)
{
    //��������Ϊ20
    JQ8900_SetVolume(10);
	
	//������Ƶ00001.mp3
	JQ8900_SendData(0x0a);
	JQ8900_SendData(0x01);
    JQ8900_SendData(0x0b);//ѡ��ȷ��
    delay_ms(2000);
}



