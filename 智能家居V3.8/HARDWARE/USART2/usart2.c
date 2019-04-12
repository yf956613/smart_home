#include "usart2.h"

UART_HandleTypeDef UART2_Handler;                         //UART���


//����2�жϷ������  
void USART2_IRQHandler(void)
{
	  u8 res;
	  if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  res = UART2_Handler.Instance->DR;		
	  }
		HAL_UART_IRQHandler(&UART2_Handler);
}

//��ʼ��IO,����3
//bound:������
void uart2_init(u32 bound)
{	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			              //ʹ��GPIOBʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			              //ʹ��USART1ʱ��

	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					      //USART3
	UART2_Handler.Init.BaudRate=bound;				      //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	      //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		      //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		      //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					      //HAL_UART_Init()��ʹ��UART2	

	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			  //PA2��PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			              //����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //����
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	              //����ΪUSART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	            	  //��ʼ��PA2,��PA3
	
	__HAL_UART_ENABLE_IT(&UART2_Handler,UART_IT_RXNE);    //���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);				      //ʹ��USART1�ж�ͨ��
	HAL_NVIC_SetPriority(USART2_IRQn,3,1);			      //��ռ���ȼ�2�������ȼ�3  
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


