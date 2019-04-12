/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��ʪ��app���
*	�ļ����� : App_TempHumiDlg.c
*	��    �� : V1.0
*	˵    �� : ��ʪ�Ƚ���
*              ��ʾ��ʪ��
*              ��ʾ��ʾ��Ϣ
*							 .......................
*              
*	
*		�汾��   ����         ����          ˵��
*		V3.8    2018-12-8   Ares  	         
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/
#include "DIALOG.h"
#include "MyGUI.h"
#include "dht11.h"

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_TEXT_0    (GUI_ID_USER + 0x02)
#define ID_TEXT_1    (GUI_ID_USER + 0x03)
#define ID_TEXT_2    (GUI_ID_USER + 0x0C)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, "TempHumiWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "tempText", ID_TEXT_0, 355, 38, 321, 38, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "humiText", ID_TEXT_1, 356, 99, 321, 38, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 102, 282, 829, 242, 0, 0x0, 0 },
};

/* ��ʼ����ʪ�� app�Ի��� */
static void InitTempHumiDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 100, 0);	  //������ʱ��
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "��ǰ�¶ȣ� ");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "��ǰʪ�ȣ�");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "���о��������������ʺ��¶�Ӧ���������´� 18�� ʱ��\n"
											"���ʪ��Ӧ������30��40%�����´� 25�� ʱ�����ʪ��Ӧ\n"
											"������ 40��50% Ϊ���ˡ�����ʪ��Ҳ���˹��߻���ͣ���\n"
											"��ʪ�ȹ��ߣ�����ɢ�ȾͱȽ����ѡ�����ʪ�ȹ��ͣ�����\n"
											"����˵ĺ��������ɬ���ܡ�ͨ��DHT11��ʪ�ȴ����� \n"
											"������ڻ�����ʪ�Ȳ���ʾ��������������������������\n"
											);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
}	

static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	char buf[30];
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
	{
		
  case WM_INIT_DIALOG:
		InitTempHumiDlg(pMsg);
    break;
	case WM_PAINT:
		GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
						  0,			   /* ���Ͻ�Y λ�� */
						  1024,	   	       /* ���½�X λ�� */
						   600,  	   	   /* ���½�Y λ�� */
						   GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
						   GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
	break;
	case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			sprintf(buf,"��ǰ�¶ȣ�%d ��",temperature);
			TEXT_SetText(hItem, buf);
	
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			sprintf(buf,"��ǰʪ�ȣ�%d %%RH",humitiy);
			TEXT_SetText(hItem, buf);
	
			WM_RestartTimer(pMsg->Data.v, 100);  //������ʱ��
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


void App_TempHumiDlg(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

