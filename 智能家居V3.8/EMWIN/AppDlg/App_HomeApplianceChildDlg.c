#include "DIALOG.h"
#include "MyGUI.h"
#include "usart2.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x05)
#define ID_BUTTON_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_1    (GUI_ID_USER + 0x07)
#define ID_BUTTON_2    (GUI_ID_USER + 0x08)
#define ID_BUTTON_3    (GUI_ID_USER + 0x09)
#define ID_BUTTON_4    (GUI_ID_USER + 0x0A)
#define ID_TEXT_0    (GUI_ID_USER + 0x0B)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "HomeApplianceChildWindow", ID_WINDOW_0, 0, 0, 1024, 600, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OpenButton", ID_BUTTON_1, 259, 190, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CloseButton", ID_BUTTON_2, 632, 190, 140, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UpButton", ID_BUTTON_3, 349, 361, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 632, 361, 50, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect,   "Text", ID_TEXT_0, 468, 374, 113, 30, 0, 0x0, 0 },
};

extern void _cbButtonReturn(WM_MESSAGE * pMsg);

static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonReturn);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "��");
    //
    // Initialization of 'CloseButton'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "�ر�");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "��");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, &GUI_FontHZ32);
		BUTTON_SetText(hItem, "��");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontHZ32);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "�¶�");
		
    break;
  case WM_PAINT:
		GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
		0,			   /* ���Ͻ�Y λ�� */
		1024,	   	       /* ���½�X λ�� */
		565,  	   	   /* ���½�Y λ�� */
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
        GUI_EndDialog(pMsg->hWin, 0);   //�رնԻ���
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'OpenButton'
      switch(NCode) {
  
      case WM_NOTIFICATION_RELEASED:
				  usart2_send_byte(0xBB);		       //���Ϳ���ָ��
					usart2_send_byte(0x01);
					usart2_send_byte(0xFF);
        break;
     
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'CloseButton'
      switch(NCode) 
			{
				case WM_NOTIFICATION_RELEASED:           //���͹ػ�ָ��
				usart2_send_byte(0xBB);		
				usart2_send_byte(0x04);
				usart2_send_byte(0xFF);
        break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'UpButton'
      switch(NCode) {
				case WM_NOTIFICATION_RELEASED:        
					usart2_send_byte(0xBB);		      //�����¶�����ָ��
					usart2_send_byte(0x02);
					usart2_send_byte(0xFF);
        break;
     
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
				case WM_NOTIFICATION_RELEASED:      
					usart2_send_byte(0xBB);		    //�����¶ȼ�Сָ��
					usart2_send_byte(0x03);
					usart2_send_byte(0xFF);
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


void App_HomeApplianceChildDlg(WM_HWIN hWin) 
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

