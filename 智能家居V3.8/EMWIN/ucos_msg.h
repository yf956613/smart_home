#ifndef _UCOS_MSG_H_
#define _UCOS_MSG_H_

#include "includes.h"


#define AIR_DEBUG             1u

#define AIR_DATA_MSG_LEN      8

#define RX_TX__DATA_MSG_LEN   1u

extern OS_TCB RX_TX_TaskTCB;
extern OS_TCB  AlarmTaskTCB;

extern OS_FLAG_GRP  AlaFlagGrp;                 //�����¼���־��     
//extern OS_FLAG_GRP  WinAndCurtainAutoFlagGrp;    //�Ŵ��Զ������¼���־��

//extern OS_Q  AIR_DATA_MSG;  //�����������������Ϣ����
////extern OS_Q  RX_TX_DATA_MSG;  //���ͽ���������Ϣ����

extern OS_FLAG_GRP	ScreenLightEventFlags;		//��ĻϨ�������¼���־λ

#define SCREEN_OFF_FLAG         (1 << 0)      //Ϩ���¼���־λ

#define ALA_SET                 (1 << 0)    //���������¼���־λ   ����������
#define ALA_OFF                 (1 << 1)   //�����ر��¼���־λ    ����/��ȼ��������

//#define HUMI_TEMP_HIGHT         (1 << 0)   //�¶Ⱥ�ʪ�ȹ����¼���־λ    �����Զ�������
//#define AIR_QUALITY_BAD         (1 << 1)   //�����������¼���־λ        �����Զ�������
//#define GAS_SMOKE_EXIST         (1 << 2)   //��ȼ��/��������¼���־λ   �����Զ�������

//extern OS_SEM DOOR_WIN_CURTAIN_SEM;

#endif

