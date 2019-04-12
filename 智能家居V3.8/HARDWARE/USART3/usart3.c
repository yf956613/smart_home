#include "usart3.h"
#include "gizwits_product.h"

UART_HandleTypeDef UART3_Handler;                         //UART���


//����3�жϷ������   ���ڻ��������ݵ��շ�
void USART3_IRQHandler(void)
{
	  u8 res;
	  if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  res = UART3_Handler.Instance->DR;
		  gizPutData(&res, 1);//����д�뵽������
		
	  }
		HAL_UART_IRQHandler(&UART3_Handler);
}

//��ʼ��IO,����3
//bound:������
void uart3_init(u32 bound)
{	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE();			              //ʹ��GPIOBʱ��
	__HAL_RCC_USART3_CLK_ENABLE();			              //ʹ��USART1ʱ��

	//UART ��ʼ������
	UART3_Handler.Instance=USART3;					      //USART3
	UART3_Handler.Init.BaudRate=bound;				      //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	      //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		      //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		      //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					      //HAL_UART_Init()��ʹ��UART3	

	GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;			  //PB10��PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			              //����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //����
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	              //����ΪUSART3
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	            	  //��ʼ��PB10,��PB11
	
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);    //���������ж�
	HAL_NVIC_EnableIRQ(USART3_IRQn);				      //ʹ��USART1�ж�ͨ��
	HAL_NVIC_SetPriority(USART3_IRQn,2,3);			      //��ռ���ȼ�2�������ȼ�3


  
}


