/*
*********************************************************************************************************
*	                                  
*	模块名称 : 开发者app界面设计
*	文件名称 : App_DeveloperDlg.c
*	版    本 : V1.0
*	说    明 : 开发者界面
*            显示主要的系统总体框架功能说明
*              
*		版本号   日期         作者      
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

/* 初始化开发者 app对话框 */
static void InitDeveloperDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "本设计结合了物联网技术和嵌入式技术，可应用\n"
											"于家庭住宅环境远程检测、控制的智能家居系统。\n"
											"RTOS采用uC/OS-III嵌入式实时操作系统和GUI采用\n"
											"emwin图形界面，以ARM Cortex-M4内核架构STM32F4\n"
											"系列单片机作为主控。利用DHT11监测温度、湿度；\n"
											"MQ2监测可燃气/烟雾；利用MQ135监测空气质量。可app\n"
											"远程以及通过GUI控制灯光、插座、门窗、窗帘、电器等。\n"
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
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
									0,			   /* 左上角Y 位置 */
									1024,	   	       /* 右下角X 位置 */
									600,  	   	   /* 右下角Y 位置 */
									GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
									GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
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
							GUI_EndDialog(pMsg->hWin, 0);  //关闭对话框
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

