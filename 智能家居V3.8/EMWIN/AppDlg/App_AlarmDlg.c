/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������app�������
*	�ļ����� : App_AlarmDlg.c
*	��    �� : V1.0
*	˵    �� : ��������app����
*            ͨ���رհ�ť�رշ�������  ͨ��������ť������������
*            ͨ����ʱ����ͼ��򿪶�ʱ������
*              
*		�汾��   ����         ����          ˵��
*		V1.0    2018-12-4    Ares Xu	       �װ�   
*   Mail    1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/


#include "DIALOG.h"
#include "MyGUI.h"
#include "string.h"
#include "stdio.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_0      (GUI_ID_USER + 0x05)



unsigned char gucAlarmFlag = 0;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
   { WINDOW_CreateIndirect, "AlaWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenALaButton", ID_BUTTON_0, 468, 285, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_1, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OpenCloseAlaText", ID_TEXT_0, 372, 204, 370, 41, 0, 0x0, 0 },
};

extern OS_TCB  AlarmTaskTCB;
extern  unsigned char GizwitsAlaFlag;
//extern unsigned char g_ucAlarmStatus;              
////extern void App_AlarmCtrlChlidDlg(WM_HWIN hWin);

/* ��ʼ���������� app�Ի��� */
void InitAlarmDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	BUTTON_SetText(hItem, "");
	BUTTON_SetBitmapEx(hItem,0,gucAlarmFlag?&bmON:&bmOFF,0,0);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "����/�رշ������ܣ�");
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
  int     NCode;
  int     Id;
//	char buf[40];
	OS_ERR err;

  switch (pMsg->MsgId) 
  {
  case WM_INIT_DIALOG:
		WM_CreateTimer(pMsg->hWin, 0, 2000, 0);	  //������ʱ��
		InitAlarmDlg(pMsg);
    break;
  case WM_PAINT:
		GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							0,			   /* ���Ͻ�Y λ�� */                    //purple
							1024,	   	       /* ���½�X λ�� */
							600,  	   	   /* ���½�Y λ�� */
							GUI_YELLOW,	   /* ���������Ҫ���Ƶ���ɫ */
							GUI_BLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
		break;
	case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetBitmapEx(hItem,0,gucAlarmFlag?&bmON:&bmOFF,0,0);
			WM_RestartTimer(pMsg->Data.v, 2000);  //������ʱ��
			break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'OpenALaButton'
      switch(NCode) 
	   {
        case WM_NOTIFICATION_RELEASED:
					gucAlarmFlag = !gucAlarmFlag;
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
					BUTTON_SetBitmapEx(hItem,0,gucAlarmFlag?&bmON:&bmOFF,0,0);
					if(gucAlarmFlag)
					{
						OSTaskQPost((OS_TCB*	)&AlarmTaskTCB,	//������AlarmSet��������Ϣ
											(void*		)ALAON,
											(OS_MSG_SIZE)sizeof(ALAON),
											(OS_OPT		)OS_OPT_POST_FIFO,
											(OS_ERR*	)&err);
					}
					else
					{
						OSTaskQPost((OS_TCB*	)&AlarmTaskTCB,	//������AlarmSet��������Ϣ
											(void*		)ALAOFF,
											(OS_MSG_SIZE)sizeof(ALAOFF),
											(OS_OPT		)OS_OPT_POST_FIFO,
											(OS_ERR*	)&err);
						GizwitsAlaFlag = 0;                    //�رձ��������������app����
					}
					
        break; 
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'CloseAlaButton'
     switch(NCode) 
	   {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);  //�رնԻ���
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

//����������������
void App_AlarmDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}
