/*
*********************************************************************************************************
*	                                  
*	ģ������ : ���ȵ���app�������
*	�ļ����� : App_LightDlg.c
*	��    �� : V1.0
*	˵    �� : ���ȵ��ڽ���
*            ͨ����������������
*              
*		�汾��   ����         ����      
*		V3.8    2018-1-9      Ares 	         
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "tftlcd.h"
#include "MyGUI.h"
#include "timer.h"
#include "ucos_msg.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x0E)
#define ID_BUTTON_0    (GUI_ID_USER + 0x11)
#define ID_SLIDER_0    (GUI_ID_USER + 0x12)

unsigned int LightVal;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "LightWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 218, 254, 588, 52, 0, 0x0, 0 },
};

static void InitLightDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
	SLIDER_SetRange(hItem, 0, 1000); 
	SLIDER_SetValue(hItem,LightVal);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cbButtonReturn);
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
//  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId)
	{
  case WM_INIT_DIALOG:
		InitLightDlg(pMsg);
    break;
	case WM_PAINT:
		GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  565,  	   	   /* ���½�Y λ�� */
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
			case ID_SLIDER_0: // Notifications sent by 'Slider'
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
							LightVal = SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0));	//��ȡSLIDER�ؼ���ֵ
							TIM_SetTIM3Compare2(LightVal);	                                        //����ռ�ձ�
						break;
					case WM_NOTIFICATION_VALUE_CHANGED:
							
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


void App_LightDlg( WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
 
}
