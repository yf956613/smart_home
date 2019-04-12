/*
*********************************************************************************************************
*	                                  
*	模块名称 : 时钟日历app界面设计
*	文件名称 : App_ClockSetDlg.c
*	版    本 : V1.0
*	说    明 : 时钟日历界面
*            显示时分秒，星期、年月日
*            可以设置时分秒 年月日
*              
*		版本号   日期         作者          说明
*		V3.8    2018-1-9    Ares 	         
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/
#include "DIALOG.h"
#include "MyGUI.h"
#include "includes.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x01)
#define ID_SPINBOX_1    (GUI_ID_USER + 0x02)
#define ID_SPINBOX_2    (GUI_ID_USER + 0x03)
#define ID_TEXT_0    (GUI_ID_USER + 0x07)
#define ID_TEXT_1    (GUI_ID_USER + 0x09)
#define ID_TEXT_2    (GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x0E)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x0F)
#define ID_TEXT_3    (GUI_ID_USER + 0x10)
#define ID_TEXT_4    (GUI_ID_USER + 0x11)
#define ID_BUTTON_0    (GUI_ID_USER + 0x12)
#define ID_BUTTON_1    (GUI_ID_USER + 0x13)
#define ID_BUTTON_2    (GUI_ID_USER + 0x14)


static const GUI_WIDGET_CREATE_INFO _aAppClockSetDialogCreate[] = {
{ WINDOW_CreateIndirect, "Win", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
 { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 251, 240, 130, 40, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 249, 321, 130, 40, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 251, 400, 130, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Hour", ID_TEXT_0, 145, 241, 89, 29, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Min", ID_TEXT_1, 135, 321, 105, 32, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Sec", ID_TEXT_2, 137, 401, 97, 29, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "SaveHMS", ID_CHECKBOX_0, 251, 480, 199, 25, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "SaveYMD", ID_CHECKBOX_1, 616, 479, 224, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "HMS", ID_TEXT_3, 354, 52, 450, 38, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "YMD", ID_TEXT_4, 354, 129, 365, 37, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Return", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Save", ID_BUTTON_1, 843, 271, 80, 47, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancle", ID_BUTTON_2, 843, 385, 80, 47, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/* 用于星期的显示 */
static const char *ucWeekDay[7] =
{
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
	"日"
};

const char *apDays[] =
{
	"六",
	"日",
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
};

const char *apMonths[]=
{
	"1月",
	"2月",
	"3月",
	"4月",
	"5月",
	"6月",
	"7月",
	"8月",
	"9月",
	"10月",
	"11月",
	"12月",
};

extern RTC_HandleTypeDef RTC_Handler;  //RTC中断句柄
extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef  RTC_DateStructure;


extern void _cbButtonReturn(WM_MESSAGE * pMsg);
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day);
/*
*********************************************************************************************************
*	函 数 名: Clock_Update
*	功能说明: 更新时间
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void Clock_Update(WM_MESSAGE * pMsg) 
{
	char buf[30];
	WM_HWIN hWin = pMsg->hWin;

	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);

	/* 更新时间 */
	sprintf(buf, 
	          "时间: %0.2d:%0.2d:%0.2d  星期：%0.3s", 
			  RTC_TimeStructure.Hours, 
			  RTC_TimeStructure.Minutes,
			  RTC_TimeStructure.Seconds,
				ucWeekDay[RTC_DateStructure.WeekDay-1]);
	TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_3), buf);
 
	/* 更新日期 */
	sprintf(buf, 
	      "日期: 20%0.2d/%0.2d/%0.2d", 
		  RTC_DateStructure.Year, 
		  RTC_DateStructure.Month, 
		  RTC_DateStructure.Date);
	TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_4), buf); 

}


/*
*********************************************************************************************************
*	函 数 名: InitAppCLockSetDlg
*	功能说明: 初始化时间设置对话框对话框
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitAppCLockSetDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	/* 日历控件初始化 */
	CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 35 );
	CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 40 );
	CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 28 );

	CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT, &GUI_FontHZ16);
	CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, &GUI_FontHZ16); 
	CALENDAR_SetDefaultBkColor(CALENDAR_CI_WEEKDAY, 0xFF00FF);
	
	CALENDAR_SetDefaultDays(apDays);
	CALENDAR_SetDefaultMonths(apMonths);


