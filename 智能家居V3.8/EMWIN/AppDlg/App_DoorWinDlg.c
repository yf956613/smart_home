#include "DIALOG.h"
#include "MyGUI.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_1    (GUI_ID_USER + 0x02)
#define ID_BUTTON_2    (GUI_ID_USER + 0x03)
#define ID_BUTTON_3    (GUI_ID_USER + 0x04)
#define ID_BUTTON_4    (GUI_ID_USER + 0x05)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "DoorWinWindow", ID_WINDOW_0, 0, -1, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenDoorButton", ID_BUTTON_0, 279, 155, 120, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CloseDoorButton", ID_BUTTON_1, 279, 350, 120, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenwinButton", ID_BUTTON_2, 603, 155, 120, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CloseWinButton", ID_BUTTON_3, 606, 350, 120, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_4, 924, 0, 100, 100, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

static void _cdOpenDoorButton(WM_MESSAGE * pMsg) //--------------（6）
{
	WM_HWIN hWin;
	GUI_RECT Rect;
	hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(GUI_RED);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);;
				GUI_SetBkColor(GUI_RED);
				GUI_SetColor(GUI_WHITE);
			}
			GUI_SetFont(&GUI_FontHZ24);
			GUI_DispStringInRect("开门", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cdCloseDoorButton(WM_MESSAGE * pMsg) //--------------（6）
{
	WM_HWIN hWin;
	GUI_RECT Rect;
	hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(GUI_BLUE);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);;
				GUI_SetBkColor(GUI_BLUE);
				GUI_SetColor(GUI_WHITE);
			}
			GUI_SetFont(&GUI_FontHZ24);
			GUI_DispStringInRect("关门", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cdOpenWinButton(WM_MESSAGE * pMsg) //--------------（6）
{
	WM_HWIN hWin;
	GUI_RECT Rect;
	hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(GUI_RED);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);;
				GUI_SetBkColor(GUI_RED);
				GUI_SetColor(GUI_WHITE);
			}
			GUI_SetFont(&GUI_FontHZ24);
			GUI_DispStringInRect("开窗", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
		default:
			BUTTON_Callback(pMsg);
	}
}

static void _cdCloseWinButton(WM_MESSAGE * pMsg) //--------------（6）
{
	WM_HWIN hWin;
	GUI_RECT Rect;
	hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin))
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE);
			}
			else
			{
				GUI_SetColor(GUI_BLUE);
				GUI_AA_FillCircle((Rect.x1-Rect.x0)/2,(Rect.y1-Rect.y0)/2, (Rect.y1-Rect.y0)/2);;
				GUI_SetBkColor(GUI_BLUE);
				GUI_SetColor(GUI_WHITE);
			}
			GUI_SetFont(&GUI_FontHZ24);
			GUI_DispStringInRect("关窗", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
		break;
		default:
			BUTTON_Callback(pMsg);
	}
}


/*********************************************************************
*
*       _cbDialog
*/
static void _cbAppDoorWinDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
    //
    // Initialization of 'OpenDoorButton'
    //
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	BUTTON_SetFont(hItem, GUI_FONT_32B_1);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cdOpenDoorButton);
    //
    // Initialization of 'CloseDoorButton'
    //
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetFont(hItem, GUI_FONT_32B_1);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cdCloseDoorButton);
    //
    // Initialization of 'OpenwinButton'
    //
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetFont(hItem, GUI_FONT_32B_ASCII);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cdOpenWinButton);
    //
    // Initialization of 'CloseWinButton'
    //
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
	BUTTON_SetFont(hItem, GUI_FONT_32B_1);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cdCloseWinButton);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
	WM_SetHasTrans(hItem); 
	WM_SetCallback(hItem, _cbButtonReturn);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
	case WM_PAINT:
		GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  600,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
			break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'OpenDoorButton'
    switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
    
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'CloseDoorButton'
      switch(NCode) 
	  {
		case WM_NOTIFICATION_RELEASED:
      
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'OpenwinButton'
      switch(NCode) 
	  {
		case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'CloseWinButton'
      switch(NCode) 
	  {
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Button'
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


void App_DoorWinDlg(WM_HWIN hWin) 
{
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbAppDoorWinDialog, hWin, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
