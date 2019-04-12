/*
*********************************************************************************************************
*	                                  
*	模块名称 : 系统信息app设计
*	文件名称 : App_SSysInfoDlg.c
*	版    本 : V1.0
*	说    明 : 系统信息界面
*              显示开发板，处理器，处理器内核等等
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
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_0    (GUI_ID_USER + 0x08)
#define ID_TEXT_1    (GUI_ID_USER + 0x09)
#define ID_TEXT_2    (GUI_ID_USER + 0x0A)

WM_HWIN SysInfohWin;

static const GUI_WIDGET_CREATE_INFO _aSysInfoDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Board", ID_TEXT_0, 61, 140, 518, 450, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "SysInfo", ID_TEXT_1, 27, 64, 139, 38, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OtherText", ID_TEXT_2, 56, 503, 764, 48, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


/* 初始化系统信息 app对话框 */
static void InitSysInfoDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "Board       : 阿波罗STM32F429 \n"
											"Core        : Cortex-M4 \n"
											"CPU         : STM32F429IGT6 \n"
											"CPU Speed   : 180MHZ \n"
											"File System : FatFS \n"
											"GUI         : emwin V5.36 \n"
											"RTOS        : uC/OS-III V3.04 \n"
											"Copy right (C) 2018-2020 Ares");

	//			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	//			TEXT_SetFont(hItem,&GUI_FontHZ24);
	//			TEXT_SetText(hItem, "Core : Cortex-M4");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "欢迎进入智能家居系统");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "系统信息");
}
static void _cbSysInfoDialog(WM_MESSAGE * pMsg) 
{
  int NCode;
  int Id;
//  WM_HWIN hItem;
  WM_HWIN hWin = pMsg->hWin;
  // USER START (Optionally insert additional variables)
  // USER END

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitSysInfoDlg(pMsg);  //初始化对话框
			break;
		case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  565,  	   	   /* 右下角Y 位置 */
							  GUI_ORANGE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
			 break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) 
		{
			case ID_BUTTON_0: // Notifications sent by 'Button'
			switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					GUI_EndDialog(hWin, 0);
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


//WM_HWIN CreateWindow(void);
void AppSysInfoDlg(WM_HWIN hWin) 
{
  SysInfohWin = GUI_CreateDialogBox(_aSysInfoDialogCreate, GUI_COUNTOF(_aSysInfoDialogCreate), _cbSysInfoDialog, hWin, 0, 0);
}
