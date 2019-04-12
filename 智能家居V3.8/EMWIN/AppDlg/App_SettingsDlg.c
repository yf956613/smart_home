
#include "DIALOG.h"
#include "MyGUI.h"
#include "includes.h"
#include "ucos_msg.h"
#include "includes.h"
#include "timer.h"



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_0    (GUI_ID_USER + 0x07)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x08)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x09)
#define ID_BUTTON_1    (GUI_ID_USER + 0x0A)
#define ID_TEXT_0    (GUI_ID_USER + 0x0B)

OS_FLAG_GRP	ScreenLightEventFlags;   //��Ļ�¼���־��
const unsigned char g_cSreenLightArr[10] = {0,1,5,10,30,50,100,120,150,200};  //��ʱϨ��ʱ������
unsigned char g_ScreenLightTime;  //��ʱϨ��ʱ��

unsigned char g_ucAutoToCloseScreenLightFlag;  
unsigned char TimeIndex;
unsigned char SrceenOffFlag;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
   { WINDOW_CreateIndirect, "SettingsWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 149, 156, 225, 19, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 465, 156, 291, 32, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CloseButton", ID_BUTTON_1, 710, 444, 124, 63, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 169, 102, 194, 29, 0, 0x0, 0 },
};
extern unsigned int LightVal;

/* ��ʼ������ app�Ի��� */
static void InitSettingsDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
	CHECKBOX_SetText(hItem, "�����Զ�Ϩ��");
	CHECKBOX_SetFont(hItem, &GUI_FontHZ32);

	CHECKBOX_SetState(hItem,g_ucAutoToCloseScreenLightFlag);  //�ָ�֮ǰѡ����״̬

	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetFont(hItem, GUI_FONT_32B_1);
	BUTTON_SetText(hItem, "Ϩ��");
	BUTTON_SetFont(hItem, &GUI_FontHZ32);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_FontHZ32);
	TEXT_SetText(hItem, "�Զ�Ϩ����");
 
	hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
	DROPDOWN_SetListHeight(hItem, 60);
	DROPDOWN_SetFont(hItem, &GUI_FontHZ32);   
	DROPDOWN_AddString(hItem,"����");
	DROPDOWN_AddString(hItem,"1����");
	DROPDOWN_AddString(hItem,"5����");
	DROPDOWN_AddString(hItem,"10����");
	DROPDOWN_AddString(hItem,"30����");
	DROPDOWN_AddString(hItem,"50����");
	DROPDOWN_AddString(hItem,"100����");
	DROPDOWN_AddString(hItem,"120����");
	DROPDOWN_AddString(hItem,"150����");
	DROPDOWN_AddString(hItem,"200����");
	DROPDOWN_SetItemSpacing(hItem, 50);   //����Ԫ��֮��ļ��
	DROPDOWN_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	DROPDOWN_SetAutoScroll(hItem, 1);  //�Զ�ʹ�ù�����
	DROPDOWN_SetListHeight(hItem, 250);
	DROPDOWN_SetScrollbarWidth(hItem, 25);
	
	DROPDOWN_SetSel(hItem,TimeIndex);
}
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	OS_ERR err;
	
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
		InitSettingsDlg(pMsg);
    break;
	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  600,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
		break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ReturnButton'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
        	GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    case ID_DROPDOWN_0: // Notifications sent by 'Dropdown'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_SEL_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
				TimeIndex = DROPDOWN_GetSel(hItem);
				g_ScreenLightTime = g_cSreenLightArr[TimeIndex];   //��ȡ��ʱʱ��
        break;
      }
      break;
    case ID_CHECKBOX_0: // Notifications sent by 'Checkbox'
      switch(NCode) 
			{
      case WM_NOTIFICATION_RELEASED:
					
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
				if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)) && g_ScreenLightTime > 0)  //���ѡ���˶�ʱ��
				{
					g_ucAutoToCloseScreenLightFlag = 1;
				}
				else
				{
					g_ucAutoToCloseScreenLightFlag = 0;
				}
        break;
     
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'CloseButton' Ϩ��
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
//					TIM_SetTIM3Compare2(0);   //Ϩ����Ļ
//					SrceenOffFlag = 1;
//					LightVal = 0;
					OSFlagPost((OS_FLAG_GRP*)&ScreenLightEventFlags,
										(OS_FLAGS	  )SCREEN_OFF_FLAG,
										(OS_OPT	  )OS_OPT_POST_FLAG_SET,
										(OS_ERR*	  )&err);
				
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



void App_SettingsDlg(WM_HWIN hWin) 
{
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}




