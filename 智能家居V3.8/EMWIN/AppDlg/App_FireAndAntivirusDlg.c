#include "DIALOG.h"
#include "MyGUI.h"
#include "stdio.h"
#include "string.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x05)
#define ID_TEXT_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_0    (GUI_ID_USER + 0x07)
#define ID_TEXT_1    (GUI_ID_USER + 0x08)
#define ID_TEXT_2    (GUI_ID_USER + 0x09)

char g_cGas_buf[30];
extern float  g_GasConcentration;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, "GasWindow", ID_WINDOW_0, 0, 0, 1024, 565, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GasText", ID_TEXT_0, 254, 42, 521, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ReturnButton", ID_BUTTON_0, 924, 0, 100, 100, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 57, 206, 909, 330, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GasVolText", ID_TEXT_2, 254, 134, 521, 45, 0, 0x0, 0 },

};

/* ��ʼ����ȼ������ app�Ի��� */
static void InitFireAndAntivirusDlg(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	WM_CreateTimer(pMsg->hWin, 0, 1000, 0);	  //������ʱ��
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	WM_SetHasTrans(hItem);
	WM_SetCallback(hItem, _cbButtonReturn);

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "��ȼ��/����");
		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "MQ2�����ѹ��");

	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "������ͨ��MQ2������������ڻ�����\n"
											"�Ƿ���������ȼ��й©���Ӷ����б�\n"
											"�������û����Դﵽ������δȻ��Ч����\n"
											"ͨ��ADC����MQ2ģ���ѹ����ѹԽ�ߣ���\n"
											"��������ȼй©�ȼ���Ϊ�ޡ�΢������\n"
											"�ȡ��жȡ��ضȡ����ص������ȼ�");
}

static void _cbDialog(WM_MESSAGE * pMsg) 
{
//  WM_HWIN hItem;
  int     NCode;
  int     Id;
	char buf[30];
	static float GasConcentration = 0;
 
  switch (pMsg->MsgId) 
	{
  case WM_INIT_DIALOG:
		InitFireAndAntivirusDlg(pMsg);
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
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0),g_cGas_buf);  //��ʾ����/��ȼ��Ũ�ȵȼ�
			if(GasConcentration != g_GasConcentration)
			{
				GasConcentration = g_GasConcentration;
				sprintf(buf,"MQ2�����ѹ��%f V",g_GasConcentration);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_2),buf);  //��ʾMQ2��ѹֵ
			}
	
			WM_RestartTimer(pMsg->Data.v, 1000);   //������ʱ��
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
  
    }
    break;
 
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


void App_FireAndAntivirusDlg(WM_HWIN hWin) 
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
}

