/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��ʪ�ȿ�����������ǿ�����������������
*	�ļ����� : App_LampCtrlDlg.c
*	��    �� : V1.0
*	˵    �� : ��ʪ�ȿ�������������������
*            ��ʾ��ʪ�ȣ�������������ȼ��/����ȵ�
*              
*		�汾��   ����         ����          ˵��
*		V3.8    2018-12-4    Ares 	      
*   Mail    1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "stdio.h"
#include "string.h"
#include "MyGUI.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_2    (GUI_ID_USER + 0x03)

#define MAG     3

unsigned char g_ucSwitchAirStatus = 0;
unsigned char g_ucSwitchMQ2Status = 0;
unsigned char temperature         = 23;
unsigned char humitiy             = 45;
extern unsigned short g_uAls;    //����ǿ��
float  g_GasConcentration = 10.07;
char *g_AirStatus[] = {"�� ","����","���","�ж�","�ض�","����"};
char *g_MQ2Status[] = {"�� ","΢��","���","�ж�","�ض�","����"};

float g_Air_test = 0;
extern char g_cAir_buf[24];
extern char g_cGas_buf[30];

/*
*********************************************************************************************************
*	�� �� ��: _cbDialogSysInfo
*	����˵��: �ص�����
*	��    ��: pMsg  WM_MESSAGE����ָ�����           
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbDialogSysInfo(WM_MESSAGE * pMsg) 
{
	GUI_RECT pRect;
	char buf[40];

	switch (pMsg->MsgId) 
	{
		case WM_TIMER:
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 250);
			break;

		case WM_PAINT:
			GUI_SetColor(0x007C1877);
			WM_GetClientRect(&pRect);
			GUI_AA_FillRoundedRect(pRect.x0, pRect.y0, pRect.x1, pRect.y1, 6);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(GUI_FONT_13_1);

			GUI_SetFont(&GUI_FontHZ16);//��������
			
			sprintf(buf, "�¶�: %2.1d ��",temperature);	
			GUI_DispStringAt(buf, 3, 3);			

			sprintf(buf, "ʪ��: %2.1d%% RH",humitiy);	
			GUI_DispStringAt(buf, 3, 20);

			switch(g_ucSwitchAirStatus)
			{
				case 0: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 1: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 2: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 3: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 4: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 5: sprintf(buf, "��������: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
			}
			strcpy(g_cAir_buf,buf);
			GUI_DispStringAt(buf, 3, 37);

			switch(g_ucSwitchAirStatus)
			{
				case 0: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 1: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 2: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 3: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 4: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 5: sprintf(buf, "��ȼ��/����: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
			}
			strcpy(g_cGas_buf,buf);     //���浱ǰ��ȼ��/����ȼ�
			GUI_DispStringAt(buf, 3, 54);
			
			
			sprintf(buf, "����ǿ��: %d cd",g_uAls);
			GUI_DispStringAt(buf, 3, 71);
	
			break;

		case WM_CREATE:
			/* ���ÿ�������� */
			GUI_AA_SetFactor(MAG);
			
			WM_CreateTimer(pMsg->hWin, 0, 250, 0);	         
			break;

		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CreateSysInfoDlg
*	����˵��: ϵͳ��Ϣ�Ի���
*	��    ��: ��   	
*	�� �� ֵ: ���ھ��
*********************************************************************************************************
*/
void CreateTempHumiAirDlg(void) 
{
	WM_CreateWindow(3, 
	                450, 
	                142, 
	                90, 
	                WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_MOTION_X | WM_CF_MOTION_Y,
                   _cbDialogSysInfo, 
	                0);
}

