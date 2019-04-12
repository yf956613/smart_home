/*
*********************************************************************************************************
*	                                  
*	ģ������ : ���ܼҾ����������
*	�ļ����� : GUIDEMO_Start.c
*	��    �� : V1.0
*	˵    �� : ���ܼҾӽ���
*              ��������app����
*              ��������������ʾʱ��������cpuʹ���ʵȵ�  
*              ������������ʪ�ȡ����������ȼ�����ȼ��/����ȼ��ȵ�
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2018-12-8   Ares  	       �װ�   
*   Mail     1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "GUIDEMO.h"
#include "MyGUI.h"


/**********************************************************************************************************
*                               ����ICONVIEW�ؼ��������
**********************************************************************************************************/ 
typedef struct 
{
	const GUI_BITMAP * pBitmap;
	const char       * pTextEn;
	const char       * pTextCn;
} BITMAP_ITEM;


 /* ��1��ICONVIEW�ؼ���Ӧ��app */
static void (* _apModules0[])( WM_HWIN hWin) =   
{
	AppSysInfoDlg,      //ϵͳ��Ϣapp
	App_LampCtrlDlg,    //�ƹ����app
	App_ClockSetDlg,    //ʱ������app
	App_SocketCtrlDlg,   //���ܲ���app
	
	App_DoorWinCurtainDlg,        //�����Ŵ�app
	App_HomeApplianceDlg,   //�ҵ����app
	App_LightDlg,           //���ȵ���app
};
static const BITMAP_ITEM _aFirstInterface[] =  /* ��1��ICONVIEW�ؼ���ͼ�� */
{
	{&bmSystemInfo,      "System",         "ϵͳ��Ϣ"},
	{&bmLamp,		         "Lamp",            "�ƹ����" },
	{&bmClock    ,       "Clock",           "ʱ������"},
	{&bmSocket,          "Socket",           "���ܲ���"},

	{&bmDoorWindows,       "DoorWindows",    "�Ŵ�����"},
	{&bmElectricAppliance, "HomeAppliance",  "�ҵ����"},

	{&bmLight,           "Light",            "���ȵ���"},
	
}; 

/* ��1��ICONVIEW�ؼ���Ӧ��app */
static void (* _apModules1[])( WM_HWIN hWin) =  
{
	App_DeveloperDlg,    //������app
	App_TempHumiDlg,    //��ʪ��app
	App_AlarmDlg,       //��������app
	App_AirQualityDlg,    //Ϩ��app
	
	App_FireAndAntivirusDlg,   //��ȼ������app
	App_SettingsDlg,           //����app
	App_WifiStatusDlg,		     //wifi appp
};
static const BITMAP_ITEM _aSecondInterface[] =  /* ��2��ICONVIEW�ؼ���ͼ�� */  
{
	{&bmDeveloper,       "Developer",       "������"},
	{&bmTempHumi,		     "Temp&Humi",       "��ʪ��" },
	{&bmWanring    ,      "Wanring",        "��������"},
	{&bmAirQuality,      "AirQuality",      "��������"},

	{&bmGasFire,         "GasFire",        "��ȼ������"},
	{&bmSettings,        "Settings",        "����"},
	{&bmWIFI,            "WIFI",            "WIFI"},	

}; 

#define ICONVIEW_TBorder   50   /* �ؼ�ICONVIEW���ϱ߾� */
#define ICONVIEW_LBorder   54   /* �ؼ�ICONVIEW����߾� */  
#define ICONVIEW_Width     915  /* �ؼ�ICONVIEW�Ŀ� */  
#define ICONVIEW_Height    120  /* �ؼ�ICONVIEW�ĸ� */  
#define ICONVIEW_YSpace    0    /* �ؼ�ICONVIEW��Y������ */
#define ICONVIEW_XSpace    30    /* �ؼ�ICONVIEW��x������ */

