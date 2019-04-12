/*
*********************************************************************************************************
*	                                  
*	模块名称 : 温湿度空气质量光照强度主界面悬浮窗设计
*	文件名称 : App_LampCtrlDlg.c
*	版    本 : V1.0
*	说    明 : 温湿度空气质量主界面悬浮窗
*            显示温湿度，空气质量，可燃气/烟雾等等
*              
*		版本号   日期         作者          说明
*		V3.8    2018-12-4    Ares 	      
*   Mail    1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "DIALOG.h"
#include "stdio.h"
#include "string.h"
#include "MyGUI.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_2    (GUI_ID_USER + 0x03)

#define MAG     3

unsigned char g_ucSwitchAirStatus = 0;
unsigned char g_ucSwitchMQ2Status = 0;
unsigned char temperature         = 23;
unsigned char humitiy             = 45;
extern unsigned short g_uAls;    //光照强度
float  g_GasConcentration = 10.07;
char *g_AirStatus[] = {"优 ","良好","轻度","中度","重度","严重"};
char *g_MQ2Status[] = {"无 ","微量","轻度","中度","重度","严重"};

float g_Air_test = 0;
extern char g_cAir_buf[24];
extern char g_cGas_buf[30];

/*
*********************************************************************************************************
*	函 数 名: _cbDialogSysInfo
*	功能说明: 回调函数
*	形    参: pMsg  WM_MESSAGE类型指针变量           
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialogSysInfo(WM_MESSAGE * pMsg) 
{
	GUI_RECT pRect;
	char buf[40];

	switch (pMsg->MsgId) 
	{
		case WM_TIMER:
			WM_InvalidateWindow(pMsg->hWin);
			WM_RestartTimer(pMsg->Data.v, 250);
			break;

		case WM_PAINT:
			GUI_SetColor(0x007C1877);
			WM_GetClientRect(&pRect);
			GUI_AA_FillRoundedRect(pRect.x0, pRect.y0, pRect.x1, pRect.y1, 6);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(GUI_FONT_13_1);

			GUI_SetFont(&GUI_FontHZ16);//设置字体
			
			sprintf(buf, "温度: %2.1d ℃",temperature);	
			GUI_DispStringAt(buf, 3, 3);			

			sprintf(buf, "湿度: %2.1d%% RH",humitiy);	
			GUI_DispStringAt(buf, 3, 20);

			switch(g_ucSwitchAirStatus)
			{
				case 0: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 1: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 2: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 3: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 4: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
				case 5: sprintf(buf, "空气质量: %s",g_AirStatus[g_ucSwitchAirStatus]);break;
			}
			strcpy(g_cAir_buf,buf);
			GUI_DispStringAt(buf, 3, 37);

			switch(g_ucSwitchAirStatus)
			{
				case 0: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 1: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 2: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 3: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 4: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
				case 5: sprintf(buf, "可燃气/烟雾: %s",g_MQ2Status[g_ucSwitchMQ2Status]);break;
			}
			strcpy(g_cGas_buf,buf);     //保存当前可燃气/烟雾等级
			GUI_DispStringAt(buf, 3, 54);
			
			
			sprintf(buf, "光照强度: %d cd",g_uAls);
			GUI_DispStringAt(buf, 3, 71);
	
			break;

		case WM_CREATE:
			/* 设置抗锯齿因数 */
			GUI_AA_SetFactor(MAG);
			
			WM_CreateTimer(pMsg->hWin, 0, 250, 0);	         
			break;

		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CreateSysInfoDlg
*	功能说明: 系统信息对话框
*	形    参: 无   	
*	返 回 值: 窗口句柄
*********************************************************************************************************
*/
void CreateTempHumiAirDlg(void) 
{
	WM_CreateWindow(3, 
	                450, 
	                142, 
	                90, 
	                WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_MOTION_X | WM_CF_MOTION_Y,
                   _cbDialogSysInfo, 
	                0);
}

