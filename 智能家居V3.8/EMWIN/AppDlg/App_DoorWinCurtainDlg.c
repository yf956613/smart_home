#include "DIALOG.h"
#include "MyGUI.h"
#include "string.h"
#include "includes.h"
#include "stdio.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x02)
#define ID_DROPDOWN_1    (GUI_ID_USER + 0x03)
#define ID_DROPDOWN_2    (GUI_ID_USER + 0x04)
#define ID_BUTTON_1    (GUI_ID_USER + 0x05)
#define ID_BUTTON_2    (GUI_ID_USER + 0x07)
#define ID_BUTTON_3    (GUI_ID_USER + 0x08)
#define ID_TEXT_0    (GUI_ID_USER + 0x0A)
#define ID_TEXT_1    (GUI_ID_USER + 0x0C)
#define ID_TEXT_2    (GUI_ID_USER + 0x0D)


unsigned char WinFLag         = 0;     //窗状态标志位
unsigned char WinAutoFlag     = 0;     //窗自动控制标志位

unsigned char DoorFLag        = 0;     //门状态标志位

unsigned char CurtainFLag     = 0;  //窗帘状态标志位
unsigned char AutoCurtainFlag = 0;  //窗帘自动控制标志位


//OS_FLAG_GRP  WinAndCurtainAutoFlagGrp;    //门窗自动控制事件标志组

//OS_SEM DOOR_WIN_CURTAIN_SEM;
extern OS_TCB RX_TX_TaskTCB;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
 { WINDOW_CreateIndirect, "DoorWinWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "WinDropdown", ID_DROPDOWN_0, 73, 139, 210, 30, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "DoorDropdown", ID_DROPDOWN_1, 388, 138, 210, 30, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "CurtainDropdown", ID_DROPDOWN_2, 713, 139, 210, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenButton", ID_BUTTON_1, 100, 447, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenCloseDoorButton", ID_BUTTON_2, 443, 447, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenCloseCurtainButton", ID_BUTTON_3, 774, 447, 110, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "WinOpenText", ID_TEXT_0, 86, 391, 141, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "DoorOpenText", ID_TEXT_1, 422, 391, 166, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "CurtainOpenText", ID_TEXT_2, 753, 391, 168, 33, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

WM_HWIN DoorWinCurtainAppHwin;
 
/* 初始化门窗窗帘 app对话框 */
void InitAppDoorWinCurtainDlg(WM_MESSAGE * pMsg)
{
	  WM_HWIN hItem;
		int i;
	 char buf[40];
	
	 hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonReturn);
    //
    // Initialization of 'WinDropdown'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
		DROPDOWN_SetListHeight(hItem, 60);
		for(i = 1; i < 4 + 1; i++)
		{
			sprintf(buf,"窗%d",i);
			DROPDOWN_AddString(hItem, buf);
		}
		DROPDOWN_AddString(hItem, "所有窗");
		DROPDOWN_SetItemSpacing(hItem, 20);   //设置元素之间的间距
		DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
		DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
		DROPDOWN_SetListHeight(hItem, 150);
		DROPDOWN_SetScrollbarWidth(hItem, 35);
    //
    // Initialization of 'DoorDropdown'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
    DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
		DROPDOWN_SetListHeight(hItem, 60);
		for(i = 1; i < 4 + 1; i++)
		{
			sprintf(buf,"门%d",i);
			DROPDOWN_AddString(hItem, buf);
		}
		DROPDOWN_AddString(hItem, "所有门");
		DROPDOWN_SetItemSpacing(hItem, 20);   //设置元素之间的间距
		DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
		DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
		DROPDOWN_SetListHeight(hItem, 150);
		DROPDOWN_SetScrollbarWidth(hItem, 35);
    //
    // Initialization of 'CurtainDropdown'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_2);
    DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
		DROPDOWN_SetListHeight(hItem, 60);
		for(i = 1; i < 4 + 1; i++)
		{
			sprintf(buf,"窗帘%d",i);
			DROPDOWN_AddString(hItem, buf);
		}
		DROPDOWN_AddString(hItem, "所有窗帘");
		DROPDOWN_SetItemSpacing(hItem, 20);   //设置元素之间的间距
		DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
		DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
		DROPDOWN_SetListHeight(hItem, 150);
		DROPDOWN_SetScrollbarWidth(hItem, 35);
    //
    // Initialization of 'OpenButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetText(hItem, "");
		BUTTON_SetBitmapEx(hItem,0,WinFLag?&bmON:&bmOFF,0,0);
    
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetText(hItem, "");
		BUTTON_SetBitmapEx(hItem,0,DoorFLag?&bmON:&bmOFF,0,0);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
		BUTTON_SetText(hItem, "");
		BUTTON_SetBitmapEx(hItem,0,CurtainFLag?&bmON:&bmOFF,0,0);

//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
//		BUTTON_SetText(hItem, "");
//		BUTTON_SetBitmapEx(hItem,0,CurtainFLag?&bmON:&bmOFF,0,0);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem,&GUI_FontHZ24);
		TEXT_SetText(hItem, "开窗/关窗：");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetFont(hItem,&GUI_FontHZ24);
		TEXT_SetText(hItem, "开门/关门：");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont(hItem,&GUI_FontHZ24);
		TEXT_SetText(hItem, "开帘/闭帘：");
    //
    // Initialization of 'Checkbox'
    //
}

