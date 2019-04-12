#include "timer.h"
#include "led.h"
//////////////////////////定时器3用来输出PWM控制LCD背光了///////////////////////////////////////// 	 

TIM_HandleTypeDef TIM3_Handler;         //定时器3PWM句柄 
TIM_OC_InitTypeDef TIM3_CH4Handler;	    //定时器3通道4句柄
TIM_OC_InitTypeDef TIM3_CH2Handler;	    //定时器3通道2句柄

extern unsigned int LightVal;
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
    TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH2Handler.Pulse=arr/2+300;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
	  LightVal = arr/2+300;
    TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道2
	
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5;           	//PB5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	 GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PB2复用为TIM3_CH2
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


//设置TIM通道2(PB5)的占空比  PWM调节LCD的亮度
//compare:比较值
void TIM_SetTIM3Compare2(u32 compare)
{
	TIM3->CCR2=compare; 
}



extern void gizTimerMs(void);

TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 

//通用定时器2中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器2!(定时器2挂在APB1上，时钟为HCLK/2)
void TIM2_Init(u16 arr,u16 psc)
{  
	
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2更新中断：TIM_IT_UPDATE  

}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
   if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
	}
}

unsigned char Enter_IT_Flag;
//定时器2中断服务函数
void TIM2_IRQHandler(void)
{   
	 if(__HAL_TIM_GET_FLAG(&TIM2_Handler, TIM_FLAG_UPDATE) != RESET)
     {
				if(__HAL_TIM_GET_IT_SOURCE(&TIM2_Handler, TIM_IT_UPDATE) !=RESET)
				{
					 __HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);  //清除标志位
#if 0
					Enter_IT_Flag = 1;
#endif
					 gizTimerMs();//系统毫秒定时
				}
     }
     
}
