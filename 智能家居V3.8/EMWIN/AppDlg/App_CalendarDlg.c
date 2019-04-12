/*
*********************************************************************************************************
*	                                  
*	ģ������ : �������������
*	�ļ����� : App_CalendarDlg.c
*	��    �� : V1.0
*	˵    �� : ����������
*              1. GUI���������Ͻǵİ�ť���ڵ���ʱ�䣬���ں��������ý��棬���Ͻǵİ�ťδ�õ����û���ֲ�������
*                 �Ļ����˰�ť���ڷ���ϵͳ�����档�����������Ӷ����ڶԻ�������ʵ�ֵģ���ֲ�ܷ��㡣
*              2. ����ʱ�䵽�󣬷���������5����
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-12-23   Eric2013  	    �װ�   
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "DIALOG.h"
#include "MyGUI.h"
#include "includes.h"
#include "stm32f4xx.h"
/*
*********************************************************************************************************
*                            �ⲿ�ļ��ж��壬�����ļ�ʹ��
*********************************************************************************************************
*/
extern void App_SetTimeAlarm(void);
extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef  RTC_DateStructure;
extern RTC_AlarmTypeDef RTC_AlarmStructure;
extern RTC_HandleTypeDef RTC_Handler;  //RTC���

extern GUI_CONST_STORAGE GUI_BITMAP bmClockIcon;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock;
/*
*********************************************************************************************************
*                                       �궨��
*********************************************************************************************************
*/
#define countof(Obj) (sizeof(Obj) / sizeof(Obj[0]))
#define DEG2RAD      (3.1415926f / 180)
#define MAG          3

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_1   (GUI_ID_USER + 0x02)


/*
*********************************************************************************************************
*                                       ��ֵ
*********************************************************************************************************
*/ 
typedef struct {
  GUI_POINT        aPoints[7];   /* ��������� */
  float            Angle;        /* ��ת�Ƕ� */
  
  GUI_POINT        aPoints1[7];   
  float            Angle1;
	
  GUI_POINT        aPoints2[7];   
  float            Angle2;
} PARAM;

/* ����ָ������ */
static const GUI_POINT _aNeedle[] = 
{
	{ MAG * ( -1), MAG * (  0 ) },
	{ MAG * (-1), MAG * (-50 ) },
	{ MAG * (0), MAG * (-65 ) },
	{ MAG * ( 1), MAG * (-50 ) },
	{ MAG * ( 1), MAG * (0 ) },
};

static const GUI_POINT _aNeedle1[] = 
{
	{ MAG * ( -2), MAG * (  0 ) },
	{ MAG * (-2), MAG * (-50 ) },
	{ MAG * (0), MAG * (-65 ) },
	{ MAG * ( 2), MAG * (-50 ) },
	{ MAG * ( 2), MAG * (0 ) },
};

static const GUI_POINT _aNeedle2[] = 
{
	{ MAG * ( -2), MAG * (  0 ) },
	{ MAG * (-2), MAG * (-40 ) },
	{ MAG * (0), MAG * (-55 ) },
	{ MAG * ( 2), MAG * (-40 ) },
	{ MAG * ( 2), MAG * (0 ) },
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


/*
*********************************************************************************************************
*	�� �� ��: _cbButtonList
*	����˵��: ��ť�Զ���ص�����
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbButtonList(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(0, 0, 72*MAG);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmClockIcon, 2, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72*MAG);
				GUI_SetBkColor(GUI_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmClockIcon, 2, 5);
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Clock_DrawDisp
*	����˵��: ����ʱ�ӱ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Clock_DrawDisp(void) 
{
	PARAM       Param;        /* PARAM���ͱ��� */

    int      t0;   /* ��������ָ��ļ��� */
	int      t1;
    int      t2;	
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
	
	/* ��һ��ָ�������������ת���� */
	t0 = 360 - RTC_TimeStructure.Seconds * 6;

	/* �ڶ���ָ�������������ת���� */
	t1 = 360 - RTC_TimeStructure.Minutes * 6;
	
	/* ������ָ�������������תʱ�� */
	t2 = 360 - RTC_TimeStructure.Hours * 30;

	/* ��ת��һ��ָ�� */
	Param.Angle= t0;
	Param.Angle *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);

	/* ��ת�ڶ���ָ�� */
	Param.Angle1= t1;
	Param.Angle1 *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);

	/* ��ת������ָ�� */
	Param.Angle2= t2;
	Param.Angle2 *= 3.1415926f / 180;
	GUI_RotatePolygon(Param.aPoints2, _aNeedle2, countof(_aNeedle2), Param.Angle2);
						

	GUI_DrawBitmap(&bmclock, 310, 120);
		
	/* ʹ�ܿ���ݣ������������£��˺���һ��Ҫʵʱ���ã������ͼ�����л������ͳ����� */
	GUI_AA_EnableHiRes();
	
	GUI_SetColor(GUI_RED);
	GUI_AA_FillPolygon(Param.aPoints, countof(_aNeedle), MAG * 400, MAG * 210);
	GUI_AA_FillPolygon(Param.aPoints1, countof(_aNeedle1), MAG * 400, MAG * 210);
	GUI_AA_FillPolygon(Param.aPoints2, countof(_aNeedle2), MAG * 400, MAG * 210);
}

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
	//HAL_RTC_GetAlarm(&RTC_Handler, RTC_Alarm_A, &RTC_AlarmStructure);

	/* ����ʱ�� */
	sprintf(buf, 
	          "ʱ��: %0.2d:%0.2d:%0.2d ", 
			  RTC_TimeStructure.Hours, 
			  RTC_TimeStructure.Minutes,
			  RTC_TimeStructure.Seconds);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1), buf);
 
	/* �������� */
	sprintf(buf, 
	      "����: 20%0.2d/%0.2d/%0.2d", 
		  RTC_DateStructure.Year, 
		  RTC_DateStructure.Month, 
		  RTC_DateStructure.Date);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0), buf); 
	
	/* �������� */
	sprintf(buf, "����: %s", ucWeekDay[RTC_DateStructure.WeekDay-1]);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT2), buf);
	