/*********************************************************************
*
*       _cbDialog对话框回调函数
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  int     NCode;
  int     Id;
	OS_ERR err;
	char TimerId;
//	WM_HTIMER hTimer; 
//	static char Flag = 0;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	
			InitAppDoorWinCurtainDlg(pMsg);
			break;
		case MSG_UPDATECURTAIN:
			WM_InvalidateWindow(DoorWinCurtainAppHwin);
			break;
		case MSG_UPDATEDOOR:
			WM_InvalidateWindow(DoorWinCurtainAppHwin);
			break;
	  case MSG_UPDATEWIN:
			WM_InvalidateWindow(DoorWinCurtainAppHwin);
			break;
		case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
			0,			   /* 左上角Y 位置 */
			1024,	   	       /* 右下角X 位置 */
			565,  	   	   /* 右下角Y 位置 */
			GUI_BLACK,	   /* 矩形最左侧要绘制的颜色 */
			GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
	
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,WinFLag?&bmON:&bmOFF,0,0);
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,DoorFLag?&bmON:&bmOFF,0,0);	
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3),0,CurtainFLag?&bmON:&bmOFF,0,0);
		
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
					    GUI_EndDialog(pMsg->hWin, 0);
					break;

				}
				break;
			case ID_DROPDOWN_0: // Notifications sent by 'WinDropdown'
				switch(NCode)
				{
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_SEL_CHANGED:

					break;

				}
				break;
		case ID_DROPDOWN_1: // Notifications sent by 'DoorDropdown'
			switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:

				break;
				case WM_NOTIFICATION_RELEASED:

				break;
				case WM_NOTIFICATION_SEL_CHANGED:

				break;

			}
			break;
		case ID_DROPDOWN_2: // Notifications sent by 'CurtainDropdown'
			switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:

				break;
				case WM_NOTIFICATION_RELEASED:

				break;
				case WM_NOTIFICATION_SEL_CHANGED:

				break;

			}
			break;

		case ID_BUTTON_1: 
			switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					WinFLag = ~WinFLag;
					if(WinFLag)
					{				
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送开窗消息
											(void*		)WIN_ON,
											(OS_MSG_SIZE)sizeof(WIN_ON),
											(OS_OPT		)OS_OPT_POST_FIFO,
											(OS_ERR*	)&err);
					}
					else
					{
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送消息
											(void*		)WIN_OFF,
											(OS_MSG_SIZE)sizeof(WIN_OFF),
											(OS_OPT		)OS_OPT_POST_FIFO,
											(OS_ERR*	)&err);
						
					}
					BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,WinFLag?&bmON:&bmOFF,0,0); //禁用状态下显示按钮图案				
				break;

			}
			break;
		case ID_BUTTON_2: // Notifications sent by 'DoorButton'
			switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					DoorFLag = !DoorFLag;
//					BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,DoorFLag?&bmON:&bmOFF,0,0);
					if(DoorFLag)
					{
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送消息
											(void*		)DOOR_ON,
											(OS_MSG_SIZE)sizeof(DOOR_ON),
											(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
					}
					else
					{
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送消息
											(void*		)DOOR_OFF,
											(OS_MSG_SIZE)sizeof(DOOR_OFF),
											(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
					}
//					WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2));   //开窗没完成禁用按钮而不能按关闭
					BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,DoorFLag?&bmON:&bmOFF,0,0); //禁用状态下显示按钮图案
//					WM_CreateTimer(pMsg->hWin, 2, 1700, 0);	  //创建定时器
				break;

			}
			break;
		case ID_BUTTON_3: // Notifications sent by 'OpenCloseDoorButton'
			switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					CurtainFLag = ~CurtainFLag;
//					BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3),0,CurtainFLag?&bmON:&bmOFF,0,0);
					if(CurtainFLag)
					{				
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送消息
											(void*		)CURTAIN_ON,
											(OS_MSG_SIZE)sizeof(CURTAIN_ON),
											(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
					}
					else
					{
						OSTaskQPost((OS_TCB*	)&RX_TX_TaskTCB,	//向任务RX_TX_Task发送消息
											(void*		)CURTAIN_OFF,
											(OS_MSG_SIZE)sizeof(CURTAIN_OFF),
											(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
					}
//					WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3));   //开窗没完成禁用按钮而不能按关闭
					BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3),0,CurtainFLag?&bmON:&bmOFF,0,0); //禁用状态下显示按钮图案
//					WM_CreateTimer(pMsg->hWin, 3, 8500, 0);	  //创建定时器
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


void App_DoorWinCurtainDlg( WM_HWIN hWin) 
{
  DoorWinCurtainAppHwin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}
