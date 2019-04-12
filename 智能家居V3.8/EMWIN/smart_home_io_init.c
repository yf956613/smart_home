#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "includes.h"
#include "led.h"
#include "ucos_msg.h"


//#define ALA_SET  "AlA SET"

OS_FLAG_GRP  AlaFlagGrp;

//控制灯IO初始化
void Lamp_GPIO_Init()
{
	//初始化PA6为输出.并使能时钟	    

    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6; //PA6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);	//PA6置0
}

//控制插座IO口初始化
void Socket_GPIO_Init(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_11; //PA11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);	//PA11置1 
}

//报警功能开关初始化
void ALarmSw_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟

	GPIO_Initure.Pin=GPIO_PIN_12;            //PA12
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发  低-->高  
	GPIO_Initure.Pull=GPIO_PULLDOWN;          //下拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);       //抢占优先级为2，子优先级为0
//	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             //使能中断线12
	
//    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//PB1置1 
}

//智能家居用到的IO口初始化
void SmartHomeIOInit(void)
{
	Lamp_GPIO_Init();     //控制灯IO初始化
	Socket_GPIO_Init();   //控制插座IO口初始化
	ALarmSw_GPIO_Init();  //报警功能开关初始化
}

unsigned char GizwitsAlaFlag = 0;

unsigned char CountAla = 0;
//报警处理函数
void alarm_set(void)
{
	OS_ERR err;
	OS_MSG_SIZE	 msg_size;
	CPU_TS	     ts;
	
	LED1 = !LED1;
	GizwitsAlaFlag = 1;
	OSFlagPost((OS_FLAG_GRP*)&AlaFlagGrp,  //发送报警标志
						(OS_FLAGS	  )ALA_SET,
						(OS_OPT	  )OS_OPT_POST_FLAG_SET,
						(OS_ERR*	  )&err);

}


void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);//调用中断处理公用函数
	OSIntExit();
}



//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_12:
           alarm_set();
            break;
    }
}



