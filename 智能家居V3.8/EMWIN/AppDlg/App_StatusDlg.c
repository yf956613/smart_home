/*
*********************************************************************************************************
*	                                  
*	模块名称 : 系统任务栏设计
*	文件名称 : App_SocketCtrlDlg.c
*	版    本 : V1.0
*	说    明 : 系统任务栏界面
*              1. 显示CPU使用率，时间日期，
*              2. 显示电量，网络信号等等
*							 .......................
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
#include "includes.h"

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_TEXT_0     (GUI_ID_USER + 0x01)
#define ID_TEXT_1     (GUI_ID_USER + 0x02)
#define ID_TEXT_2     (GUI_ID_USER + 0x03)

WM_HWIN hWinStatus;

static const GUI_WIDGET_CREATE_INFO _aSysInfoDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 565, 1024, 35, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 890, 2, 130, 18, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 890, 17, 120, 18, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "CPU", ID_TEXT_2, 680, 12, 85, 19, 0, 0x0, 0 },
};

/* 用于星期的显示 */
const char ucWeekDay[7][3] =
{
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

extern RTC_HandleTypeDef RTC_Handler;  //RTC句柄
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_DateTypeDef  RTC_DateStructure;

static void Caculate_RTC(WM_MESSAGE * pMsg)
{
	char buf[30];
	WM_HWIN hWin = pMsg->hWin;
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
	sprintf(buf, 
					"%0.2d:%0.2d:%0.2d %0.3s", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes,
			RTC_TimeStructure.Seconds, 
			ucWeekDay[RTC_DateStructure.WeekDay-1]);
 TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_0), buf);

	sprintf(buf, 
					"20%0.2d/%0.2d/%0.2d", 
			RTC_DateStructure.Year, 
			RTC_DateStructure.Month, 
			RTC_DateStructure.Date);
	TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_1), buf); 	

}

/*
*********************************************************************************************************
*	函 数 名: PaintDialogSysInfo
*	功能说明: 重绘函数 
*	形    参: pMsg  WM_MESSAGE类型指针变量  
*	返 回 值: 无
*********************************************************************************************************
*/
static void PaintDialogSysInfo(WM_MESSAGE * pMsg)
{

	int        xSize;
	int        ySize;
	static int Reception = 5; 
	int        xPos;
	int        i;
	
	char buf[20];

    xSize = WM_GetWindowSizeX(pMsg->hWin);    //得到任务栏的大小
    ySize = WM_GetWindowSizeY(pMsg->hWin);	
	
	GUI_SetColor(0x303030);  //设置任务栏颜色
    GUI_FillRect(0, 2, xSize - 1, ySize - 1);
	
    GUI_SetColor(0x808080);
    GUI_DrawHLine(0, 0, xSize - 1);
	
    GUI_SetColor(0x404040);
    GUI_DrawHLine(1, 0, xSize - 1);
	
	GUI_DrawBitmap(&_bmBatteryEmpty_27x14, 855,10); //绘制电池任务栏图标
  GUI_DrawBitmap(_apbmCharge[4], 855, 10); //画出电池的电量的进度条
	
	/* WIFI信号绘制 */
	GUI_SetColor(GUI_WHITE);  
	for (xPos = 810, i = 0; i < Reception; i++)  /* 绘制有信号的格数 */
	{
		GUI_DrawVLine(xPos + i * 4 + 0, 23 - i * 2, 25);
		GUI_DrawVLine(xPos + i * 4 + 1, 23 - i * 2, 25);
	}
	
	GUI_SetColor(GUI_GRAY);
	for (; i < 8; i++)        /* 绘制信号总格数 */
	{     
		GUI_DrawVLine(xPos + i * 4 + 0, 23 - i * 2, 25);
		GUI_DrawVLine(xPos + i * 4 + 1, 23 - i * 2, 25);
	}
	
	/* 闹钟绘制 */
	GUI_DrawBitmap(&_bmAlarm_16x16, 780, 14);
	
	sprintf(buf,"CPU: %3.2f%%",(float)OSStatTaskCPUUsage/100);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_2), buf); 
}


/*
*********************************************************************************************************
*	函 数 名: InitDialogSysInfo
*	功能说明: 初始化系统状态栏 
*	形    参: pMsg  WM_MESSAGE类型指针变量  
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogSysInfo(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	WM_HWIN hWin = pMsg->hWin; 

	/* 设置系统对话框背景颜色*/
	WINDOW_SetBkColor(hWin, GUI_BLACK);

	WM_CreateTimer(hWin, 0, 1000, 0);//创建定时器，1000ms定时
	WM_CreateTimer(hWin, 1, 2000, 0);
	
  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetBkColor(hItem, 0x303030);
	TEXT_SetTextColor(hItem, GUI_WHITE);
  TEXT_SetFont(hItem, GUI_FONT_16B_1);
  TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
   
  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetBkColor(hItem, 0x303030);  //0x303030
	TEXT_SetTextColor(hItem, GUI_WHITE);
  TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
  TEXT_SetFont(hItem, GUI_FONT_16B_1);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetBkColor(hItem, 0x303030);  //0x303030
	TEXT_SetTextColor(hItem, GUI_WHITE);
//    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
  TEXT_SetFont(hItem, GUI_FONT_16B_1);

	Caculate_RTC(pMsg);//计算RTC时间
 
}


static void _cbDialogOfSysInfo(WM_MESSAGE * pMsg) 
{
//  WM_HWIN hItem;
	char buf[10];
	char TimerId;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitDialogSysInfo(pMsg);
			break;
	  /* 定时器更新 */
		case WM_TIMER:
			TimerId = WM_GetTimerId(pMsg->Data.v);
			if(TimerId)
			{
				sprintf(buf,"CPU: %3.2f%%",(float)OSStatTaskCPUUsage/100);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_2), buf); 
				WM_RestartTimer(pMsg->Data.v, 2000);  //重启定时器
			}
			else
			{
				Caculate_RTC(pMsg);
				WM_RestartTimer(pMsg->Data.v, 1000);  //重启定时器
			}
			break;
		case WM_PAINT:
			PaintDialogSysInfo(pMsg);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}



void CreateStatusDlg(void) 
{
  hWinStatus = GUI_CreateDialogBox(_aSysInfoDialogCreate, GUI_COUNTOF(_aSysInfoDialogCreate), _cbDialogOfSysInfo, WM_HBKWIN, 0, 0);
}


