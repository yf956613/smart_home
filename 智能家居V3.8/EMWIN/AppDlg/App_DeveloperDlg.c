/*
*********************************************************************************************************
*	                                  
*	ģ������ : ������app�������
*	�ļ����� : App_DeveloperDlg.c
*	��    �� : V1.0
*	˵    �� : �����߽���
*            ��ʾ��Ҫ��ϵͳ�����ܹ���˵��
*              
*		�汾��   ����         ����      
*		V3.8    2018-1-9      Ares 	         
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/


#include "DIALOG.h"
#include "MyGUI.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_0    (GUI_ID_USER + 0x05)
#define ID_TEXT_0    (GUI_ID_USER + 0x06)



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "DeveloperWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 43, 108, 925, 402, 0, 0x0, 0 },
};

/* ��ʼ�������� app�Ի��� */
static void InitDeveloperDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "����ƽ����������������Ƕ��ʽ��������Ӧ��\n"
											"�ڼ�ͥסլ����Զ�̼�⡢���Ƶ����ܼҾ�ϵͳ��\n"
											"RTOS����uC/OS-IIIǶ��ʽʵʱ����ϵͳ��GUI����\n"
											"emwinͼ�ν��棬��ARM Cortex-M4�ں˼ܹ�STM32F4\n"
											"ϵ�е�Ƭ����Ϊ���ء�����DHT11����¶ȡ�ʪ�ȣ�\n"
											"MQ2����ȼ��/��������MQ135��������������app\n"
											"Զ���Լ�ͨ��GUI���Ƶƹ⡢�������Ŵ��������������ȡ�\n"
											 );

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cbButtonReturn);
}
/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
//  WM_HWIN hItem;
  int     NCode;
  int     Id;
 
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitDeveloperDlg(pMsg);
			break;
		case WM_PAINT:
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
									0,			   /* ���Ͻ�Y λ�� */
									1024,	   	       /* ���½�X λ�� */
									600,  	   	   /* ���½�Y λ�� */
									GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
									GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
				break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_0: // Notifications sent by 'ReturnButton'
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							GUI_EndDialog(pMsg->hWin, 0);  //�رնԻ���
							break;
					
					}
					break;
			}
			break;
	 
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


void App_DeveloperDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

