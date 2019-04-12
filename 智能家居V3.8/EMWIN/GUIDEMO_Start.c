/*
*********************************************************************************************************
*	                                  
*	模块名称 : 智能家居主界面设计
*	文件名称 : GUIDEMO_Start.c
*	版    本 : V1.0
*	说    明 : 智能家居界面
*              创建各个app界面
*              创建任务栏：显示时钟日历、cpu使用率等等  
*              悬浮窗用于温湿度、空气质量等级、可燃气/烟雾等级等等
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2018-12-8   Ares  	       首版   
*   Mail     1741417385@qq.com                                 
*	Copyright (C), 2018-2020, @Ares Xu
*
*********************************************************************************************************
*/

#include "GUIDEMO.h"
#include "MyGUI.h"


/**********************************************************************************************************
*                               创建ICONVIEW控件所需变量
**********************************************************************************************************/ 
typedef struct 
{
	const GUI_BITMAP * pBitmap;
	const char       * pTextEn;
	const char       * pTextCn;
} BITMAP_ITEM;


 /* 第1个ICONVIEW控件对应的app */
static void (* _apModules0[])( WM_HWIN hWin) =   
{
	AppSysInfoDlg,      //系统信息app
	App_LampCtrlDlg,    //灯光控制app
	App_ClockSetDlg,    //时钟日历app
	App_SocketCtrlDlg,   //智能插座app
	
	App_DoorWinCurtainDlg,        //智能门窗app
	App_HomeApplianceDlg,   //家电控制app
	App_LightDlg,           //亮度调节app
};
static const BITMAP_ITEM _aFirstInterface[] =  /* 第1个ICONVIEW控件的图标 */
{
	{&bmSystemInfo,      "System",         "系统信息"},
	{&bmLamp,		         "Lamp",            "灯光控制" },
	{&bmClock    ,       "Clock",           "时钟日历"},
	{&bmSocket,          "Socket",           "智能插座"},

	{&bmDoorWindows,       "DoorWindows",    "门窗控制"},
	{&bmElectricAppliance, "HomeAppliance",  "家电控制"},

	{&bmLight,           "Light",            "亮度调节"},
	
}; 

/* 第1个ICONVIEW控件对应的app */
static void (* _apModules1[])( WM_HWIN hWin) =  
{
	App_DeveloperDlg,    //开发者app
	App_TempHumiDlg,    //温湿度app
	App_AlarmDlg,       //防盗报警app
	App_AirQualityDlg,    //熄屏app
	
	App_FireAndAntivirusDlg,   //可燃气烟雾app
	App_SettingsDlg,           //设置app
	App_WifiStatusDlg,		     //wifi appp
};
static const BITMAP_ITEM _aSecondInterface[] =  /* 第2个ICONVIEW控件的图标 */  
{
	{&bmDeveloper,       "Developer",       "开发者"},
	{&bmTempHumi,		     "Temp&Humi",       "温湿度" },
	{&bmWanring    ,      "Wanring",        "防盗报警"},
	{&bmAirQuality,      "AirQuality",      "空气质量"},

	{&bmGasFire,         "GasFire",        "可燃气烟雾"},
	{&bmSettings,        "Settings",        "设置"},
	{&bmWIFI,            "WIFI",            "WIFI"},	

}; 

#define ICONVIEW_TBorder   50   /* 控件ICONVIEW的上边距 */
#define ICONVIEW_LBorder   54   /* 控件ICONVIEW的左边距 */  
#define ICONVIEW_Width     915  /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    120  /* 控件ICONVIEW的高 */  
#define ICONVIEW_YSpace    0    /* 控件ICONVIEW的Y方向间距 */
#define ICONVIEW_XSpace    30    /* 控件ICONVIEW的x方向间距 */

#define ICON_Width     105      /* 控件ICONVIEW的图标的宽度 */  
#define ICON_Height    115       /* 控件ICONVIEW的图标的高度, 含图标下面的文本，一起的高度 */ 

#define SCREEN_Width    1024     /* 显示屏的宽度 */  
#define SCREEN_Height   600     /* 显示屏高度   */ 

#define SysStartInfoX   100
#define SysStartDelay   150    //延时150ms
#define LineCap         40      /* 字符点阵的高是16 */


