/*
*********************************************************************************************************
*	                                  
*	模块名称 : 家电定时管理子界面设计
*	文件名称 : App_HomeApplianceTimerChildDlg.c
*	版    本 : V4.0
*	说    明 : 家电定时管理子界面
*              Spinbox设置定时时间
*              下拉列表选择插座
*               复选框选择定时开or定时关
*               按钮启动定时或取消定时  
* 
*		版本号   日期         作者          说明
*		V1.0    2018-12-8   Ares  	       首版   
*   Mail     1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "stdio.h"
#include "MyGUI.h"
#include "includes.h"
#include "led.h"
#include "ucos_msg.h"
#include "usart2.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_0    (GUI_ID_USER + 0x02)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x03)
#define ID_TEXT_1    (GUI_ID_USER + 0x04)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x05)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_1    (GUI_ID_USER + 0x07)
#define ID_TEXT_2    (GUI_ID_USER + 0x08)
#define ID_BUTTON_2    (GUI_ID_USER + 0x09)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x0A)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "XXXTimerWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "xxxTimerText", ID_TEXT_0, 319, 58, 375, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 260, 219, 144, 37, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TimeSetText", ID_TEXT_1, 81, 222, 161, 33, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "SaveCheckbox", ID_CHECKBOX_0, 567, 223, 196, 29, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "SocketDropdown", ID_DROPDOWN_0, 260, 362, 155, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "EnterButton", ID_BUTTON_1, 746, 206, 189, 59, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "MinText", ID_TEXT_2, 420, 222, 131, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CancleButton", ID_BUTTON_2, 746, 357, 189, 59, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 567, 370, 199, 29, 0, 0x0, 0 },

};

OS_TMR 	g_HomeAppliancetmr2;		//定时器2
void g_HomeAppliancetmr2_callback(void *p_tmr, void *p_arg); 	//插座定时器回调函数

static unsigned char g_ucHomeApplianceNumber = 0;  //选中家电的编号
static unsigned int g_u32TimerToSet;   //存放SPINBox设置的时间
static unsigned int g_CountSec = 0;  //用来计数周期定时器经过了多少秒
static char g_ucBuf[10];
unsigned char g_ucHomeApplianceStatus = 0;  //家电的状态
unsigned char g_bHomeApplianceTimerFlag;    //定时器启动标志


//初始化家电定时控制窗口
static void InitSocketCtrlChildDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	char buf[15];
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	//
	// Initialization of 'xxxTimerText'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "家电定时控制");
	//
	// Initialization of 'Spinbox'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
	SPINBOX_SetFont(hItem, GUI_FONT_32B_1);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 360);  //设置选值得大小范围

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "定时时间：");
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);


	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "定时开");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "定时关");

	//
	// Initialization of 'SocketDropdown'
	//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	DROPDOWN_SetListHeight(hItem, 60);
	DROPDOWN_AddString(hItem, "空调1");
	DROPDOWN_AddString(hItem, "空调2");
	DROPDOWN_AddString(hItem, "空调3");
	DROPDOWN_AddString(hItem, "电视1");
	DROPDOWN_AddString(hItem, "电视2");
	DROPDOWN_AddString(hItem, "电视3");
	DROPDOWN_AddString(hItem, "冰箱1");
	DROPDOWN_AddString(hItem, "冰箱2");
	DROPDOWN_AddString(hItem, "冰箱3");
	DROPDOWN_AddString(hItem, "洗衣机1");
	DROPDOWN_AddString(hItem, "洗衣机2");
	DROPDOWN_AddString(hItem, "洗衣机3");
	DROPDOWN_SetItemSpacing(hItem, 10);   //设置元素之间的间距
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
	DROPDOWN_SetListHeight(hItem, 150);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
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
}
static void _cbDialog(WM_MESSAGE * pMsg) 
{
 
  int     NCode;
  int     Id;
  
  
	OS_ERR err;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitSocketCtrlChildDlg(pMsg);
//		WM_CreateTimer(pMsg->hWin, 0, 250, 0);		//创建一个1S钟的定时器
		break;
	case WM_TIMER:
//			sprintf(buf,"倒计时：%d 秒",g_u32TimerToSet * 60-g_CountSec);
//			TEXT_SetText(g_CountSechItem,buf);
//			WM_RestartTimer(pMsg->Data.v, 250);    
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
    case ID_DROPDOWN_0: // Notifications sent by 'SocketDropdown'
      switch(NCode) 
			{
      case WM_NOTIFICATION_CLICKED:
       
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
				g_ucHomeApplianceNumber = DROPDOWN_GetSel(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
				DROPDOWN_GetItemText(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), g_ucHomeApplianceNumber,g_ucBuf,sizeof(g_ucBuf));    
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'EnterButton'
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
					{
						g_ucHomeApplianceStatus = 1;
					}
					
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
					{
						g_ucHomeApplianceStatus = 2;
					}
					if(g_u32TimerToSet > 0 && g_ucHomeApplianceStatus > 0)  //定时时间和选中开或关才开启定时器
					{				
						if(OSTmrStateGet ((OS_TMR  *)&g_HomeAppliancetmr2,(OS_ERR  *)&err) == OS_TMR_STATE_UNUSED)  //如果定时器没有创建
						{
							g_bHomeApplianceTimerFlag = 1;
							OSTmrCreate((OS_TMR		*)&g_HomeAppliancetmr2,		//定时器1
														(CPU_CHAR	*)"g_HomeAppliancetmr2",		//定时器名字
														(OS_TICK	 )0,			//20*10=200ms
														(OS_TICK	 )100,          //100*10=1000ms
														(OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
														(OS_TMR_CALLBACK_PTR)g_HomeAppliancetmr2_callback, //定时器1回调函数
														(void	    *)0,			//参数为0
														(OS_ERR	    *)&err);		//返回的错误码		
							OSTmrStart(&g_HomeAppliancetmr2,&err);	//开启定时器1
							GUI_EndDialog(pMsg->hWin, 0);  //关闭对话框
					 }
					}
					
					break;
     
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CancleButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
				if(OSTmrStateGet ((OS_TMR  *)&g_HomeAppliancetmr2,(OS_ERR  *)&err) != OS_TMR_STATE_UNUSED) //定时器已经创建
				 {
						OSTmrDel ((OS_TMR  *)&g_HomeAppliancetmr2,(OS_ERR  *)&err);  //取消定时-删除定时器
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

void App_HomeApplianceTimerChildDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

extern unsigned int HomeApplianceCountdownSec;

//定时器的回调函数
void g_HomeAppliancetmr2_callback(void *p_tmr, void *p_arg)
{
	OS_ERR  err;
	g_CountSec++;
//	char RX_TX_Buf[10];
	HomeApplianceCountdownSec = g_u32TimerToSet*60 - g_CountSec;
	if(g_CountSec >= (g_u32TimerToSet*60))
	{
		g_CountSec = 0;
		g_bHomeApplianceTimerFlag = 0;
		g_u32TimerToSet = 0;
		if(g_ucHomeApplianceStatus == 1)   //定时开
		{
			usart2_send_byte(0xBB);		       //发送开机指令
			usart2_send_byte(0x01);
			usart2_send_byte(0xFF);
		}else if(g_ucHomeApplianceStatus == 2)   //定时关
		{
			usart2_send_byte(0xBB);		        //发送关机指令
			usart2_send_byte(0x04);
			usart2_send_byte(0xFF);
		}
		OSTmrDel ((OS_TMR  *)&g_HomeAppliancetmr2,(OS_ERR  *)&err);  //定时时间到,删除定时器
	}
}

