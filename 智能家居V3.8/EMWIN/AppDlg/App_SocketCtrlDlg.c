/*
*********************************************************************************************************
*	                                  
*	模块名称 : 插座控制界面设计
*	文件名称 : App_SocketCtrlDlg.c
*	版    本 : V1.0
*	说    明 : 插座控制界面
*              1. 下拉列表用于选择要操作的插座，
*              2. 通过下拉列表选择要打开或关闭的灯，黄色图标开灯，灰色图标关灯
*              
*	修改记录 :
*		版本号   日期         作者        
*		V3.8    2018-12-8     Ares  	          
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/


#include "DIALOG.h"


#include "MyGUI.h"
#include "includes.h"
#include "led.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_0    (GUI_ID_USER + 0x02)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_1    (GUI_ID_USER + 0x04)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_TEXT_1    (GUI_ID_USER + 0x07)

unsigned char SocketOnOfflag = 0;
static unsigned char  g_ucSocketNumber = 0;  //灯的序号(通过这个变量获知选中了哪个灯)
static char g_cBuf[10];

/* 灯光控制对话框初始化资源列表 */
static const GUI_WIDGET_CREATE_INFO _aAppLampDialogCreate[] = {
  { WINDOW_CreateIndirect, "SocketWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 368, 57, 333, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 402, 149, 232, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ON", ID_BUTTON_1, 467, 462, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimerButton", ID_BUTTON_2, 0, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OpenCloseText", ID_TEXT_1, 433, 402, 217, 31, 0, 0x0, 0 },
};

extern void App_SocketCtrlChlidDlg(WM_HWIN hWin);
/*
*********************************************************************************************************
*	函 数 名: InitAppLampCtrlDlg
*	功能说明: 初始化灯光照明控制对话框
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitAppSocketCtrlDlg(WM_MESSAGE * pMsg)
{
	char buf[10];
	int i;
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//	WM_SetHasTrans(hItem);
//	BUTTON_SetFont(hItem,&GUI_FontHZ32);
	BUTTON_SetText(hItem, "");
	BUTTON_SetBitmapEx(hItem,0,SocketOnOfflag?&bmON:&bmOFF,0,0);
//	WM_SetCallback(hItem, _cbOpenButton);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbTimerButton);
	
	
	//显示标题
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "                              ");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "开启/关闭：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetListHeight(hItem, 60);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	for(i = 1; i < SOCKET_NUM + 1; i++)
	{
		sprintf(buf,"插座%d",i);
		DROPDOWN_AddString(hItem, buf);
	}
	DROPDOWN_AddString(hItem, "所有插座");
	DROPDOWN_SetItemSpacing(hItem, 50);   //设置元素之间的间距
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
	DROPDOWN_SetListHeight(hItem, 250);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
}
extern unsigned char g_bSocketTimerFlag;
extern unsigned char g_ucSocketStatus;
unsigned int SocketCountdownSec;
/* 灯光控制对话框的回调函数 */
static void _cbAppSocketCtrlDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	char buf[30];

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //创建定时器
			InitAppSocketCtrlDlg(pMsg);
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
			if(g_bSocketTimerFlag)
			{
				if(1 == g_ucSocketStatus)
				{
					sprintf(buf,"定时开倒计时：%d S",SocketCountdownSec);
					TEXT_SetText(hItem, buf);
				}
				if(2 == g_ucSocketStatus)
				{
					sprintf(buf,"定时关倒计时：%d S",SocketCountdownSec);
					TEXT_SetText(hItem, buf);
				}
			}
			else
			{
				sprintf(buf,"                             ");
				TEXT_SetText(hItem, buf);
			}
			
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,SocketOnOfflag?&bmON:&bmOFF,0,0);
			WM_RestartTimer(pMsg->Data.v, 1000);  //重启定时器
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
								GUI_EndDialog(pMsg->hWin, 0);
					break;
		 
				}
				break;
				case ID_DROPDOWN_0: 
				switch(NCode)
				{
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_SEL_CHANGED:   //下拉列表已经更改
						hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
						g_ucSocketNumber = DROPDOWN_GetSel(hItem);
						DROPDOWN_GetItemText(hItem, g_ucSocketNumber,g_cBuf,sizeof(g_cBuf));
					break;
			  
				}
				break;
				case ID_BUTTON_1:   //开
				switch(NCode)
				{
				  case WM_NOTIFICATION_RELEASED:
							SOCKET_SW = ~SOCKET_SW;
							SocketOnOfflag = !SocketOnOfflag;
							BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,SocketOnOfflag?&bmON:&bmOFF,0,0);
					break;
				
				}
				break;
				case ID_BUTTON_2:   
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						App_SocketCtrlChlidDlg(pMsg->hWin);   //创建定时管理子窗口
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

void App_SocketCtrlDlg(WM_HWIN hWin)     
{
  GUI_CreateDialogBox(_aAppLampDialogCreate, GUI_COUNTOF(_aAppLampDialogCreate), _cbAppSocketCtrlDialog, hWin, 0, 0);
}


