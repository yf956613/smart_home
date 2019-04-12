/*
*********************************************************************************************************
*	                                  
*	模块名称 : Wifi app设计
*	文件名称 : App_WifiStatusDlg.c
*	版    本 : V1.0
*	说    明 : wifi app界面
*              1. 显示手机设备连接状态
*              2. 显示wifi联网状态
*							 .......................
*              
*
*		版本号   日期         作者          说明
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

extern unsigned char g_ucWifiStatus;    //手机设备是否连上机智云服务器标志
extern unsigned char g_ucWifiConRouter; //wifi模组是否连接路由器(即wifi是否联网) 


/* 初始化wifi app对话框 */
static void InitWifiStatusDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "手机设备连接状态：");
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	//
	// Initialization of 'Text'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "Wifi设备连接状态：");

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
		WM_CreateTimer(pMsg->hWin, 0, 250, 0);  //创建定时器
    InitWifiStatusDlg(pMsg);
    break;
	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  565,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
		break;
  case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetBitmapEx(hItem,0,g_ucWifiStatus?&bmON:&bmOFF,0,0);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetBitmapEx(hItem,0,g_ucWifiConRouter?&bmON:&bmOFF,0,0);
	
			WM_RestartTimer(pMsg->Data.v, 250);  //重启定时器
			break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ReturnButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
         GUI_EndDialog(pMsg->hWin, 0);  //关闭时间设置对话框
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

