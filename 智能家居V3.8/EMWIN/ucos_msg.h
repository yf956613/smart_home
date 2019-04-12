#ifndef _UCOS_MSG_H_
#define _UCOS_MSG_H_

#include "includes.h"


#define AIR_DEBUG             1u

#define AIR_DATA_MSG_LEN      8

#define RX_TX__DATA_MSG_LEN   1u

extern OS_TCB RX_TX_TaskTCB;
extern OS_TCB  AlarmTaskTCB;

extern OS_FLAG_GRP  AlaFlagGrp;                 //报警事件标志组     
//extern OS_FLAG_GRP  WinAndCurtainAutoFlagGrp;    //门窗自动控制事件标志组

//extern OS_Q  AIR_DATA_MSG;  //传输空气质量数据消息队列
////extern OS_Q  RX_TX_DATA_MSG;  //发送接收数据消息队列

extern OS_FLAG_GRP	ScreenLightEventFlags;		//屏幕熄屏亮度事件标志位

#define SCREEN_OFF_FLAG         (1 << 0)      //熄屏事件标志位

#define ALA_SET                 (1 << 0)    //防盗报警事件标志位   防盗报警用
#define ALA_OFF                 (1 << 1)   //报警关闭事件标志位    烟雾/可燃气报警用

//#define HUMI_TEMP_HIGHT         (1 << 0)   //温度和湿度过高事件标志位    窗户自动控制用
//#define AIR_QUALITY_BAD         (1 << 1)   //空气质量差事件标志位        窗户自动控制用
//#define GAS_SMOKE_EXIST         (1 << 2)   //可燃气/烟雾存在事件标志位   窗户自动控制用

//extern OS_SEM DOOR_WIN_CURTAIN_SEM;

#endif

