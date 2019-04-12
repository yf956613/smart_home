/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������app�������
*	�ļ����� : App_AirQualityDlg.c
*	��    �� : V3.8
*	˵    �� : ������������
*            ��ʾ��������
*            MQ135�����ѹ
*            ͨ�������б�ѡ��Ҫ�򿪻�رյĵƣ���ɫͼ�꿪�ƣ���ɫͼ��ص�
*              
*	
*		�汾��   ����         ����          ˵��
*		V3.8    2018-12-4    Ares 	       �װ�   
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "MyGUI.h"
#include "stdio.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_BUTTON_0    (GUI_ID_USER + 0x03)
#define ID_TEXT_2    (GUI_ID_USER + 0x04)



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "AirWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "AirText", ID_TEXT_0, 314, 46, 403, 49, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "AirVolText", ID_TEXT_1, 314, 116, 403, 49, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 56, 220, 904, 325, 0, 0x0, 0 },
};

extern float g_Air_test;
char g_cAir_buf[40];

/* ��ʼ���������� app�Ի��� */
static void InitAirQualityDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //������ʱ��
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	//
	// Initialization of 'AirText'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "����������");
	//
	// Initialization of 'AirVolText'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "MQ135�����ѹ��");
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	//
	// Initialization of 'Text'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "����MQ-135������������������ͨ��ADC�ɼ�MQ-135\n"
											"�����ģ���ѹ��ת��Ϊ���ֵ�ѹ����ȫ����Ⱦ��״̬\n"
											"�£������ѹֵΪ0~0.3V,��ѹԽ�󣬿�����������Խ��\n"
											"����Ƹ��ݵ�ѹ�ѿ�����������Ϊ�����ȼ����ֱ����ţ�\n"
											"���ã���ȣ��жȣ��ضȣ�����;��������������Ӱ�켫\n"
											"����Ҫ�����ܼҾӱ�����Լ�����ڿ�������~~~~~~~~~~");
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	char buf[50];
	static float air = 0;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
		InitAirQualityDlg(pMsg);
    break;
		case WM_PAINT:  //���Ʊ�����ɫ
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  565,  	   	   /* ���½�Y λ�� */
							  GUI_ORANGE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
		break;
	case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetText(hItem, g_cAir_buf);
	
			if(air != g_Air_test)
			{
				air = g_Air_test;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				sprintf(buf,"MQ135�����ѹ: %f V",g_Air_test);
				TEXT_SetText(hItem, buf);
			}
	
			WM_RestartTimer(pMsg->Data.v, 1000);  //������ʱ��
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
						GUI_EndDialog(pMsg->hWin, 0);  //���أ��رնԻ��� 
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

void App_AirQualityDlg(WM_HWIN hWin) 
{

  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}


