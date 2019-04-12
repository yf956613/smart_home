/*
*********************************************************************************************************
*	                                  
*	模块名称 : 温湿度app设计
*	文件名称 : App_TempHumiDlg.c
*	版    本 : V1.0
*	说    明 : 温湿度界面
*              显示温湿度
*              显示提示信息
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

/* 初始化温湿度 app对话框 */
static void InitTempHumiDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 100, 0);	  //创建定时器
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "当前温度： ");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "当前湿度：");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "据研究表明，室内最适合温度应保持在室温达 18℃ 时，\n"
											"相对湿度应保持在30—40%，室温达 25℃ 时，相对湿度应\n"
											"保持在 40—50% 为最宜。室内湿度也不宜过高或过低，室\n"
											"内湿度过高，人体散热就比较困难。室内湿度过低，空气\n"
											"干燥，人的呼吸道会干涩难受。通过DHT11温湿度传感器 \n"
											"监测室内环境温湿度并显示、、、、、、、、、、、、、\n"
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
		GUI_DrawGradientV(0,			   /* 左上角X 位置 */
						  0,			   /* 左上角Y 位置 */
						  1024,	   	       /* 右下角X 位置 */
						   600,  	   	   /* 右下角Y 位置 */
						   GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
						   GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
	break;
	case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			sprintf(buf,"当前温度：%d ℃",temperature);
			TEXT_SetText(hItem, buf);
	
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			sprintf(buf,"当前湿度：%d %%RH",humitiy);
			TEXT_SetText(hItem, buf);
	
			WM_RestartTimer(pMsg->Data.v, 100);  //重启定时器
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


void App_TempHumiDlg(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

