/******************************************************************************************
**�ļ�����MyGUI.h
**����  : ���ڶ�app��ͼ�꣬app�������������õĻص����������������ļ��ȵȵ�����
**����  ��Ares Xu
**Date  : 2018-11-1
**Mail  : 1741417385@qq.com 
******************************************************************************************/

#ifndef _MYGUI_H_
#define  _MYGUI_H_
#include "GUIDEMO.h"
#include "sys.h"

#define MSG_UPDATECURTAIN     (GUI_ID_USER + 0x12)               //�Զ���emwin�ص���Ϣ
#define MSG_UPDATEDOOR        (GUI_ID_USER + 0x13)               //�Զ���emwin�ص���Ϣ
#define MSG_UPDATEWIN         (GUI_ID_USER + 0x14)               //�Զ���emwin�ص���Ϣ

#define SOCKET_NUM   20    //����������
#define LAMP_NUM   20    //�Ƶ�����

#define LAMP_SW        PAout(6)
#define SOCKET_SW      PAout(11)
#define ALARM_SW       PAin(12)

#define ENABLE_ALA      HAL_NVIC_EnableIRQ(EXTI15_10_IRQn)     //ʧ���ж���   �رձ��� 
#define DISABLE_ALA     HAL_NVIC_DisableIRQ(EXTI15_10_IRQn)    //ʧ���ж���   �򿪱���

#define DOOR_ON           "DOOR ON"
#define DOOR_OFF          "DOOR OFF"

#define WIN_ON            "WIN ON"
#define WIN_OFF           "WIN OFF"


#define CURTAIN_ON         "CURTAIN ON"
#define CURTAIN_OFF        "CURTAIN OFF"


#define ALAON   "ALAON"     //������
#define ALAOFF  "ALAOFF"    //������

extern unsigned char WinAutoFlag;     //���Զ����Ʊ�־λ
extern unsigned char WinFLag;         //�������Ʊ�־λ

extern unsigned char CurtainFLag;
extern unsigned char AutoCurtainFlag;  //�����Զ����Ʊ�־λ

extern WM_HWIN DoorWinCurtainAppHwin;

/*****************************************************************************************************************
*********************************App�����������ⲿ����************************************************************
******************************************************************************************************************/
extern void CreateStatusDlg(void);    //����������
extern  void AppSysInfoDlg(WM_HWIN hwin);  //ϵͳ��ϢApp
extern void App_LampCtrlDlg(WM_HWIN hWin);
extern void App_HomeApplianceDlg(WM_HWIN hWin); //�ҵ����app
extern void App_ClockSetDlg(WM_HWIN hWin);   //ʱ������app
extern void App_ToBeDeveloped( WM_HWIN hWin) ;  //������app
extern void App_SocketCtrlDlg(WM_HWIN hWin) ;  //����app
extern void App_TempHumiDlg(WM_HWIN hWin);      //��ʪ��app
extern void App_DoorWinCurtainDlg( WM_HWIN hWin) ;      //�Ŵ�app����
extern void App_AirConditioningDlg(WM_HWIN hWin); //�յ�app
extern void App_LightDlg( WM_HWIN hWin);    //�ƹ����app
extern void App_AlarmDlg(WM_HWIN hWin);    //��������app
extern void App_DeveloperDlg(WM_HWIN hWin);  //������app
extern void App_AirQualityDlg(WM_HWIN hWin);   //��������app
extern void App_FireAndAntivirusDlg(WM_HWIN hWin);   //��ȼ��/����
extern void App_SettingsDlg(WM_HWIN hWin);           //����app����
extern void App_WifiStatusDlg( WM_HWIN hWin);        //wifi�豸����״̬
extern void CreateTempHumiAirDlg(void);    //��ʾ��ʪ��/��������/��ȼ������ ������������
extern void App_HomeApplianceChildDlg(WM_HWIN hWin);
extern void App_HomeApplianceTimerChildDlg(WM_HWIN hWin);


/**************************************************************************************************************
**************************����app�ر�ͼ��Ļص������Ͷ�ʱ����ͼ��Ļص�����************************************
***************************************************************************************************************/
extern void _cbButtonReturn(WM_MESSAGE * pMsg); /* ����app�Ĺرհ�ť�Ļص����� */
extern void _cbTimerButton(WM_MESSAGE * pMsg);  //���ж�ʱ����ť�Ļص�����

/************************************************************************************************************** 
*********************************************APP��ͼ���ⲿ����************************************************* 
***************************************************************************************************************/
extern GUI_CONST_STORAGE GUI_BITMAP bmClock;   //ʱ������appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmSystemInfo; //ϵͳ��Ϣappͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmSettings; //����Appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmLamp;     //�ƹ����Appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmSocket;   //���ܲ���Appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmDoorWindows;  //�����Ŵ�Appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmAirQuality;  //��������Appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmTempHumi;    //��ʪ��appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmWanring;    //��������appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmElectricAppliance; //�ҵ����appͼ��

extern GUI_CONST_STORAGE GUI_BITMAP bmDeveloper;  //������appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmLight;     //�ƹ����appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmGasFire;        //��ȼ������appͼ��
extern GUI_CONST_STORAGE GUI_BITMAP bmWIFI;       //WIFI  appͼ��

extern GUI_CONST_STORAGE GUI_BITMAP bmTimerManager; //��ʱ�����ӹ��ܴ���ͼ��
//�ҵ����app���ͼ��
//extern GUI_CONST_STORAGE GUI_BITMAP bmAirConditioner72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmRefrigerator72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmTelevision72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmWasher72px;


extern GUI_CONST_STORAGE GUI_BITMAP bmON;
extern GUI_CONST_STORAGE GUI_BITMAP bmOFF;

// /* �ƹ����app��ͼ�� */
//extern GUI_CONST_STORAGE GUI_BITMAP bmOffLamp;
//extern GUI_CONST_STORAGE GUI_BITMAP bmOnLamp;

 /* ����App���ذ�ťͼ�� */
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;

extern GUI_CONST_STORAGE GUI_BITMAP _bmAlarm_16x16;  //����������ͼ��
extern GUI_CONST_STORAGE GUI_BITMAP _bmBatteryEmpty_27x14;   //���ͼ��
extern GUI_CONST_STORAGE GUI_BITMAP * _apbmCharge[];
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_clock48x48;

/*���������С*/
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32;


#endif


