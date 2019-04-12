/*
*********************************************************************************************************
*	                                  
*	模块名称 : 空气质量app界面设计
*	文件名称 : App_AirQualityDlg.c
*	版    本 : V3.8
*	说    明 : 空气质量界面
*            显示空气质量
*            MQ135输入电压
*            通过下拉列表选择要打开或关闭的灯，黄色图标开灯，灰色图标关灯
*              
*	
*		版本号   日期         作者          说明
*		V3.8    2018-12-4    Ares 	       首版   
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

/* 初始化空气质量 app对话框 */
static void InitAirQualityDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //创建定时器
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	//
	// Initialization of 'AirText'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "空气质量：");
	//
	// Initialization of 'AirVolText'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "MQ135输入电压：");
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	//
	// Initialization of 'Text'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "利用MQ-135传感器监测空气质量，通过ADC采集MQ-135\n"
											"输入的模拟电压并转换为数字电压，完全无污染的状态\n"
											"下，输入电压值为0~0.3V,电压越大，空气的质量就越差\n"
											"本设计根据电压把空气质量划分为六个等级，分别是优，\n"
											"良好，轻度，中度，重度，严重;空气质量对人体影响极\n"
											"其重要，智能家居必须可以检测室内空气质量~~~~~~~~~~");
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
		case WM_PAINT:  //绘制背景颜色
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  565,  	   	   /* 右下角Y 位置 */
							  GUI_ORANGE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
		break;
	case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetText(hItem, g_cAir_buf);
	
			if(air != g_Air_test)
			{
				air = g_Air_test;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
				sprintf(buf,"MQ135输入电压: %f V",g_Air_test);
				TEXT_SetText(hItem, buf);
			}
	
			WM_RestartTimer(pMsg->Data.v, 1000);  //重启定时器
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
						GUI_EndDialog(pMsg->hWin, 0);  //返回，关闭对话框 
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