WM_HWIN hMotion;
static WM_HWIN hIcon1,hIcon2; //hIcon3;

#if 1
extern unsigned char _ac_bkpic[80802UL + 1];
GUI_HMEM hMempic;
#endif

/* 所有app的关闭按钮的回调函数 */
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
*	函 数 名: _cbTimerButton
*	功能说明: 定时管理按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
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
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口的回调函数,这里主要是绘制背景窗口和界面切换时，切换标志的绘制 
*	形    参: pMsg  WM_MESSAGE类型指针变量   
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		/* 重绘消息*/
		case WM_PAINT:
				GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  1024,	   	       /* 右下角X 位置 */
							  600,  	   	   /* 右下角Y 位置 */
							  GUI_BLACK,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */
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
				/* 第一个界面上的图标 */
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
							tStart = GUI_GetTime();
							break;

						/* ICON控件释放消息 */
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
				/* 第一个界面上的图标 */
				case GUI_ID_ICONVIEW1:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
							tStart = GUI_GetTime();
							break;

						/* ICON控件释放消息 */
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
*	函 数 名: _CreateICONVIEW
*	功能说明: 创建ICONVIEW
*	形    参：hParent   父窗口
*             pBm       ICONVIEW上的位图
*             BitmapNum ICONVIEW上图标个数       
*             x         x轴坐标
*             y         y轴坐标
*             w         ICONVIEW宽
*             h         ICONVIEW高   
*	返 回 值: 无
*********************************************************************************************************
*/
static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const BITMAP_ITEM *pBm, int BitmapNum, int Id, int x, int y, int w, int h) 
{
	WM_HWIN hIcon;
	int i;
	
	/*在指定位置创建指定尺寸的ICONVIEW 小工具*/
	hIcon = ICONVIEW_CreateEx(x, 					/* 小工具的最左像素（在父坐标中）*/
						     y, 					/* 小工具的最上像素（在父坐标中）*/
							 w,                     /* 小工具的水平尺寸（单位：像素）*/
							 h, 	                /* 小工具的垂直尺寸（单位：像素）*/
	                         hParent, 				            /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
	                         0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
							 Id, 			                    /* 小工具的窗口ID */
							 ICON_Width, 				        /* 图标的水平尺寸 */
							 ICON_Height);						/* 图标的垂直尺寸,图标和文件都包含在里面，不要大于ICONVIEW的高度，导致Text显示不完整*/
	
	
	/* 向ICONVIEW 小工具添加新图标 */
	for (i = 0; i < BitmapNum; i++) 
	{
		ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pTextCn);
	}
		
	ICONVIEW_SetFont(hIcon, &GUI_FontHZ16);
	
	/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
	
	/* 设置X方向的边界值为0，默认不是0, Y方向默认是0，这里我们也进行一下设置，方便以后修改 */
	ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
	ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);
	
	/* 设置图标在x 或y 方向上的间距。*/
	ICONVIEW_SetSpace(hIcon, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ICONVIEW_YSpace);
	
	/* 设置对齐方式 在5.22版本中最新加入的 */
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	
	//ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0xF020A0);
	return hIcon;
}



void MainTask(void) 
{
	WM_SetCreateFlags(WM_CF_MEMDEV);  //启动所有窗口的存储设备 启用后屏幕不会闪烁
	GUI_Init();  			           //STemWin初始化
	WM_MULTIBUF_Enable(1);       //开启STemWin多缓冲,RGB屏可能会用到
	
#if 1	
	/* 绘制桌面窗口的背景图片 */
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

	/* 设置桌面窗口的回调函数 */   
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);

	hMotion = WM_CreateWindowAsChild(0,    //桌面窗口起始x坐标
	                                 0, 	//桌面窗口起始y坐标
																	 SCREEN_Width, //显示屏宽度
																	 SCREEN_Height, //显示屏高度
																	 WM_HBKWIN,     //桌面窗口句柄
																	 WM_CF_SHOW | WM_CF_HASTRANS,  //创建后立即显示
																	 _cbMotion,                    //回调函数
																	0);
	
	/* 第1个CON图标 */
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

	/*创建任务栏对话框*/
	CreateStatusDlg();
	CreateTempHumiAirDlg();
	while(1)
	{
		GUI_Delay(10);
	}
}

/*************************** End of file ****************************/

