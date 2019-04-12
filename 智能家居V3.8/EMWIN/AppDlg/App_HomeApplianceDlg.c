#include "DIALOG.h"
#include "MyGUI.h"
#include "string.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x02)
#define ID_BUTTON_1    (GUI_ID_USER + 0x03)
#define ID_TEXT_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_2    (GUI_ID_USER + 0x05)
#define ID_BUTTON_3    (GUI_ID_USER + 0x07)
#define ID_TEXT_1    (GUI_ID_USER + 0x08)


#define OPEN_SYS       0
#define UP_TEMP        1
#define DOWN_TEMP      2
#define CLOSE_SYS      3


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "HomeApplianceWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 172, 136, 244, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenButton", ID_BUTTON_1, 518, 348, 146, 49, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 576, 165, 245, 117, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "JieMaButton", ID_BUTTON_2, 737, 349, 146, 49, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimerButton", ID_BUTTON_3, 0, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TimerText", ID_TEXT_1, 367, 41, 355, 34, 0, 0x0, 0 },
};


char g_ucNumber;
unsigned int HomeApplianceCountdownSec;

extern unsigned char g_ucHomeApplianceStatus;  //家电的状态
extern unsigned char g_bHomeApplianceTimerFlag;    //定时器启动标志

static WM_HWIN Text1hItem;
static void InitAppHomeApplianceDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);   
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbTimerButton);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetFont(hItem,&GUI_FontHZ24);
	BUTTON_SetText(hItem, "打开控制界面");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetFont(hItem,&GUI_FontHZ24);
	BUTTON_SetText(hItem, "红外解码");


	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
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
	
	DROPDOWN_SetItemSpacing(hItem, 50);   //设置元素之间的间距
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //自动使用滚动条
	DROPDOWN_SetListHeight(hItem, 250);
	DROPDOWN_SetScrollbarWidth(hItem, 30);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem,"按一次红外解码学习\n\r"
	                   "一个指令依次学习开\n\r"
										 "指令、增加温度指令\n\r"
										 "减温度指令、关指令\n\r");
										 
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	Text1hItem = hItem;
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "");
}



static void _cbDialog(WM_MESSAGE * pMsg) 
{
  int NCode;
  int Id;
  WM_HWIN hItem;
	
  char g_ucBuf[10];
	char buf[30];
	static unsigned char num = 0;

  switch (pMsg->MsgId) 
  {
  case WM_INIT_DIALOG: 
		WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //创建定时器
		InitAppHomeApplianceDlg(pMsg);
    break;
  case WM_PAINT:
		GUI_DrawGradientV(0,			   /* 左上角X 位置 */
											0,			   /* 左上角Y 位置 */
											1024,	   	       /* 右下角X 位置 */
											565,  	   	   /* 右下角Y 位置 */
											GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
											GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
	
		GUI_DrawRect(576, 155, 821, 272);  
	 
		break;
	case WM_TIMER:
		if(g_bHomeApplianceTimerFlag)
		{
			if(1 == g_ucHomeApplianceStatus)
			{
				sprintf(buf,"定时开倒计时：%d S",HomeApplianceCountdownSec);
				TEXT_SetText(Text1hItem, buf);
			}else if(2 == g_ucHomeApplianceStatus)
			{
				sprintf(buf,"定时关倒计时：%d S",HomeApplianceCountdownSec);
				TEXT_SetText(Text1hItem, buf);
			}
		}
		else
		{
			sprintf(buf,"                             ");
			TEXT_SetText(Text1hItem, buf);		
		}
		
		
//		BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2),0,LampAutoFlag?&bmON:&bmOFF,0,0);
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
					GUI_EndDialog(pMsg->hWin, 0);   //关闭对话框
					break;
		 
				}
      break;
    case ID_DROPDOWN_0: // Notifications sent by 'Dropdown'
      switch(NCode) 
			{
      case WM_NOTIFICATION_CLICKED:
      
        break;
      case WM_NOTIFICATION_RELEASED:
    
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
				g_ucNumber = DROPDOWN_GetSel(hItem);
				DROPDOWN_GetItemText(hItem, g_ucNumber,g_ucBuf,10);
        break;
  
      }
      break;
    case ID_BUTTON_1: 
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					App_HomeApplianceChildDlg(pMsg->hWin);
					break;
 
      }
      break;
		case ID_BUTTON_2: 
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
						switch(num++)
						{
							case OPEN_SYS:
								usart2_send_byte(0xAA);		
								usart2_send_byte(0x01);
								usart2_send_byte(0xFF);
								break;
							case UP_TEMP: 
									usart2_send_byte(0xAA);		
									usart2_send_byte(0x02);
									usart2_send_byte(0xFF);
								break;
							case DOWN_TEMP: 
									usart2_send_byte(0xAA);		
									usart2_send_byte(0x03);
									usart2_send_byte(0xFF);
								break;
							case CLOSE_SYS: 
									usart2_send_byte(0xAA);		
									usart2_send_byte(0x04);
									usart2_send_byte(0xFF);
									num = 0;
							break;
						}
					break;
 
      }
      break;
		 case ID_BUTTON_3: 
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					App_HomeApplianceTimerChildDlg(pMsg->hWin);
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

void App_HomeApplianceDlg(WM_HWIN hWin) 
{
   GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}
