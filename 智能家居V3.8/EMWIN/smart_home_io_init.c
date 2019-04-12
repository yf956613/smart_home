#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "includes.h"
#include "led.h"
#include "ucos_msg.h"


//#define ALA_SET  "AlA SET"

OS_FLAG_GRP  AlaFlagGrp;

//���Ƶ�IO��ʼ��
void Lamp_GPIO_Init()
{
	//��ʼ��PA6Ϊ���.��ʹ��ʱ��	    

    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6; //PA6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);	//PA6��0
}

//���Ʋ���IO�ڳ�ʼ��
void Socket_GPIO_Init(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_11; //PA11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);	//PA11��1 
}

//�������ܿ��س�ʼ��
void ALarmSw_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��

	GPIO_Initure.Pin=GPIO_PIN_12;            //PA12
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���  ��-->��  
	GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
//	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             //ʹ���ж���12
	
//    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//PB1��1 
}

//���ܼҾ��õ���IO�ڳ�ʼ��
void SmartHomeIOInit(void)
{
	Lamp_GPIO_Init();     //���Ƶ�IO��ʼ��
	Socket_GPIO_Init();   //���Ʋ���IO�ڳ�ʼ��
	ALarmSw_GPIO_Init();  //�������ܿ��س�ʼ��
}

unsigned char GizwitsAlaFlag = 0;

unsigned char CountAla = 0;
//����������
void alarm_set(void)
{
	OS_ERR err;
	OS_MSG_SIZE	 msg_size;
	CPU_TS	     ts;
	
	LED1 = !LED1;
	GizwitsAlaFlag = 1;
	OSFlagPost((OS_FLAG_GRP*)&AlaFlagGrp,  //���ͱ�����־
						(OS_FLAGS	  )ALA_SET,
						(OS_OPT	  )OS_OPT_POST_FLAG_SET,
						(OS_ERR*	  )&err);

}


void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);//�����жϴ����ú���
	OSIntExit();
}



//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_12:
           alarm_set();
            break;
    }
}



