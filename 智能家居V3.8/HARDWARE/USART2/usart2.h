#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����3��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2016/6/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	

//UART���
extern UART_HandleTypeDef UART2_Handler; 

void uart2_init(u32 bound);			//����2��ʼ�� 
void usart2_send_str(unsigned char *str);
void usart2_send_byte(unsigned char data);

#endif



