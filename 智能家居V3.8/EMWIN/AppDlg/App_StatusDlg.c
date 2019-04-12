/*
*********************************************************************************************************
*	                                  
*	ģ������ : ϵͳ���������
*	�ļ����� : App_SocketCtrlDlg.c
*	��    �� : V1.0
*	˵    �� : ϵͳ����������
*              1. ��ʾCPUʹ���ʣ�ʱ�����ڣ�
*              2. ��ʾ�����������źŵȵ�
*							 .......................
*              
*		�汾��   ����         ����          ˵��
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

/* �������ڵ���ʾ */
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

extern RTC_HandleTypeDef RTC_Handler;  //RTC���
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
*	�� �� ��: PaintDialogSysInfo
*	����˵��: �ػ溯�� 
*	��    ��: pMsg  WM_MESSAGE����ָ�����  
*	�� �� ֵ: ��
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

    xSize = WM_GetWindowSizeX(pMsg->hWin);    //�õ��������Ĵ�С
    ySize = WM_GetWindowSizeY(pMsg->hWin);	
	
	GUI_SetColor(0x303030);  //������������ɫ
    GUI_FillRect(0, 2, xSize - 1, ySize - 1);
	
    GUI_SetColor(0x808080);
    GUI_DrawHLine(0, 0, xSize - 1);
	
    GUI_SetColor(0x404040);
    GUI_DrawHLine(1, 0, xSize - 1);
	
	GUI_DrawBitmap(&_bmBatteryEmpty_27x14, 855,10); //���Ƶ��������ͼ��
  GUI_DrawBitmap(_apbmCharge[4], 855, 10); //������صĵ����Ľ�����
	
	/* WIFI�źŻ��� */
	GUI_SetColor(GUI_WHITE);  
	for (xPos = 810, i = 0; i < Reception; i++)  /* �������źŵĸ��� */
	{
		GUI_DrawVLine(xPos + i * 4 + 0, 23 - i * 2, 25);
		GUI_DrawVLine(xPos + i * 4 + 1, 23 - i * 2, 25);
	}
	
	GUI_SetColor(GUI_GRAY);
	for (; i < 8; i++)        /* �����ź��ܸ��� */
	{     
		GUI_DrawVLine(xPos + i * 4 + 0, 23 - i * 2, 25);
		GUI_DrawVLine(xPos + i * 4 + 1, 23 - i * 2, 25);
	}
	
	/* ���ӻ��� */
	GUI_DrawBitmap(&_bmAlarm_16x16, 780, 14);
	
	sprintf(buf,"CPU: %3.2f%%",(float)OSStatTaskCPUUsage/100);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_2), buf); 
}


/*
*********************************************************************************************************
*	�� �� ��: InitDialogSysInfo
*	����˵��: ��ʼ��ϵͳ״̬�� 
*	��    ��: pMsg  WM_MESSAGE����ָ�����  
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitDialogSysInfo(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	WM_HWIN hWin = pMsg->hWin; 

	/* ����ϵͳ�Ի��򱳾���ɫ*/
	WINDOW_SetBkColor(hWin, GUI_BLACK);

	WM_CreateTimer(hWin, 0, 1000, 0);//������ʱ����1000ms��ʱ
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

	Caculate_RTC(pMsg);//����RTCʱ��
 
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
	  /* ��ʱ������ */
		case WM_TIMER:
			TimerId = WM_GetTimerId(pMsg->Data.v);
			if(TimerId)
			{
				sprintf(buf,"CPU: %3.2f%%",(float)OSStatTaskCPUUsage/100);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ID_TEXT_2), buf); 
				WM_RestartTimer(pMsg->Data.v, 2000);  //������ʱ��
			}
			else
			{
				Caculate_RTC(pMsg);
				WM_RestartTimer(pMsg->Data.v, 1000);  //������ʱ��
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


