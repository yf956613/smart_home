/*
*********************************************************************************************************
*	                                  
*	ģ������ : Wifi app���
*	�ļ����� : App_WifiStatusDlg.c
*	��    �� : V1.0
*	˵    �� : wifi app����
*              1. ��ʾ�ֻ��豸����״̬
*              2. ��ʾwifi����״̬
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


#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_1    (GUI_ID_USER + 0x02)
#define ID_BUTTON_2    (GUI_ID_USER + 0x03)
#define ID_TEXT_0    (GUI_ID_USER + 0x04)
#define ID_TEXT_1    (GUI_ID_USER + 0x05)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "WIFIWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WIFICONM2MButton", ID_BUTTON_1, 238, 266, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WIFICONROUTERButton", ID_BUTTON_2, 653, 266, 110, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 117, 197, 360, 34, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 534, 197, 360, 34, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

extern unsigned char g_ucWifiStatus;    //�ֻ��豸�Ƿ����ϻ����Ʒ�������־
extern unsigned char g_ucWifiConRouter; //wifiģ���Ƿ�����·����(��wifi�Ƿ�����) 


/* ��ʼ��wifi app�Ի��� */
static void InitWifiStatusDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "�ֻ��豸����״̬��");
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	//
	// Initialization of 'Text'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "Wifi�豸����״̬��");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetText(hItem, "");
	WM_SetHasTrans(hItem);
	BUTTON_SetBitmapEx(hItem,0,g_ucWifiStatus?&bmON:&bmOFF,0,0);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetText(hItem, "");
	WM_SetHasTrans(hItem);
	BUTTON_SetBitmapEx(hItem,0,g_ucWifiConRouter?&bmON:&bmOFF,0,0);
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
		WM_CreateTimer(pMsg->hWin, 0, 250, 0);  //������ʱ��
    InitWifiStatusDlg(pMsg);
    break;
	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  565,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
		break;
  case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetBitmapEx(hItem,0,g_ucWifiStatus?&bmON:&bmOFF,0,0);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetBitmapEx(hItem,0,g_ucWifiConRouter?&bmON:&bmOFF,0,0);
	
			WM_RestartTimer(pMsg->Data.v, 250);  //������ʱ��
			break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ReturnButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
         GUI_EndDialog(pMsg->hWin, 0);  //�ر�ʱ�����öԻ���
        break;
      }
      break;
//    case ID_BUTTON_1: // Notifications sent by 'WIFICONM2MButton'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//       
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        break;
//      }
//      break;
//    case ID_BUTTON_2: // Notifications sent by 'WIFICONROUTERButton'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        
//        break;
//     
//      }
//      break;
   
    }
    break;
 
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


void App_WifiStatusDlg( WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