//	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	CALENDAR_Create(pMsg->hWin, 
									 500, 
									 225, 
									 RTC_DateStructure.Year+2000, 
									 RTC_DateStructure.Month, 
									 RTC_DateStructure.Date, 
										2, 
									GUI_ID_CALENDAR0, 
									WM_CF_SHOW);
	
	
	WM_CreateTimer(pMsg->hWin, 0, 10, 0);	
	// Initialization of 'Hour'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	// Initialization of 'Min'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	// Initialization of 'Sec'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem, GUI_FONT_24B_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	;

	// Initialization of 'HMS'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
	TEXT_SetFont(hItem, &GUI_FontHZ32);

	// Initialization of 'YMD'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	
	// Initialization of 'Save
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetText(hItem, "确定");

	// Initialization of 'Cancle'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetText(hItem, "取消");
	
	//时选值框初始化
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 23);
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);

	//分选值框初始化
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 59);  //设置选值得大小范围
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);

	//秒选值框初始化
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 59);  //设置选值得大小范围
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "时 ：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "分 ：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "秒 ：");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "保存时间设置"); 
	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "保存年月日设置"); 
		
	
	/* 设置关闭按钮的透明标记和回调函数 */
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
	
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_0), RTC_TimeStructure.Hours);   /* 时 */								   
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_1), RTC_TimeStructure.Minutes); /* 分 */							   
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_2), RTC_TimeStructure. Seconds); /* 秒 */
}


static void _cbAppClockSetDialog(WM_MESSAGE * pMsg) 
{
//   WM_HWIN hItem;
  int     NCode;
  int     Id;
	CALENDAR_DATE CALENDAR_Date;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitAppCLockSetDlg(pMsg);
			break;
		case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  600,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
			break;
		case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
			break;
	
		case WM_POST_PAINT:
			GUI_MULTIBUF_End();
			break;	
		
		/* 定时1秒更新一次时间 */
		case WM_TIMER:
//			WM_InvalidateRect(pMsg->hWin, &rRTC);  //使指定的窗口无效，就会执行WM_PAINT消息
			Clock_Update(pMsg);
			WM_RestartTimer(pMsg->Data.v, 1000);  //重启定时器
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
				switch(Id) 
				{	
					case ID_BUTTON_0: // Notifications sent by 'Return'
						switch(NCode) 
						{
							case WM_NOTIFICATION_RELEASED:
								GUI_EndDialog(pMsg->hWin, 0);  //关闭时间设置对话框
								break;
						}
						break;
					case ID_BUTTON_1: // Notifications sent by 'Save'
						switch(NCode) 
						{
							case WM_NOTIFICATION_CLICKED:
							
								break;
							case WM_NOTIFICATION_RELEASED:
								if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
								{
									RTC_TimeStructure.Hours   = SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0));
									RTC_TimeStructure.Minutes = SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1));
									RTC_TimeStructure.Seconds = SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2));
									HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	
								}
								
								if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
								{
									CALENDAR_GetSel(WM_GetDialogItem(pMsg->hWin,GUI_ID_CALENDAR0), &CALENDAR_Date);
								
									RTC_DateStructure.Year = CALENDAR_Date.Year%100;
									RTC_DateStructure.Month = CALENDAR_Date.Month;
									RTC_DateStructure.Date = CALENDAR_Date.Day;
									RTC_DateStructure.WeekDay = RTC_CalcWeek(RTC_DateStructure.Year, 
																			                     RTC_DateStructure.Month,
																			                     RTC_DateStructure.Date);
									HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
								}
								break;
				
						}
						break;
					case ID_BUTTON_2: // Notifications sent by 'Cancle'
						switch(NCode) 
						{
							case WM_NOTIFICATION_RELEASED:
								GUI_EndDialog(pMsg->hWin, 0);  //关闭时间设置对话框
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


void App_ClockSetDlg(WM_HWIN hWin) 
{
  hWin = GUI_CreateDialogBox(_aAppClockSetDialogCreate, GUI_COUNTOF(_aAppClockSetDialogCreate), _cbAppClockSetDialog, hWin, 0, 0);
}

