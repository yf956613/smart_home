/*
*********************************************************************************************************
*	                                  
*	ģ������ : �ƹⶨʱ�����ӽ������
*	�ļ����� : App_SocketCtrlChildDlg.c
*	��    �� : V1.0
*	˵    �� : �ƹܶ�ʱ�����ӽ���
*              Spinbox���ö�ʱʱ��
*              Dropdown�����б�ѡ��Ƶ�
*              Checkbox��ѡ��ѡ��ʱ��or��ʱ��
*              Button��ť������ʱ��ȡ����ʱ  
*              
*		�汾��   ����         ����          ˵��
*		V1.0    2018-12-8   Ares Xu  	      �װ�   
*   Mail     1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "stdio.h"
#include "MyGUI.h"
#include "includes.h"
#include "led.h"

OS_TMR 	g_Lamptmr1;		//��ʱ��1
unsigned char g_ucLampStatus = 0;
void g_Lamptmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_0    (GUI_ID_USER + 0x02)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x03)
#define ID_TEXT_1    (GUI_ID_USER + 0x04)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x05)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_1    (GUI_ID_USER + 0x07)
#define ID_TEXT_2    (GUI_ID_USER + 0x08)
#define ID_BUTTON_2    (GUI_ID_USER + 0x09)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x0A)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "XXXTimerWindow", ID_WINDOW_0, 0, 0, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "xxxTimerText", ID_TEXT_0, 319, 58, 375, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 260, 219, 144, 37, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TimeSetText", ID_TEXT_1, 81, 222, 161, 33, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "SaveCheckbox", ID_CHECKBOX_0, 567, 223, 196, 29, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "SocketDropdown", ID_DROPDOWN_0, 260, 362, 155, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "EnterButton", ID_BUTTON_1, 746, 206, 189, 59, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "MinText", ID_TEXT_2, 420, 222, 131, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CancleButton", ID_BUTTON_2, 746, 357, 189, 59, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 567, 370, 199, 29, 0, 0x0, 0 },

};

static unsigned char g_ucLampNumber = 0;
static char g_cBuf1[6];
extern unsigned char LED_flag;
static unsigned int g_u32TimerToSet;
static unsigned int g_CountSec = 0;
unsigned char g_bLampTimerFlag = 0;

static void InitLampChildDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	int i;
  char buf[10];
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
		//
		// Initialization of 'xxxTimerText'
		//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "�ƹⶨʱ����");
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
		// Initialization of 'SocketDropdown'
		//
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	DROPDOWN_SetListHeight(hItem, 60);
	for(i = 1; i < 10 + 1; i++)
	{
		sprintf(buf,"��%d",i);
		DROPDOWN_AddString(hItem, buf);
	}
	DROPDOWN_AddString(hItem, "���е�");
	DROPDOWN_SetItemSpacing(hItem, 10);   //����Ԫ��֮��ļ��
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //�Զ�ʹ�ù�����
	DROPDOWN_SetListHeight(hItem, 150);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
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
}
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
//  int i;
 // char buf[10];
	OS_ERR err;

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			InitLampChildDlg(pMsg);
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
			case ID_CHECKBOX_0:
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
    case ID_DROPDOWN_0: // Notifications sent by 'SocketDropdown'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_CLICKED:
       
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
				g_ucLampNumber = DROPDOWN_GetSel(hItem);
				DROPDOWN_GetItemText(hItem, g_ucLampNumber,g_cBuf1,4);
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'EnterButton'
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
					{
						g_ucLampStatus = 1;
					}
					
					if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1)))
					{
						g_ucLampStatus = 2;
					}
					if(g_u32TimerToSet > 0 && g_ucLampStatus > 0)  //��ʱʱ���ѡ�п���زŴ�����ʱ��
					{
						if(OSTmrStateGet ((OS_TMR  *)&g_Lamptmr1,(OS_ERR  *)&err) == OS_TMR_STATE_UNUSED)  //�����ʱ��û�д���
						{
							g_bLampTimerFlag = 1;
							OSTmrCreate((OS_TMR		*)&g_Lamptmr1,		//��ʱ��1
														(CPU_CHAR	*)"g_Lamptmr1",		//��ʱ������
														(OS_TICK	 )0,			//���ӳ�
														(OS_TICK	 )100,  //�������ڶ�ʱʱ��Ϊ1s
														(OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
														(OS_TMR_CALLBACK_PTR)g_Lamptmr1_callback,//��ʱ��1�ص�����
														(void	    *)0,			//����Ϊ0
														(OS_ERR	    *)&err);		//���صĴ�����		
							OSTmrStart(&g_Lamptmr1,&err);	//������ʱ��1
							GUI_EndDialog(pMsg->hWin, 0);  //�رնԻ���
						}
					}
					break;
     
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CancleButton'
      switch(NCode) 
	  {
      case WM_NOTIFICATION_RELEASED:
				 if(OSTmrStateGet ((OS_TMR  *)&g_Lamptmr1,(OS_ERR  *)&err) != OS_TMR_STATE_UNUSED) //��ʱ���Ѿ�����
				 {
						OSTmrDel ((OS_TMR  *)&g_Lamptmr1,(OS_ERR  *)&err);  //ȡ����ʱ��ɾ����ʱ��
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

void App_LampCtrlChlidDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

extern unsigned int LampCountdownSec;
//����ƶ�ʱ�Ķ�ʱ���Ļص�����
void g_Lamptmr1_callback(void *p_tmr, void *p_arg)
{
	OS_ERR  err;
	g_CountSec++;
	LampCountdownSec = g_u32TimerToSet*60 - g_CountSec;
	if(g_CountSec >= (g_u32TimerToSet*60))  //��ʱʱ�䵽
	{
		g_CountSec = 0;                        //�����������
		g_bLampTimerFlag = 0;
		if(g_ucLampStatus == 1 && LED_flag==0)                  //�Ƿ��Ƕ�ʱ��
		{
			LAMP_SW = 1;
			LED_flag = 1;
		}
		if(g_ucLampStatus==2 && LED_flag)                              //��ʱ��
		{
			LAMP_SW = 0;
			LED_flag = 0;
		}
		OSTmrDel ((OS_TMR  *)&g_Lamptmr1,(OS_ERR  *)&err);  //��ʱʱ�䵽,ɾ����ʱ��
	}
}

