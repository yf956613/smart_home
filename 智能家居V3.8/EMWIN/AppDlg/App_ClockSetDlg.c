/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʱ������app�������
*	�ļ����� : App_ClockSetDlg.c
*	��    �� : V1.0
*	˵    �� : ʱ����������
*            ��ʾʱ���룬���ڡ�������
*            ��������ʱ���� ������
*              
*		�汾��   ����         ����          ˵��
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

/* �������ڵ���ʾ */
static const char *ucWeekDay[7] =
{
	"һ",
	"��",
	"��",
	"��",
	"��",
	"��",
	"��"
};

const char *apDays[] =
{
	"��",
	"��",
	"һ",
	"��",
	"��",
	"��",
	"��",
	"��",
};

const char *apMonths[]=
{
	"1��",
	"2��",
	"3��",
	"4��",
	"5��",
	"6��",
	"7��",
	"8��",
	"9��",
	"10��",
	"11��",
	"12��",
};

extern RTC_HandleTypeDef RTC_Handler;  //RTC�жϾ��
extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef  RTC_DateStructure;


extern void _cbButtonReturn(WM_MESSAGE * pMsg);
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day);
/*
*********************************************************************************************************
*	�� �� ��: Clock_Update
*	����˵��: ����ʱ��
*	��    ��: pMsg  ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Clock_Update(WM_MESSAGE * pMsg) 
{
	char buf[30];
	WM_HWIN hWin = pMsg->hWin;

	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);

	/* ����ʱ�� */
	sprintf(buf, 
	          "ʱ��: %0.2d:%0.2d:%0.2d  ���ڣ�%0.3s", 
			  RTC_TimeStructure.Hours, 
			  RTC_TimeStructure.Minutes,
			  RTC_TimeStructure.Seconds,
				ucWeekDay[RTC_DateStructure.WeekDay-1]);
	TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_3), buf);
 
	/* �������� */
	sprintf(buf, 
	      "����: 20%0.2d/%0.2d/%0.2d", 
		  RTC_DateStructure.Year, 
		  RTC_DateStructure.Month, 
		  RTC_DateStructure.Date);
	TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_4), buf); 

}


/*
*********************************************************************************************************
*	�� �� ��: InitAppCLockSetDlg
*	����˵��: ��ʼ��ʱ�����öԻ���Ի���
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitAppCLockSetDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	/* �����ؼ���ʼ�� */
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
	BUTTON_SetText(hItem, "ȷ��");

	// Initialization of 'Cancle'
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetText(hItem, "ȡ��");
	
	//ʱѡֵ���ʼ��
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 23);
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);

	//��ѡֵ���ʼ��
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 59);  //����ѡֵ�ô�С��Χ
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);

	//��ѡֵ���ʼ��
	hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
	SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
	SPINBOX_SetRange(hItem, 0, 59);  //����ѡֵ�ô�С��Χ
	SPINBOX_SetFont(hItem, &GUI_FontHZ24);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "ʱ ��");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "�� ��");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem,&GUI_FontHZ24);
	TEXT_SetText(hItem, "�� ��");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "����ʱ������"); 
	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
	CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
	CHECKBOX_SetText(hItem, "��������������"); 
		
	
	/* ���ùرհ�ť��͸����Ǻͻص����� */
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
	
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_0), RTC_TimeStructure.Hours);   /* ʱ */								   
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_1), RTC_TimeStructure.Minutes); /* �� */							   
	SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin,ID_SPINBOX_2), RTC_TimeStructure. Seconds); /* �� */
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
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  600,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
			break;
		case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
			break;
	
		case WM_POST_PAINT:
			GUI_MULTIBUF_End();
			break;	
		
		/* ��ʱ1�����һ��ʱ�� */
		case WM_TIMER:
//			WM_InvalidateRect(pMsg->hWin, &rRTC);  //ʹָ���Ĵ�����Ч���ͻ�ִ��WM_PAINT��Ϣ
			Clock_Update(pMsg);
			WM_RestartTimer(pMsg->Data.v, 1000);  //������ʱ��
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
								GUI_EndDialog(pMsg->hWin, 0);  //�ر�ʱ�����öԻ���
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
								GUI_EndDialog(pMsg->hWin, 0);  //�ر�ʱ�����öԻ���
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

