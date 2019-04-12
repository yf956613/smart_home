/*
*********************************************************************************************************
*	                                  
*	模块名称 : 灯光控制界面设计
*	文件名称 : App_LampCtrlDlg.c
*	版    本 : V1.0
*	说    明 : 灯光控制界面
*            通过下拉列表选择要打开或关闭的灯，黄色图标开灯，灰色图标关灯
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V3.8    2018-12-4    Ares 	         
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/


#include "DIALOG.h"


#include "MyGUI.h"
#include "includes.h"
#include "led.h"
#include "24l01.h"
#include "ucos_msg.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_0    (GUI_ID_USER + 0x02)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_1    (GUI_ID_USER + 0x04)
#define ID_BUTTON_2    (GUI_ID_USER + 0x05)
#define ID_BUTTON_3    (GUI_ID_USER + 0x06)
#define ID_TEXT_1    (GUI_ID_USER + 0x07)
#define ID_TEXT_2    (GUI_ID_USER + 0x08)


static unsigned char  g_ucLampNumber = 0;  //灯的序号(通过这个变量获知选中了哪个灯)

unsigned char LED_flag = 0;
unsigned char LampAutoFlag = 0;
extern void App_LampCtrlChlidDlg(WM_HWIN hWin);

/* 灯光控制对话框初始化资源列表 */
static const GUI_WIDGET_CREATE_INFO _aAppLampDialogCreate[] = {
   { WINDOW_CreateIndirect, "LampWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 361, 58, 333, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 402, 149, 232, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LampCtrl", ID_BUTTON_1, 214, 389, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "AutoCtrl", ID_BUTTON_2, 712, 389, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimerButton", ID_BUTTON_3, 0, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "LampCtrlText", ID_TEXT_1, 184, 330, 201, 33, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "AutoCtrlText", ID_TEXT_2, 672, 330, 232, 35, 0, 0x0, 0 },
 
};


/*
*********************************************************************************************************
*	函 数 名: InitAppLampCtrlDlg
*	功能说明: 初始化灯光照明控制对话框
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitAppLampCtrlDlg(WM_MESSAGE * pMsg)
{
	char buf[10];
	int i;
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//	WM_SetHasTrans(hItem);
	BUTTON_SetText(hItem, "");
	BUTTON_SetBitmapEx(hItem,0,LED_flag?&bmON:&bmOFF,0,0);
//	WM_SetCallback(hItem, _cbOpenButton);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);	
	BUTTON_SetText(hItem, "");
	BUTTON_SetBitmapEx(hItem,0,LampAutoFlag?&bmON:&bmOFF,0,0);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbTimerButton);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "                           ");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "开灯/关灯：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "灯光自控：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetListHeight(hItem, 60);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	for(i = 1; i < LAMP_NUM + 1; i++)
	{
		sprintf(buf,"灯%d",i);
		DROPDOWN_AddString(hItem, buf);
	}
	DROPDOWN_AddString(hItem, "所有灯");
	DROPDOWN_SetItemSpacing(hItem, 50);   //设置元素之间的间距
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
	DROPDOWN_SetListHeight(hItem, 250);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
}

unsigned int LampCountdownSec;
extern unsigned char g_bLampTimerFlag;
extern unsigned char g_ucLampStatus;
/* 灯光控制对话框的回调函数 */
static void _cbAppLampCtrlDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	char g_ucBuf[10];

	char buf[30];

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //创建定时器
			InitAppLampCtrlDlg(pMsg);
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
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		if(g_bLampTimerFlag)
		{
			if(1 == g_ucLampStatus)
			{
				sprintf(buf,"定时开倒计时：%d S",LampCountdownSec);
			}else if(2 == g_ucLampStatus)
			{
				sprintf(buf,"定时关倒计时：%d S",LampCountdownSec);
				
			}
		}
		else
		{
			sprintf(buf,"                             ");
		}
		TEXT_SetText(hItem, buf);
		
		if(!g_bLampTimerFlag)  //如果定时时间到
		{
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,LED_flag?&bmON:&bmOFF,0,0);
		}
//		BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,LampAutoFlag?&bmON:&bmOFF,0,0);
		WM_RestartTimer(pMsg->Data.v, 1000);  //重启定时器
		break;
	case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) 
		{
			case ID_BUTTON_0: // 返回按钮
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:  
						GUI_EndDialog(pMsg->hWin, 0);
					break;
				}
				break;
			case ID_DROPDOWN_0: 
				switch(NCode)
				{
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_SEL_CHANGED:   //下拉列表已经更改
						hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
						g_ucLampNumber = DROPDOWN_GetSel(hItem);
						DROPDOWN_GetItemText(hItem, g_ucLampNumber,g_ucBuf,4);
					break;

				}
				break;
			case ID_BUTTON_1:   //开灯
				switch(NCode)
				{
					case WM_NOTIFICATION_RELEASED:	
								LAMP_SW = ~LAMP_SW;
								LED_flag = !LED_flag;
								BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,LED_flag?&bmON:&bmOFF,0,0);
						break;
					
				}
				break;
			case ID_BUTTON_2:    //灯光自控按钮
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						LampAutoFlag = !LampAutoFlag;
						BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,LampAutoFlag?&bmON:&bmOFF,0,0);
						break;

				}
				break;
			case ID_BUTTON_3: // Notifications sent by 'TimerButton'
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						App_LampCtrlChlidDlg(pMsg->hWin);   //创建定时控制子app
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

void App_LampCtrlDlg(WM_HWIN hWin)     //不小心把回调函数弄成InitAppLampCtrlDlg导致卡死 
{
  GUI_CreateDialogBox(_aAppLampDialogCreate, GUI_COUNTOF(_aAppLampDialogCreate), _cbAppLampCtrlDialog, hWin, 0, 0);
}