//	/* �������� */
//	sprintf(buf, 
//	      "����: %0.2d:%0.2d:%0.2d ", 
//		  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, 
//		  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, 
//		  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
//	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT3), buf); 
}

/*
*********************************************************************************************************
*	               �Ի���ؼ��б�
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateRTC[] = {
	{ WINDOW_CreateIndirect,  "Window",    ID_WINDOW_0,     0,   0,  800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect,      "����",    GUI_ID_TEXT0,    270,   60, 200, 20, 0,0},
    { TEXT_CreateIndirect,      "ʱ��",    GUI_ID_TEXT1,    420,   60, 200, 20, 0,0},
	{ TEXT_CreateIndirect,      "����",    GUI_ID_TEXT2,    270,   80, 200, 20, 0,0},
    { TEXT_CreateIndirect,      "����",    GUI_ID_TEXT3,    420,   80, 200, 20, 0,0},

	{ BUTTON_CreateIndirect, "DlgBack",    ID_BUTTON_0,  700,  0, 100, 100, 0, 0, 0 },	
	{ BUTTON_CreateIndirect, "TimeSet",    ID_BUTTON_1,    0,  0, 100, 100, 0, 0, 0 },
};

/*
*********************************************************************************************************
*	�� �� ��: InitDialogRTC
*	����˵��: �Ի���ص������ĳ�ʼ����Ϣ
*	��    ��: pMsg   ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogRTC(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hItem;
	
	WM_CreateTimer(hWin, 0, 10, 0);						
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonBack);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonList);
	
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_FontHZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_FontHZ16);
	Clock_Update(pMsg);
	
	/* ʹ�ܿ���� */
	GUI_AA_EnableHiRes();
	
	/* ���ÿ�������� */
	GUI_AA_SetFactor(MAG);
}

/*
*********************************************************************************************************
*	�� �� ��: _cbCallbackRTC
*	����˵��: �ص�����
*	��    ��: pMsg   ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbCallbackRTC(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
 	GUI_RECT rRTC={310, 120, 490, 300};
	
    switch (pMsg->MsgId) 
    {
		case WM_INIT_DIALOG:
            InitDialogRTC(pMsg);
            break;
				
		case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
			break;
		
		case WM_PAINT:
			Clock_DrawDisp();
            break;
		
		case WM_POST_PAINT:
			GUI_MULTIBUF_End();
			break;	
		
		/* ��ʱ1�����һ��ʱ�� */
		case WM_TIMER:
			WM_InvalidateRect(hWin, &rRTC);
			Clock_Update(pMsg);
		    WM_RestartTimer(pMsg->Data.v, 1000);
	        break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
				/* �رնԻ��� */
				case ID_BUTTON_0:
                    switch(NCode)
                    {
						case WM_NOTIFICATION_RELEASED:
							GUI_AA_DisableHiRes();
                            GUI_EndDialog(hWin, 0);
                            break;
                    }
                    break;
					
				/* ����ʱ�䣬���ں����� */				
				case ID_BUTTON_1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
//                            App_CalendarChild(hWin);
                            break;
                    }
                    break;
            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: App_Calendar
*	����˵��: �������Ի���
*	��    ��: hWin �������Ի��򸸴��� 	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Calendar(WM_HWIN hWin) 
{
	GUI_CreateDialogBox(_aDialogCreateRTC, 
	                    GUI_COUNTOF(_aDialogCreateRTC), 
	                    &_cbCallbackRTC, 
	                    hWin, 
	                    0, 
	                    0);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
