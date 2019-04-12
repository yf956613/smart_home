/*
*********************************************************************************************************
*	                                  
*	模块名称 : 空调控制界面设计
*	文件名称 : App_LampCtrlDlg.c
*	版    本 : V1.0
*	说    明 : 空控制界面
*              大致模拟了遥控器上有的遥控功能
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
#include "MyGUI.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_1    (GUI_ID_USER + 0x05)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_BUTTON_3    (GUI_ID_USER + 0x07)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x08)
#define ID_TEXT_0    (GUI_ID_USER + 0x09)
#define ID_BUTTON_4    (GUI_ID_USER + 0x0A)
#define ID_BUTTON_5    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_6    (GUI_ID_USER + 0x0C)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "AirWindow", ID_WINDOW_0, 0, 0, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CLoseButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenCloseButton", ID_BUTTON_1, 292, 161, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RateButton", ID_BUTTON_2, 598, 161, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "coolButton", ID_BUTTON_3, 292, 365, 140, 50, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 450, 285, 128, 36, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TempText", ID_TEXT_0, 428, 248, 169, 27, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "hotButton", ID_BUTTON_4, 598, 365, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UpDownButton", ID_BUTTON_5, 292, 484, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LeftRightButton", ID_BUTTON_6, 598, 484, 140, 50, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

	 hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	 WM_SetHasTrans(hItem); 
  	WM_SetCallback(hItem, _cbButtonReturn);
    //
    // Initialization of 'OpenCloseButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "开/关");
    //
    // Initialization of 'RateButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "风速");
    //
    // Initialization of 'coolButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
     BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "制冷");
    //
    // Initialization of 'Spinbox'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    SPINBOX_SetFont(hItem, GUI_FONT_32B_1);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 30);  //设置选值得大小范围
	SPINBOX_SetValue(hItem, 20);  //设置选值得大小范围
//	SPINBOX_SetFont(hItem, &GUI_FontHZ24);
    //
    // Initialization of 'TempText'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontHZ32);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "温度");
    //
    // Initialization of 'hotButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
     BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "制热");
    //
    // Initialization of 'UpDownButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
     BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "上下扫风");
    //
    // Initialization of 'LeftRightButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    BUTTON_SetFont(hItem,&GUI_FontHZ32);
		BUTTON_SetText(hItem, "左右扫风");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  565,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
			break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'CLoseButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);  //关闭对话框
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'OpenCloseButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'RateButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'coolButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
      
        break;
    
      }
      break;
    case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
      switch(NCode) 
			{
      case WM_NOTIFICATION_CLICKED:
    
        break;
      case WM_NOTIFICATION_RELEASED:
   
        break;
      case WM_NOTIFICATION_MOVED_OUT:
    
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
    
        break;
   
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'hotButton'
      switch(NCode)
			{
      case WM_NOTIFICATION_RELEASED:
    
        break;
     
      }
      break;
    case ID_BUTTON_5: // Notifications sent by 'UpDownButton'
      switch(NCode)
			{
      case WM_NOTIFICATION_RELEASED:
    
        break;
  
      }
      break;
    case ID_BUTTON_6: // Notifications sent by 'LeftRightButton'
      switch(NCode)
			{
      case WM_NOTIFICATION_RELEASED:
					
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

void App_AirConditioningDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}
