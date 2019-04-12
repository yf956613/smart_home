/*
*********************************************************************************************************
*	                                  
*	ģ������ : �������ƽ������
*	�ļ����� : App_SocketCtrlDlg.c
*	��    �� : V1.0
*	˵    �� : �������ƽ���
*              1. �����б�����ѡ��Ҫ�����Ĳ�����
*              2. ͨ�������б�ѡ��Ҫ�򿪻�رյĵƣ���ɫͼ�꿪�ƣ���ɫͼ��ص�
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����        
*		V3.8    2018-12-8     Ares  	          
*   Mail    1741417385@qq.com                                  
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/


#include "DIALOG.h"


#include "MyGUI.h"
#include "includes.h"
#include "led.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_0    (GUI_ID_USER + 0x02)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_1    (GUI_ID_USER + 0x04)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_TEXT_1    (GUI_ID_USER + 0x07)

unsigned char SocketOnOfflag = 0;
static unsigned char  g_ucSocketNumber = 0;  //�Ƶ����(ͨ�����������֪ѡ�����ĸ���)
static char g_cBuf[10];

/* �ƹ���ƶԻ����ʼ����Դ�б� */
static const GUI_WIDGET_CREATE_INFO _aAppLampDialogCreate[] = {
  { WINDOW_CreateIndirect, "SocketWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 368, 57, 333, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 402, 149, 232, 38, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ON", ID_BUTTON_1, 467, 462, 110, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimerButton", ID_BUTTON_2, 0, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "OpenCloseText", ID_TEXT_1, 433, 402, 217, 31, 0, 0x0, 0 },
};

extern void App_SocketCtrlChlidDlg(WM_HWIN hWin);
/*
*********************************************************************************************************
*	�� �� ��: InitAppLampCtrlDlg
*	����˵��: ��ʼ���ƹ��������ƶԻ���
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitAppSocketCtrlDlg(WM_MESSAGE * pMsg)
{
	char buf[10];
	int i;
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//	WM_SetHasTrans(hItem);
//	BUTTON_SetFont(hItem,&GUI_FontHZ32);
	BUTTON_SetText(hItem, "");
	BUTTON_SetBitmapEx(hItem,0,SocketOnOfflag?&bmON:&bmOFF,0,0);
//	WM_SetCallback(hItem, _cbOpenButton);
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbTimerButton);
	
	
	//��ʾ����
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "                              ");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "����/�رգ�");
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetListHeight(hItem, 60);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
	for(i = 1; i < SOCKET_NUM + 1; i++)
	{
		sprintf(buf,"����%d",i);
		DROPDOWN_AddString(hItem, buf);
	}
	DROPDOWN_AddString(hItem, "���в���");
	DROPDOWN_SetItemSpacing(hItem, 50);   //����Ԫ��֮��ļ��
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //�Զ�ʹ�ù�����
	DROPDOWN_SetListHeight(hItem, 250);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
}
extern unsigned char g_bSocketTimerFlag;
extern unsigned char g_ucSocketStatus;
unsigned int SocketCountdownSec;
/* �ƹ���ƶԻ���Ļص����� */
static void _cbAppSocketCtrlDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	char buf[30];

	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //������ʱ��
			InitAppSocketCtrlDlg(pMsg);
		break;
		case WM_PAINT:
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  600,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
		break;
		case WM_TIMER:
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			if(g_bSocketTimerFlag)
			{
				if(1 == g_ucSocketStatus)
				{
					sprintf(buf,"��ʱ������ʱ��%d S",SocketCountdownSec);
					TEXT_SetText(hItem, buf);
				}
				if(2 == g_ucSocketStatus)
				{
					sprintf(buf,"��ʱ�ص���ʱ��%d S",SocketCountdownSec);
					TEXT_SetText(hItem, buf);
				}
			}
			else
			{
				sprintf(buf,"                             ");
				TEXT_SetText(hItem, buf);
			}
			
			BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,SocketOnOfflag?&bmON:&bmOFF,0,0);
			WM_RestartTimer(pMsg->Data.v, 1000);  //������ʱ��
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_0: // Notifications sent by 'Button'
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:  
								GUI_EndDialog(pMsg->hWin, 0);
					break;
		 
				}
				break;
				case ID_DROPDOWN_0: 
				switch(NCode)
				{
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_SEL_CHANGED:   //�����б��Ѿ�����
						hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
						g_ucSocketNumber = DROPDOWN_GetSel(hItem);
						DROPDOWN_GetItemText(hItem, g_ucSocketNumber,g_cBuf,sizeof(g_cBuf));
					break;
			  
				}
				break;
				case ID_BUTTON_1:   //��
				switch(NCode)
				{
				  case WM_NOTIFICATION_RELEASED:
							SOCKET_SW = ~SOCKET_SW;
							SocketOnOfflag = !SocketOnOfflag;
							BUTTON_SetBitmapEx(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1),0,SocketOnOfflag?&bmON:&bmOFF,0,0);
					break;
				
				}
				break;
				case ID_BUTTON_2:   
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						App_SocketCtrlChlidDlg(pMsg->hWin);   //������ʱ�����Ӵ���
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

void App_SocketCtrlDlg(WM_HWIN hWin)     
{
  GUI_CreateDialogBox(_aAppLampDialogCreate, GUI_COUNTOF(_aAppLampDialogCreate), _cbAppSocketCtrlDialog, hWin, 0, 0);
}


