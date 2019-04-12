#include "usart2.h"

UART_HandleTypeDef UART2_Handler;                         //UART句柄


//串口2中断服务程序  
void USART2_IRQHandler(void)
{
	  u8 res;
	  if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  res = UART2_Handler.Instance->DR;		
	  }
		HAL_UART_IRQHandler(&UART2_Handler);
}

//初始化IO,串口3
//bound:波特率
void uart2_init(u32 bound)
{	
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			              //使能GPIOB时钟
	__HAL_RCC_USART2_CLK_ENABLE();			              //使能USART1时钟

	//UART 初始化设置
	UART2_Handler.Instance=USART2;					      //USART3
	UART2_Handler.Init.BaudRate=bound;				      //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	      //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		      //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		      //收发模式
	HAL_UART_Init(&UART2_Handler);					      //HAL_UART_Init()会使能UART2	

	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			  //PA2、PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			              //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //高速
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	              //复用为USART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	            	  //初始化PA2,和PA3
	
	__HAL_UART_ENABLE_IT(&UART2_Handler,UART_IT_RXNE);    //开启接收中断
	HAL_NVIC_EnableIRQ(USART2_IRQn);				      //使能USART1中断通道
	HAL_NVIC_SetPriority(USART2_IRQn,3,1);			      //抢占优先级2，子优先级3  
}

void usart2_send_str(unsigned char *str)
{
	while(*str)
	{
		HAL_UART_Transmit(&UART2_Handler, str,1,0xfff);
		str++;
	}
}

void usart2_send_byte(unsigned char data)
{
	HAL_UART_Transmit(&UART2_Handler,&data,1,0xfff);
}


