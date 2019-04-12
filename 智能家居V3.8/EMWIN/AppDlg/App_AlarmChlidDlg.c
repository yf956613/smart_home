/*
*********************************************************************************************************
*	                                  
*	ģ������ : ����������ʱ���ƽ������
*	�ļ����� : App_AlarmChildDlg.c
*	��    �� : V1.0
*	˵    �� : ����������ʱ����
*            ͨ��Spinbox�ؼ������ö�ʱʱ��
*						 ͨ����ѡ�򱣴涨ʱ����
*            ͨ��������ʱ��ť��ʼ��ʱ
*            ͨ��ȡ����ʱ��ťȡ����ʱ 
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2018-12-4    Ares 	       �װ�   
*                                     
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/
#include "DIALOG.h"
#include "stdio.h"
#include "MyGUI.h"
#include "includes.h"
#include "led.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_0    (GUI_ID_USER + 0x0C)
#define ID_TEXT_0    (GUI_ID_USER + 0x0D)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x0E)
#define ID_TEXT_1    (GUI_ID_USER + 0x0F)
#define ID_BUTTON_1    (GUI_ID_USER + 0x12)
#define ID_TEXT_2    (GUI_ID_USER + 0x13)
#define ID_BUTTON_2    (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x15)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x16)




static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "AlaTimerWindow", ID_WINDOW_0, 0, 0, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "xxxTimerText", ID_TEXT_0, 319, 58, 375, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 439, 220, 144, 37, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TimeSetText", ID_TEXT_1, 260, 223, 161, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "EnterButton", ID_BUTTON_1, 200, 434, 189, 59, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "MinText", ID_TEXT_2, 601, 223, 131, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CancleButton", ID_BUTTON_2, 648, 434, 189, 59, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 318, 336, 213, 32, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 630, 330, 219, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

static unsigned int g_u32TimerToSet;   //���SPINBox���õ�ʱ��
OS_TMR 	g_Alarmtmr3;		//��ʱ��2
void g_Alarmtmr3_callback(void *p_tmr, void *p_arg); 	//������ʱ���ص�����
char g_bAlarmTimerFlag = 0;
static unsigned int g_CountSec = 0;  //�����������ڶ�ʱ�������˶�����
unsigned char g_ucAlarmStatus = 0;  //�����������ر�־״̬

void InitAlarmChildDlg(WM_MESSAGE * pMsg)
{
}


static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	OS_ERR  err;


	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonReturn);
			//
			// Initialization of 'xxxTimerText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "����������ʱ����");
			//
			// Initialization of 'Spinbox'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
		SPINBOX_SetFont(hItem, GUI_FONT_32B_1);
		SPINBOX_SetEdge(hItem,SPINBOX_EDGE_CENTER);
		SPINBOX_SetRange(hItem, 0, 360);  //����ѡֵ�ô�С��Χ
	//	SPINBOX_SetFont(hItem, &GUI_FontHZ24);
			//
			// Initialization of 'TimeSetText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetText(hItem, "��ʱʱ�䣺");
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			//
			// Initialization of 'SaveCheckbox'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
		CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
		CHECKBOX_SetText(hItem, "��ʱ��");
			
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
		CHECKBOX_SetFont(hItem, &GUI_FontHZ24);
		CHECKBOX_SetText(hItem, "��ʱ��");
		
			//
			// Initialization of 'EnterButton'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "������ʱ");
			//
			// Initialization of 'MinText'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont(hItem, &GUI_FontHZ32);
		TEXT_SetText(hItem, "����");
			//
			// Initialization of 'CancleButton'
			//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "ȡ����ʱ");
			
		break;
  case WM_PAINT:
	GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
					  0,			   /* ���Ͻ�Y λ�� */
					  1024,	   	       /* ���½�X λ�� */
					  600,  	   	   /* ���½�Y λ�� */
					  GUI_ORANGE,	   /* ���������Ҫ���Ƶ���ɫ */
					  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
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
				GUI_EndDialog(pMsg->hWin, 0);  //�رնԻ���
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
				g_u32TimerToSet = SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0));  //��ȡ��ʱʱ��
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
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))  //���ѡ���˶�ʱ��
				{
					CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);   //ȡ��ѡ�ж�ʱ��
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
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))  //���ѡ���˶�ʱ��
				{
					CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);   //ȡ��ѡ�ж�ʱ��
				}			
				break;
     
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'EnterButton'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
					{
						g_ucAlarmStatus = 1;
					}
					
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
					{
						g_ucAlarmStatus = 2;
					}
					if(g_u32TimerToSet > 0 && g_ucAlarmStatus > 0)  //��ʱʱ���ѡ�п���زſ�����ʱ��
					{
						g_bAlarmTimerFlag = 1;
						OSTmrCreate((OS_TMR		*)&g_Alarmtmr3,		//��ʱ��1
													(CPU_CHAR	*)"g_Alarmtmr3",		//��ʱ������
													(OS_TICK	 )0,			//20*10=200ms
													(OS_TICK	 )100,          //100*10=1000ms
													(OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
													(OS_TMR_CALLBACK_PTR)g_Alarmtmr3_callback,//��ʱ��1�ص�����
													(void	    *)0,			//����Ϊ0
													(OS_ERR	    *)&err);		//���صĴ�����		
						OSTmrStart(&g_Alarmtmr3,&err);	//������ʱ��1
						GUI_EndDialog(pMsg->hWin, 0);  //�رնԻ���
				}
        break;
     
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CancleButton'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
				if(OSTmrStateGet ((OS_TMR  *)&g_Alarmtmr3,(OS_ERR  *)&err) != OS_TMR_STATE_UNUSED) //��ʱ���Ѿ�����
				 {
						OSTmrDel((OS_TMR  *)&g_Alarmtmr3,(OS_ERR  *)&err);  //ȡ����ʱ��ɾ����ʱ��
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

void App_AlarmCtrlChlidDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

extern unsigned int AlaCountdownSec;
//��ʱ��1�Ļص�����
void g_Alarmtmr3_callback(void *p_tmr, void *p_arg)
{
	OS_ERR  err;
	g_CountSec++;
	AlaCountdownSec = g_u32TimerToSet*60 - g_CountSec;
	if(g_CountSec >= (g_u32TimerToSet*60))
	{
		g_CountSec = 0;
		g_bAlarmTimerFlag = 0; //��ʱʱ�䵽����ʱ��ֹͣ
		/* ��ʱʱ�䵽 �������û����� */
		if(g_ucAlarmStatus == 1)
		{
			LED0 = 0;
		}
		if(g_ucAlarmStatus == 2)
		{
			LED0 = 1;
		}
		
		OSTmrDel ((OS_TMR *)&g_Alarmtmr3,(OS_ERR  *)&err);  //��ʱʱ�䵽,ɾ����ʱ��
	}
}



