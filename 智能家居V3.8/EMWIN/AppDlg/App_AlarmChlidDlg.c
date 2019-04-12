/*
*********************************************************************************************************
*	                                  
*	模块名称 : 安防报警定时控制界面设计
*	文件名称 : App_AlarmChildDlg.c
*	版    本 : V1.0
*	说    明 : 安防报警定时控制
*            通过Spinbox控件可设置定时时间
*						 通过复选框保存定时设置
*            通过开启定时按钮开始定时
*            通过取消定时按钮取消定时 
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2018-12-4    Ares 	       首版   
*                                     
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/
#include "DIALOG.h"
#include "stdio.h"
#include "MyGUI.h"
#include "includes.h"
#include "led.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_0    (GUI_ID_USER + 0x0C)
#define ID_TEXT_0    (GUI_ID_USER + 0x0D)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x0E)
#define ID_TEXT_1    (GUI_ID_USER + 0x0F)
#define ID_BUTTON_1    (GUI_ID_USER + 0x12)
#define ID_TEXT_2    (GUI_ID_USER + 0x13)
#define ID_BUTTON_2    (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x15)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x16)




static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "AlaTimerWindow", ID_WINDOW_0, 0, 0, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "xxxTimerText", ID_TEXT_0, 319, 58, 375, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 439, 220, 144, 37, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TimeSetText", ID_TEXT_1, 260, 223, 161, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "EnterButton", ID_BUTTON_1, 200, 434, 189, 59, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "MinText", ID_TEXT_2, 601, 223, 131, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CancleButton", ID_BUTTON_2, 648, 434, 189, 59, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 318, 336, 213, 32, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 630, 330, 219, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

static unsigned int g_u32TimerToSet;   //存放SPINBox设置的时间
OS_TMR 	g_Alarmtmr3;		//定时器2
void g_Alarmtmr3_callback(void *p_tmr, void *p_arg); 	//插座定时器回调函数
char g_bAlarmTimerFlag = 0;
static unsigned int g_CountSec = 0;  //用来计数周期定时器经过了多少秒
unsigned char g_ucAlarmStatus = 0;  //防盗报警开关标志状态

void InitAlarmChildDlg(WM_MESSAGE * pMsg)
{
}


static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	OS_ERR  err;


	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonReturn);
			//
			// Initialization of 'xxxTimerText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "防盗报警定时控制");
			//
			// Initialization of 'Spinbox'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
		SPINBOX_SetFont(hItem, GUI_FONT_32B_1);
		SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
		SPINBOX_SetRange(hItem, 0, 360);  //设置选值得大小范围
	//	SPINBOX_SetFont(hItem, &GUI_FontHZ24);
			//
			// Initialization of 'TimeSetText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetText(hItem, "定时时间：");
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			//
			// Initialization of 'SaveCheckbox'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
		CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
		CHECKBOX_SetText(hItem, "定时开");
			
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
		CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
		CHECKBOX_SetText(hItem, "定时关");
		
			//
			// Initialization of 'EnterButton'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "开启定时");
			//
			// Initialization of 'MinText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetText(hItem, "分钟");
			//
			// Initialization of 'CancleButton'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "取消定时");
			
		break;
  case WM_PAINT:
	GUI_DrawGradientV(0,			   /* 左上角X 位置 */
					  0,			   /* 左上角Y 位置 */
					  1024,	   	       /* 右下角X 位置 */
					  600,  	   	   /* 右下角Y 位置 */
					  GUI_ORANGE,	   /* 矩形最左侧要绘制的颜色 */
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
    case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
      switch(NCode) 
	  {
    
      case WM_NOTIFICATION_RELEASED:
       
        break;
      case WM_NOTIFICATION_MOVED_OUT:
      
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
				g_u32TimerToSet = SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0));  //获取定时时间
        break;
     
      }
      break;
    case ID_CHECKBOX_0: // Notifications sent by 'SaveCheckbox'
      switch(NCode) 
			{
      case WM_NOTIFICATION_CLICKED:
       
        break;
      case WM_NOTIFICATION_RELEASED:
      
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))  //如果选中了定时开
				{
					CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);   //取消选中定时关
				}			
				break;
     
      }
      break;
		case ID_CHECKBOX_1:
      switch(NCode) 
			{
			case WM_NOTIFICATION_CLICKED:
			 
				break;
			case WM_NOTIFICATION_RELEASED:
			
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))  //如果选中了定时关
				{
					CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);   //取消选中定时开
				}			
				break;
     
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'EnterButton'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
					{
						g_ucAlarmStatus = 1;
					}
					
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
					{
						g_ucAlarmStatus = 2;
					}
					if(g_u32TimerToSet > 0 && g_ucAlarmStatus > 0)  //定时时间和选中开或关才开启定时器
					{
						g_bAlarmTimerFlag = 1;
						OSTmrCreate((OS_TMR		*)&g_Alarmtmr3,		//定时器1
													(CPU_CHAR	*)"g_Alarmtmr3",		//定时器名字
													(OS_TICK	 )0,			//20*10=200ms
													(OS_TICK	 )100,          //100*10=1000ms
													(OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
													(OS_TMR_CALLBACK_PTR)g_Alarmtmr3_callback,//定时器1回调函数
													(void	    *)0,			//参数为0
													(OS_ERR	    *)&err);		//返回的错误码		
						OSTmrStart(&g_Alarmtmr3,&err);	//开启定时器1
						GUI_EndDialog(pMsg->hWin, 0);  //关闭对话框
				}
        break;
     
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CancleButton'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
				if(OSTmrStateGet ((OS_TMR  *)&g_Alarmtmr3,(OS_ERR  *)&err) != OS_TMR_STATE_UNUSED) //定时器已经创建
				 {
						OSTmrDel((OS_TMR  *)&g_Alarmtmr3,(OS_ERR  *)&err);  //取消定时，删除定时器
				 }
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

void App_AlarmCtrlChlidDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

extern unsigned int AlaCountdownSec;
//定时器1的回调函数
void g_Alarmtmr3_callback(void *p_tmr, void *p_arg)
{
	OS_ERR  err;
	g_CountSec++;
	AlaCountdownSec = g_u32TimerToSet*60 - g_CountSec;
	if(g_CountSec >= (g_u32TimerToSet*60))
	{
		g_CountSec = 0;
		g_bAlarmTimerFlag = 0; //定时时间到，定时器停止
		/* 定时时间到 下面是用户代码 */
		if(g_ucAlarmStatus == 1)
		{
			LED0 = 0;
		}
		if(g_ucAlarmStatus == 2)
		{
			LED0 = 1;
		}
		
		OSTmrDel ((OS_TMR *)&g_Alarmtmr3,(OS_ERR  *)&err);  //定时时间到,删除定时器
	}
}



