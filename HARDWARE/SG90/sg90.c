#include "sg90.h"

void SG90_Init(u32 arr, u32 psc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM14, ENABLE );
    
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource7, GPIO_AF_TIM14 );  //GPIOA7复用为定时器14
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init( GPIOA, &GPIO_InitStruct );
    
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit( TIM14, &TIM_TimeBaseInitStruct );
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
    TIM_OC1Init( TIM14, &TIM_OCInitStruct );
    
    TIM_OC1PreloadConfig( TIM14, TIM_OCPreload_Enable );  //使能TIM14在CCR1上的预装载寄存器
    TIM_ARRPreloadConfig( TIM14,ENABLE );//ARPE使能
    TIM_Cmd( TIM14, ENABLE );  //使能TIM14
    
    SG90_SetAngle( 0 );
}

void SG90_SetAngle( float angle )
{
    int servo_temp;
    
    if ( angle > 180 )
        angle = 180;
    else if ( angle < 0 )
        angle = 0;
    
    servo_temp = angle * 4000 / 180 + 1000;
    TIM_SetCompare1(TIM14,servo_temp);
}


