/******************************************************************************************
**文件名：MyGUI.h
**作用  : 用于对app的图标，app创建函数、共用的回调函数、汉字字体文件等等的声明
**作者  ：Ares Xu
**Date  : 2018-11-1
**Mail  : 1741417385@qq.com 
******************************************************************************************/

#ifndef _MYGUI_H_
#define  _MYGUI_H_
#include "GUIDEMO.h"
#include "sys.h"

#define MSG_UPDATECURTAIN     (GUI_ID_USER + 0x12)               //自定义emwin回调消息
#define MSG_UPDATEDOOR        (GUI_ID_USER + 0x13)               //自定义emwin回调消息
#define MSG_UPDATEWIN         (GUI_ID_USER + 0x14)               //自定义emwin回调消息

#define SOCKET_NUM   20    //插座的数量
#define LAMP_NUM   20    //灯的数量

#define LAMP_SW        PAout(6)
#define SOCKET_SW      PAout(11)
#define ALARM_SW       PAin(12)

#define ENABLE_ALA      HAL_NVIC_EnableIRQ(EXTI15_10_IRQn)     //失能中断线   关闭报警 
#define DISABLE_ALA     HAL_NVIC_DisableIRQ(EXTI15_10_IRQn)    //失能中断线   打开报警

#define DOOR_ON           "DOOR ON"
#define DOOR_OFF          "DOOR OFF"

#define WIN_ON            "WIN ON"
#define WIN_OFF           "WIN OFF"


#define CURTAIN_ON         "CURTAIN ON"
#define CURTAIN_OFF        "CURTAIN OFF"


#define ALAON   "ALAON"     //报警开
#define ALAOFF  "ALAOFF"    //报警关

extern unsigned char WinAutoFlag;     //窗自动控制标志位
extern unsigned char WinFLag;         //窗户控制标志位

extern unsigned char CurtainFLag;
extern unsigned char AutoCurtainFlag;  //窗帘自动控制标志位

extern WM_HWIN DoorWinCurtainAppHwin;

/*****************************************************************************************************************
*********************************App创建函数的外部声明************************************************************
******************************************************************************************************************/
extern void CreateStatusDlg(void);    //创建任务栏
extern  void AppSysInfoDlg(WM_HWIN hwin);  //系统信息App
extern void App_LampCtrlDlg(WM_HWIN hWin);
extern void App_HomeApplianceDlg(WM_HWIN hWin); //家电控制app
extern void App_ClockSetDlg(WM_HWIN hWin);   //时钟设置app
extern void App_ToBeDeveloped( WM_HWIN hWin) ;  //待开发app
extern void App_SocketCtrlDlg(WM_HWIN hWin) ;  //插座app
extern void App_TempHumiDlg(WM_HWIN hWin);      //温湿度app
extern void App_DoorWinCurtainDlg( WM_HWIN hWin) ;      //门窗app界面
extern void App_AirConditioningDlg(WM_HWIN hWin); //空调app
extern void App_LightDlg( WM_HWIN hWin);    //灯光调节app
extern void App_AlarmDlg(WM_HWIN hWin);    //防盗报警app
extern void App_DeveloperDlg(WM_HWIN hWin);  //开发者app
extern void App_AirQualityDlg(WM_HWIN hWin);   //空气质量app
extern void App_FireAndAntivirusDlg(WM_HWIN hWin);   //可燃气/烟雾
extern void App_SettingsDlg(WM_HWIN hWin);           //设置app界面
extern void App_WifiStatusDlg( WM_HWIN hWin);        //wifi设备连接状态
extern void CreateTempHumiAirDlg(void);    //显示温湿度/空气质量/可燃气烟雾 主界面悬浮窗
extern void App_HomeApplianceChildDlg(WM_HWIN hWin);
extern void App_HomeApplianceTimerChildDlg(WM_HWIN hWin);


/**************************************************************************************************************
**************************所有app关闭图标的回调函数和定时管理图标的回调函数************************************
***************************************************************************************************************/
extern void _cbButtonReturn(WM_MESSAGE * pMsg); /* 所有app的关闭按钮的回调函数 */
extern void _cbTimerButton(WM_MESSAGE * pMsg);  //所有定时管理按钮的回调函数

/************************************************************************************************************** 
*********************************************APP的图标外部声明************************************************* 
***************************************************************************************************************/
extern GUI_CONST_STORAGE GUI_BITMAP bmClock;   //时钟日历app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmSystemInfo; //系统信息app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmSettings; //设置App图标
extern GUI_CONST_STORAGE GUI_BITMAP bmLamp;     //灯光控制App图标
extern GUI_CONST_STORAGE GUI_BITMAP bmSocket;   //智能插座App图标
extern GUI_CONST_STORAGE GUI_BITMAP bmDoorWindows;  //智能门窗App图标
extern GUI_CONST_STORAGE GUI_BITMAP bmAirQuality;  //空气质量App图标
extern GUI_CONST_STORAGE GUI_BITMAP bmTempHumi;    //温湿度app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmWanring;    //安防报警app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmElectricAppliance; //家电控制app图标

extern GUI_CONST_STORAGE GUI_BITMAP bmDeveloper;  //开发者app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmLight;     //灯光调节app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmGasFire;        //可燃气烟雾app图标
extern GUI_CONST_STORAGE GUI_BITMAP bmWIFI;       //WIFI  app图标

extern GUI_CONST_STORAGE GUI_BITMAP bmTimerManager; //定时管理子功能窗口图标
//家电控制app里的图标
//extern GUI_CONST_STORAGE GUI_BITMAP bmAirConditioner72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmRefrigerator72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmTelevision72px;
//extern GUI_CONST_STORAGE GUI_BITMAP bmWasher72px;


extern GUI_CONST_STORAGE GUI_BITMAP bmON;
extern GUI_CONST_STORAGE GUI_BITMAP bmOFF;

// /* 灯光控制app的图标 */
//extern GUI_CONST_STORAGE GUI_BITMAP bmOffLamp;
//extern GUI_CONST_STORAGE GUI_BITMAP bmOnLamp;

 /* 所有App返回按钮图标 */
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;

extern GUI_CONST_STORAGE GUI_BITMAP _bmAlarm_16x16;  //任务栏闹钟图标
extern GUI_CONST_STORAGE GUI_BITMAP _bmBatteryEmpty_27x14;   //电池图标
extern GUI_CONST_STORAGE GUI_BITMAP * _apbmCharge[];
extern GUI_CONST_STORAGE GUI_BITMAP bmalarm_clock48x48;

/*汉字字体大小*/
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32;


#endif