#define ICON_Width     105      /* �ؼ�ICONVIEW��ͼ��Ŀ�� */  
#define ICON_Height    115       /* �ؼ�ICONVIEW��ͼ��ĸ߶�, ��ͼ��������ı���һ��ĸ߶� */ 

#define SCREEN_Width    1024     /* ��ʾ���Ŀ�� */  
#define SCREEN_Height   600     /* ��ʾ���߶�   */ 

#define SysStartInfoX   100
#define SysStartDelay   150    //��ʱ150ms
#define LineCap         40      /* �ַ�����ĸ���16 */


WM_HWIN hMotion;
static WM_HWIN hIcon1,hIcon2; //hIcon3;

#if 1
extern unsigned char _ac_bkpic[80802UL + 1];
GUI_HMEM hMempic;
#endif

/* ����app�Ĺرհ�ť�Ļص����� */
void _cbButtonReturn(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{	
			  GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(100, 0, 84);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 28, 2);
			} 
			else 
			{
				GUI_SetColor(GUI_LIGHTBLUE);
				GUI_AA_FillCircle(100, 0, 84);
				GUI_SetBkColor(GUI_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 28, 2);	
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*********************************************************************************************************
*	�� �� ��: _cbTimerButton
*	����˵��: ��ʱ����ť�ص�����
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbTimerButton(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				 GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(0, 0, 84);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmTimerManager, 0, 0);
			} 
			else 
			{
				GUI_SetColor(GUI_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 84);
				GUI_SetBkColor(GUI_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmTimerManager, 0, 0);			
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _cbBkWindow
*	����˵��: ���洰�ڵĻص�����,������Ҫ�ǻ��Ʊ������ںͽ����л�ʱ���л���־�Ļ��� 
*	��    ��: pMsg  WM_MESSAGE����ָ�����   
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		/* �ػ���Ϣ*/
		case WM_PAINT:
				GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  1024,	   	       /* ���½�X λ�� */
							  600,  	   	   /* ���½�Y λ�� */
							  GUI_BLACK,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
#if 1
			GUI_MEMDEV_WriteAt(hMempic, 0, 0);
#endif

			break;
	
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

static void _cbMotion(WM_MESSAGE * pMsg) 
{
//	WM_MOTION_INFO * pInfo;
//	WM_HWIN          hWin = pMsg->hWin;
//	PARA           * pPara;
	static unsigned int  tStart, tEnd;
	int NCode, Id;
	
	switch (pMsg->MsgId) 
	{
		case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
			break;
		
		case WM_POST_PAINT:
			GUI_MULTIBUF_End();
			break;
		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);     
			NCode = pMsg->Data.v;                 
			switch (Id) 
			{
				/* ��һ�������ϵ�ͼ�� */
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							tStart = GUI_GetTime();
							break;

						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 
							tEnd = GUI_GetTime() - tStart;
							if(tEnd > 800)
							{
								WM_SetFocus(WM_HBKWIN);
								break;							
							}
							_apModules0[ICONVIEW_GetSel(pMsg->hWinSrc)](WM_HBKWIN);
							break;
					}
					break;
				/* ��һ�������ϵ�ͼ�� */
				case GUI_ID_ICONVIEW1:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							tStart = GUI_GetTime();
							break;

						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 
							tEnd = GUI_GetTime() - tStart;
							if(tEnd > 800)
							{
								WM_SetFocus(WM_HBKWIN);
								break;							
							}
							_apModules1[ICONVIEW_GetSel(pMsg->hWinSrc)](WM_HBKWIN);
							break;
					}
					break;
			}
	
		}
}

