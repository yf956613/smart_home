#include "DIALOG.h"
#include "MyGUI.h"
#include "stdio.h"
#include "string.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x05)
#define ID_TEXT_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_0    (GUI_ID_USER + 0x07)
#define ID_TEXT_1    (GUI_ID_USER + 0x08)
#define ID_TEXT_2    (GUI_ID_USER + 0x09)

char g_cGas_buf[30];
extern float  g_GasConcentration;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, "GasWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GasText", ID_TEXT_0, 254, 42, 521, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 57, 206, 909, 330, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GasVolText", ID_TEXT_2, 254, 134, 521, 45, 0, 0x0, 0 },

};

/* 初始化可燃气烟雾 app对话框 */
static void InitFireAndAntivirusDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //创建定时器
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "可燃气/烟雾：");
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "MQ2输入电压：");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "本功能通过MQ2传感器检测室内环境中\n"
											"是否有烟雾或可燃气泄漏，从而进行报\n"
											"警提醒用户，以达到防患于未然的效果。\n"
											"通过ADC测量MQ2模拟电压，电压越高，烟\n"
											"将烟雾或可燃泄漏等级分为无、微量、轻\n"
											"度、中度、重度、严重等六个等级");
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
//  WM_HWIN hItem;
  int     NCode;
  int     Id;
	char buf[30];
	static float GasConcentration = 0;
 
  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
		InitFireAndAntivirusDlg(pMsg);
    break;
	case WM_PAINT:
		GUI_DrawGradientV(0,			   /* 左上角X 位置 */
						  0,			   /* 左上角Y 位置 */
						  1024,	   	       /* 右下角X 位置 */
						   600,  	   	   /* 右下角Y 位置 */
						   GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
						   GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
		break;
	case WM_TIMER:
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0),g_cGas_buf);  //显示烟雾/可燃气浓度等级
			if(GasConcentration != g_GasConcentration)
			{
				GasConcentration = g_GasConcentration;
				sprintf(buf,"MQ2输入电压：%f V",g_GasConcentration);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_2),buf);  //显示MQ2电压值
			}
	
			WM_RestartTimer(pMsg->Data.v, 1000);   //重启定时器
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


void App_FireAndAntivirusDlg(WM_HWIN hWin) 
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