/*
*********************************************************************************************************
*	�� �� ��: _CreateICONVIEW
*	����˵��: ����ICONVIEW
*	��    �Σ�hParent   ������
*             pBm       ICONVIEW�ϵ�λͼ
*             BitmapNum ICONVIEW��ͼ�����       
*             x         x������
*             y         y������
*             w         ICONVIEW��
*             h         ICONVIEW��   
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const BITMAP_ITEM *pBm, int BitmapNum, int Id, int x, int y, int w, int h) 
{
	WM_HWIN hIcon;
	int i;
	
	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW С����*/
	hIcon = ICONVIEW_CreateEx(x, 					/* С���ߵ��������أ��ڸ������У�*/
						     y, 					/* С���ߵ��������أ��ڸ������У�*/
							 w,                     /* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
							 h, 	                /* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
	                         hParent, 				            /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
	                         0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
							 Id, 			                    /* С���ߵĴ���ID */
							 ICON_Width, 				        /* ͼ���ˮƽ�ߴ� */
							 ICON_Height);						/* ͼ��Ĵ�ֱ�ߴ�,ͼ����ļ������������棬��Ҫ����ICONVIEW�ĸ߶ȣ�����Text��ʾ������*/
	
	
	/* ��ICONVIEW С���������ͼ�� */
	for (i = 0; i < BitmapNum; i++) 
	{
		ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pTextCn);
	}
		
	ICONVIEW_SetFont(hIcon, &GUI_FontHZ16);
	
	/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
	
	/* ����X����ı߽�ֵΪ0��Ĭ�ϲ���0, Y����Ĭ����0����������Ҳ����һ�����ã������Ժ��޸� */
	ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
	ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);
	
	/* ����ͼ����x ��y �����ϵļ�ࡣ*/
	ICONVIEW_SetSpace(hIcon, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ICONVIEW_YSpace);
	
	/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	
	//ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0xF020A0);
	return hIcon;
}



void MainTask(void) 
{
	WM_SetCreateFlags(WM_CF_MEMDEV);  //�������д��ڵĴ洢�豸 ���ú���Ļ������˸
	GUI_Init();  			           //STemWin��ʼ��
	WM_MULTIBUF_Enable(1);       //����STemWin�໺��,RGB�����ܻ��õ�
	
#if 1	
	/* �������洰�ڵı���ͼƬ */
	hMempic = GUI_MEMDEV_CreateFixed(0, 
	                                 0, 
	                                 LCD_GetXSize(), 
	                                 LCD_GetYSize(), 
									 GUI_MEMDEV_HASTRANS, 
									 GUI_MEMDEV_APILIST_16, 
									 GUICC_M565);								 
	GUI_MEMDEV_Select(hMempic);
    GUI_JPEG_Draw(_ac_bkpic, sizeof(_ac_bkpic), 0, 0);
	GUI_MEMDEV_Select(0);
#endif

	/* �������洰�ڵĻص����� */   
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);

	hMotion = WM_CreateWindowAsChild(0,    //���洰����ʼx����
	                                 0, 	//���洰����ʼy����
																	 SCREEN_Width, //��ʾ�����
																	 SCREEN_Height, //��ʾ���߶�
																	 WM_HBKWIN,     //���洰�ھ��
																	 WM_CF_SHOW | WM_CF_HASTRANS,  //������������ʾ
																	 _cbMotion,                    //�ص�����
																	0);
	
	/* ��1��CONͼ�� */
	hIcon1 = _CreateICONVIEW(hMotion, 
	                _aFirstInterface, 
					GUI_COUNTOF(_aFirstInterface), 
					GUI_ID_ICONVIEW0, 
					ICONVIEW_LBorder, 
					ICONVIEW_TBorder, 
					ICONVIEW_Width, 
					ICONVIEW_Height);

	hIcon2 = _CreateICONVIEW(hMotion, 
	                _aSecondInterface, 
					GUI_COUNTOF(_aSecondInterface), 
					GUI_ID_ICONVIEW1, 
					ICONVIEW_LBorder, 
					ICONVIEW_TBorder+170, 
					ICONVIEW_Width, 
					ICONVIEW_Height);

	(void)hIcon1;
	(void)hIcon2;

	/*�����������Ի���*/
	CreateStatusDlg();
	CreateTempHumiAirDlg();
	while(1)
	{
		GUI_Delay(10);
	}
}

/*************************** End of file ****************************/

