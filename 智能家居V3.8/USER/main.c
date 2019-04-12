#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "timer.h"
#include "dht11.h"
#include "sdram.h"
#include "malloc.h"
#include "touch.h"
#include "rtc.h"
#include "GUI.h"
#include "WM.h"
#include "sdio_sdcard.h"
#include "ff.h"
#include "exfuns.h"
#include "ftl.h"
#include "w25qxx.h"
#include "fontupd.h"
#include "includes.h"
#include "DIALOG.h"
#include "24l01.h"
#include "adc.h"
#include "ucos_msg.h"
#include "timer.h"
#include "ap3216c.h"
#include "usart3.h"
#include "gizwits_product.h" 
#include "MyGUI.h"
#include "pcf8574.h"
/************************************************
 UCOSIII中以下优先级用户程序不能使用，ALIENTEK
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
************************************************/

//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//开始任务任务函数   用于创建其他任务等等
void start_task(void *p_arg);


//任务优先级
#define GIZWITS_TASK_PRIO				3
//任务堆栈大小	
#define GIZWITS_STK_SIZE 				256
//任务控制块
OS_TCB GizwitsTaskTCB;
//任务堆栈	
CPU_STK GIZWITS_TASK_STK[GIZWITS_STK_SIZE];
//开始任务任务函数   用于创建其他任务等等
void gizwits_task(void *p_arg);


//NRF24L01数据收发任务优先级
#define RX_TX_TASK_PRIO				4
//任务堆栈大小	
#define RX_TX_STK_SIZE 				128
//任务控制块
OS_TCB RX_TX_TaskTCB;
//任务堆栈	
CPU_STK RX_TX_TASK_STK[RX_TX_STK_SIZE];
////NRF24L01数据收发
void RX_TX_task(void *p_arg);


//空气质量检测任务
//设置任务优先级
#define AIRQUALITY_TASK_PRIO 				5
//任务堆栈大小
#define AIRQUALITY_STK_SIZE				128
//任务控制块
OS_TCB AIRQUALITYTaskTCB;
//任务堆栈
CPU_STK AIRQUALITY_TASK_STK[AIRQUALITY_STK_SIZE];
//空气质量检测任务
void air_quality_task(void *p_arg);

//设置任务优先级
#define HC_SR501_TASK_PRIO 				6
//任务堆栈大小
#define HC_SR501_STK_SIZE				  64
//任务控制块
OS_TCB HC_SR501TaskTCB;
//任务堆栈
CPU_STK HC_SR501_TASK_STK[HC_SR501_STK_SIZE];
void HC_SR501(void *p_arg);

//DHT11任务
//设置任务优先级
#define DHT11_TASK_PRIO			6
//任务堆栈大小
#define DHT11_STK_SIZE			128
//任务控制块
OS_TCB DHT11TaskTCB;
//任务堆栈
CPU_STK DHT11_TASK_STK[DHT11_STK_SIZE];
//温湿度检测任务
void dht11_task(void *p_arg); 


//可燃气/烟雾数据处理任务
//设置任务优先级
#define AIRDATADEAL_TASK_PRIO			7
//任务堆栈大小
#define AIRDATADEAL_STK_SIZE			128
//任务控制块
OS_TCB AIRDATADEALTaskTCB;
//任务堆栈
CPU_STK AIRDATADEAL_TASK_STK[AIRDATADEAL_STK_SIZE];
//空气质量数据处理任务
void air_quality_deal_task(void *p_arg);

//空气质量数据处理任务
//设置任务优先级
#define FLAMMABLE_GAS_TASK_PRIO			8
//任务堆栈大小
#define FLAMMABLE_GAS_STK_SIZE			128
//任务控制块
OS_TCB FLAMMABLE_GASTaskTCB;
//任务堆栈
CPU_STK FLAMMABLE_GAS_TASK_STK[FLAMMABLE_GAS_STK_SIZE];
//可燃气\烟雾处理任务
void flammable_gas_task(void *p_arg);

//设置任务优先级
#define LIGHT_INTENSITY_TASK_PRIO			9
//任务堆栈大小
#define LIGHT_INTENSITY_STK_SIZE			64
//任务控制块
OS_TCB LIGHT_INTENSITYTaskTCB;
//任务堆栈
CPU_STK LIGHT_INTENSITY_TASK_STK[LIGHT_INTENSITY_STK_SIZE];
void light_intensity(void *p_arg);

//设置任务优先级
#define ALARM_TASK_PRIO			10
//任务堆栈大小
#define ALARM_STK_SIZE			64
//任务控制块
OS_TCB  AlarmTaskTCB;
//任务堆栈
CPU_STK ALARM_TASK_STK[ALARM_STK_SIZE];
void alarmset_task(void *pdata);

//设置任务优先级
#define BEEP_TASK_PRIO			11
//任务堆栈大小
#define BEEP_STK_SIZE			  64
//任务控制块
OS_TCB  BeepTaskTCB;
//任务堆栈
CPU_STK BEEP_TASK_STK[BEEP_STK_SIZE];
void beep_task(void *pdata);

////设置任务优先级
//#define WIN_AUTO_TASK_PRIO			12
////任务堆栈大小
//#define WIN_AUTO_STK_SIZE			  64
////任务控制块
//OS_TCB  WinAUtoTaskTCB;
////任务堆栈
//CPU_STK WINAUTO_TASK_STK[WIN_AUTO_STK_SIZE];
//void win_auto_task(void *p_arg);


//设置任务优先级
#define SCREENLIGHT_TASK_PRIO			5                 //旧优先级12
//任务堆栈大小
#define SCREENLIGHT_STK_SIZE			128
//任务控制块
OS_TCB SCREENLIGHTTaskTCB;
//任务堆栈
CPU_STK SCREENLIGHT_TASK_STK[SCREENLIGHT_STK_SIZE];
//屏幕自动熄屏触摸亮屏任务任务
void screen_LightIsOffOrON(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				13
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			14
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			4096
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
void emwindemo_task(void *p_arg);

void MainTask(void);
void Gizwits_Init(void);     //机智云协议相关初始化
void SmartHomeIOInit(void);  //控制GPIO的初始化

int main(void)
{
  OS_ERR err;
	CPU_SR_ALLOC();
  
	Stm32_Clock_Init(360,25,2,8);   //设置时钟为180Mhz   
	HAL_Init();                     //初始化HAL库
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART1  用于printf
	uart2_init(9600);               //初始化usart2  用于家电控制app的实现
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	DHT11_Init();                   //初始化DHT11
	RTC_Init();                     //初始化RTC
	NRF24L01_Init();                //初始化NRF24L01
	MY_ADC_Init();                  //初始化ADC
	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //配置WAKE UP中断,1秒钟中断一次 
	SDRAM_Init();                   //SDRAM初始化
	TFTLCD_Init();  		           //LCD初始化
	AP3216C_Init();                   //光照传感器初始化
	TIM3_PWM_Init(500-1,90-1);      //用PWM来调节LCD亮度 90M/90=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	FTL_Init();                     //NAND初始化
	TP_Init();				              //触摸屏初始化
	SmartHomeIOInit();              //控制灯插座等GPIO初始化
	
	Gizwits_Init();                //机智云协议初始化
	gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link 模式接入
//	NRF24L01_TX_Mode();  //设置为NRF24L01发送模式
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池
	
	exfuns_init();			        //为fatfs相关变量申请内存				 
  f_mount(fs[0],"0:",1); 		    //挂载SD卡 
	
	while(font_init())		//初始化字库
	{
		printf("Font Error!\n\r");
		while(SD_Init())	//检测SD卡
		{
			printf("SD Card Failed!\r\n");
			delay_ms(200);
			printf("SD Card Failed!\r\n");;
			delay_ms(200);		    
		}
		update_font(30,90,16,"0:");	//如果字库不存在就更新字库
		delay_ms(2000);
		LCD_Clear(WHITE);	//清屏
		break;
	}
	
	while(NRF24L01_Check())
	{
		printf("NRF24L01 is not exist!\r\n");
		delay_ms(200);
	}
	
	while(PCF8574_Init())                 //IO扩展初始化  控制蜂鸣器
	{
		printf("PCF8574 is not exist!\r\n");
		delay_ms(200);
	}
    
  OSInit(&err);		            //初始化UCOSIII
	OS_CRITICAL_ENTER();            //进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	            //退出临界区	 
	OSStart(&err);                  //开启UCOSIII
	while(1);
}

extern OS_TMR 	g_AutoLight;
extern OS_FLAG_GRP  AlaFlagGrp;
//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务 CPU使用率               
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	//使能时间片轮转调度功能,设置默认的时间片长度
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟  STM32使用emwin必须打开CRC时钟

	OS_CRITICAL_ENTER();	       //进入临界区	

	//创建事件事件标志组
	OSFlagCreate((OS_FLAG_GRP*)&ScreenLightEventFlags,		//指向事件标志组
                 (CPU_CHAR*	  )"ScreenLightEventFlags",	//名字
                 (OS_FLAGS	  )0,	//事件标志组初始值
                 (OS_ERR*  	  )&err);			//错误码	
								 
	//创建报警事件事件标志组
	OSFlagCreate((OS_FLAG_GRP*)&AlaFlagGrp,		//指向事件标志组
                 (CPU_CHAR*	  )"AlaFlagGrp",	//名字
                 (OS_FLAGS	  )0,	//事件标志组初始值
                 (OS_ERR*  	  )&err);			//错误码	
								 
		//STemWin gui任务							
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
								 
		//接收数据发送数据任务
	OSTaskCreate((OS_TCB*     )&RX_TX_TaskTCB,		
								(CPU_CHAR*   )"RX_TX task", 		
                 (OS_TASK_PTR )RX_TX_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )RX_TX_TASK_PRIO,     
                 (CPU_STK*    )&RX_TX_TASK_STK[0],	
                 (CPU_STK_SIZE)RX_TX_STK_SIZE/10,	
                 (CPU_STK_SIZE)RX_TX_STK_SIZE,		
                 (OS_MSG_QTY  )10,					      //NRF24L01任务内建消息队列长度6
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			
								 
		//可燃气任务
	OSTaskCreate((OS_TCB*     )&FLAMMABLE_GASTaskTCB,		
								(CPU_CHAR*   )"FLAMMABLE_GAS task", 		
                 (OS_TASK_PTR )flammable_gas_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )FLAMMABLE_GAS_TASK_PRIO,     
                 (CPU_STK*    )&FLAMMABLE_GAS_TASK_STK[0],	
                 (CPU_STK_SIZE)FLAMMABLE_GAS_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLAMMABLE_GAS_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )  0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);		
	//触摸屏任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//DHt11任务
	OSTaskCreate((OS_TCB*     )&DHT11TaskTCB,		
								(CPU_CHAR*   )"dht11 task", 		
                 (OS_TASK_PTR )dht11_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )DHT11_TASK_PRIO,     
                 (CPU_STK*    )&DHT11_TASK_STK[0],	
                 (CPU_STK_SIZE)DHT11_STK_SIZE/10,	
                 (CPU_STK_SIZE)DHT11_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	    
								 
	//空气质量数据检测任务
	OSTaskCreate((OS_TCB*     )&AIRQUALITYTaskTCB,		
									(CPU_CHAR*   )"air_quality task", 		
                 (OS_TASK_PTR )air_quality_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )AIRQUALITY_TASK_PRIO,     
                 (CPU_STK*    )&AIRQUALITY_TASK_STK[0],	
                 (CPU_STK_SIZE)AIRQUALITY_STK_SIZE/10,	
                 (CPU_STK_SIZE)AIRQUALITY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
	 //创建检测自动熄屏任务					 
	OSTaskCreate((OS_TCB*     )&SCREENLIGHTTaskTCB,		
				       (CPU_CHAR*   )"SCREENLIGH task", 		
                 (OS_TASK_PTR )screen_LightIsOffOrON, 			
                 (void*       )0,					
                 (OS_PRIO	  )SCREENLIGHT_TASK_PRIO,     
                 (CPU_STK*    )&SCREENLIGHT_TASK_STK[0],	
                 (CPU_STK_SIZE)SCREENLIGHT_STK_SIZE/10,	
                 (CPU_STK_SIZE)SCREENLIGHT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
			//光照强度采集任务			 
		OSTaskCreate((OS_TCB*     )&LIGHT_INTENSITYTaskTCB,		
				         (CPU_CHAR*   )"LIGHT_INTENSITY task", 		
                 (OS_TASK_PTR )light_intensity, 			
                 (void*       )0,					
                 (OS_PRIO	    ) LIGHT_INTENSITY_TASK_PRIO,     
                 (CPU_STK*    )&LIGHT_INTENSITY_TASK_STK[0],	
                 (CPU_STK_SIZE)LIGHT_INTENSITY_STK_SIZE/10,	
                 (CPU_STK_SIZE)LIGHT_INTENSITY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);    
								 
		//机智云协议处理任务			 
		OSTaskCreate((OS_TCB*     )&GizwitsTaskTCB,		
				         (CPU_CHAR*   )"Gizwits task", 		
                 (OS_TASK_PTR )gizwits_task, 			
                 (void*       )0,					
                 (OS_PRIO	    ) GIZWITS_TASK_PRIO,     
                 (CPU_STK*    )&GIZWITS_TASK_STK[0],	
                 (CPU_STK_SIZE)GIZWITS_STK_SIZE/10,	
                 (CPU_STK_SIZE)GIZWITS_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);   
		
		//报警功能处理任务			 
		OSTaskCreate((OS_TCB*     )&AlarmTaskTCB,		
				         (CPU_CHAR*   )"Alarm task", 		
                 (OS_TASK_PTR )alarmset_task, 			
                 (void*       )0,					
                 (OS_PRIO	    ) ALARM_TASK_PRIO,     
                 (CPU_STK*    )&ALARM_TASK_STK[0],	
                 (CPU_STK_SIZE)ALARM_STK_SIZE/10,	
                 (CPU_STK_SIZE)ALARM_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);    
		
		//蜂鸣器模拟报警任务			 
		OSTaskCreate((OS_TCB*     )&BeepTaskTCB,		
				         (CPU_CHAR*   )"Beep task", 		
                 (OS_TASK_PTR )beep_task, 			
                 (void*       )0,					
                 (OS_PRIO	    )BEEP_TASK_PRIO,     
                 (CPU_STK*    )&BEEP_TASK_STK[0],	
                 (CPU_STK_SIZE)BEEP_STK_SIZE/10,	
                 (CPU_STK_SIZE)BEEP_STK_SIZE,		
                 (OS_MSG_QTY  )3,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
		
		//报警任务			 
		OSTaskCreate((OS_TCB*     )&HC_SR501TaskTCB,		
				         (CPU_CHAR*   )"HC_SR501", 		
                 (OS_TASK_PTR )HC_SR501, 			
                 (void*       )0,					
                 (OS_PRIO	    )HC_SR501_TASK_PRIO,     
                 (CPU_STK*    )&HC_SR501_TASK_STK[0],	
                 (CPU_STK_SIZE)HC_SR501_STK_SIZE/10,	
                 (CPU_STK_SIZE)HC_SR501_STK_SIZE,		
                 (OS_MSG_QTY  )3,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
		
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWIND  GUI任务
void emwindemo_task(void *p_arg)
{
//	OS_ERR err;
	
	while(1)
	{
		MainTask();
	}
}

extern dataPoint_t currentDataPoint;
	
extern unsigned char Enter_IT_Flag;
void userHandle(void);
//机智云上报数据任务
void gizwits_task(void *p_arg)
{
	p_arg=p_arg;
	OS_ERR err;
	while(1)
	{ 
		OSSchedLock(&err);  //任务加锁
		gizwitsHandle((dataPoint_t *)&currentDataPoint);   //机智云协议处理
		userHandle();         //用户数据采集
		OSSchedUnlock(&err);	//任务加锁	
		
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);//延时200ms			
	}
}

//温湿度任务
void dht11_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		DHT11_Read_Data(&temperature,&humitiy);	           //温湿度采集
#if 0 
		printf("温度:%d,湿度:%d\r\n",temperature,humitiy);     //调试信息打印
#endif
		LED0 = !LED0;                                      //标志系统运行
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms		
	}
}

extern unsigned char g_ucSwitchAirStatus; 
extern float g_Air_test;
u16 air_adc_val;
//空气质量检测任务
void air_quality_task(void *p_arg)
{
	OS_ERR err;
	float temp;

	while(1)
	{
		air_adc_val = Get_Adc_Average(ADC_CHANNEL_5,10);  //采集adc1通道5的值
		temp=(float)air_adc_val*(3.3/4096);               //计算出对应的电压值	3.3是参考电压，4096是ADC是12位，2^12 = 4096
		temp = temp * 2 ;
		g_Air_test = temp;
#if 0
		sprintf(buf,"电压：%f",g_Air_test);     //调试信息打印
		printf("%s\r\n",buf);
#endif
		if(temp < 1.0f && temp >= 0.0f)
		{
			g_ucSwitchAirStatus = 0;
		}
		else if(temp >= 1.0f && temp < 1.8f)
		{
			g_ucSwitchAirStatus = 1;
		}
		else if(temp >= 1.8f && temp < 2.6f)
		{
			g_ucSwitchAirStatus = 2;
		}
		else if(temp >= 2.6f && temp < 3.4f)
		{
			g_ucSwitchAirStatus = 3;
		}
		else if(temp >= 3.4f && temp < 4.2f)
		{
			g_ucSwitchAirStatus = 4;
		}
		else
		{
			g_ucSwitchAirStatus = 5;
		}	
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//延时100ms
		
	}
}

unsigned char AlaSetFlag = 0;  //报警发生标志位
extern float  g_GasConcentration;
extern unsigned char g_ucSwitchMQ2Status;
u16 gas_adc_val;
unsigned char GizwitsGasFlag = 0;
//可燃气体检测任务
void flammable_gas_task(void *p_arg)
{
	OS_ERR err;

	float temp;

	while(1)
	{
		gas_adc_val = Get_Adc_Average(ADC_CHANNEL_0,10);  //采集adc1通道10的值
		temp=(float)gas_adc_val * (3.3 / 4096);   		//计算出对应的电压值	
		g_GasConcentration = temp * 2;      
		if(g_GasConcentration < 1.0f && g_GasConcentration >= 0.0f)
		{
			g_ucSwitchMQ2Status = 0;
		}
		else if(g_GasConcentration >= 1.0f && g_GasConcentration < 1.8f)
		{
			g_ucSwitchMQ2Status = 1;
		}
		else if(g_GasConcentration >= 1.8f && g_GasConcentration < 2.6f)
		{
			g_ucSwitchMQ2Status = 2;
		}
		else if(g_GasConcentration >= 2.6f && g_GasConcentration < 3.4f)
		{
			g_ucSwitchMQ2Status = 3;
		}
		else if(g_GasConcentration >= 3.4f && g_GasConcentration < 4.2f)
		{
			g_ucSwitchMQ2Status = 4;
		}
		else
		{
			g_ucSwitchMQ2Status = 5;
		}		
		if(g_GasConcentration > 2.8f)
		{
			OSFlagPost((OS_FLAG_GRP*)&AlaFlagGrp,      //发送报警事件标志
							(OS_FLAGS	  )ALA_SET,
							(OS_OPT	  )OS_OPT_POST_FLAG_SET,
							(OS_ERR*	  )&err);
			GizwitsGasFlag = 1;
		}
		else
		{
			PCF8574_WriteBit(BEEP_IO,1);	//关闭蜂鸣器
			AlaSetFlag = 0;
			GizwitsGasFlag = 0;
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//延时100ms
//		
	}
}

extern unsigned int LightVal;
extern unsigned char g_ScreenLightTime;
extern unsigned char g_ucAutoToCloseScreenLightFlag;
extern unsigned char SrceenOffFlag;
/*实现屏幕自动熄屏触摸亮屏任务*/
void screen_LightIsOffOrON(void *p_arg)
{
	static u32 countSec = 0;
	u32 x,y;
	OS_FLAGS flags_num;
	OS_ERR err;
	p_arg = p_arg;
	for(;;)
	{
		flags_num = OSFlagPend((OS_FLAG_GRP*)&ScreenLightEventFlags,   //收到熄屏信号量
													 (OS_FLAGS	) SCREEN_OFF_FLAG, 
													 (OS_TICK     )0xf8,
													 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_NON_BLOCKING,
													 (CPU_TS*     )0,
													 (OS_ERR*	    )&err);
		if(flags_num & SCREEN_OFF_FLAG)  //收到熄屏信号量
		{
			TIM_SetTIM3Compare2(0);   //熄灭屏幕
			SrceenOffFlag = 1;        //屏幕熄灭标志位置1
			LightVal = 0;
		}
		if(g_ucAutoToCloseScreenLightFlag || SrceenOffFlag)  //如果开启了屏幕自动关闭或屏幕是灭的才扫描触摸屏是否有触摸
		{
			if(x != GUI_TOUCH_X_MeasureX() && GUI_TOUCH_X_MeasureY() != y)        //如果有触摸
			{		
				x = GUI_TOUCH_X_MeasureX();//保存这一次的触摸值
				y = GUI_TOUCH_X_MeasureY();
				countSec = 0;                //计时变量清零
//				LED0 = !LED0;
				if(SrceenOffFlag)
				{
					SrceenOffFlag = 0;
					TIM_SetTIM3Compare2(500);  //点亮屏幕
				}
				LightVal = 500;
			}
			else if(g_ScreenLightTime > 0 && g_ucAutoToCloseScreenLightFlag)
			{
				countSec++;
				if(countSec >= g_ScreenLightTime*60)         //到达定时灭屏时间
				{
					 SrceenOffFlag = 1;
					 TIM_SetTIM3Compare2(0);                   //熄灭屏幕
				}
			}
		}
	#if 0
		printf("x=%d,y=%d\n",GUI_TOUCH_X_MeasureX(),GUI_TOUCH_X_MeasureY());
	#endif
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);       //延时1s
	}
}

unsigned short g_uAls;  //光照强度
extern unsigned char LampAutoFlag;
extern unsigned char LED_flag;
/* 光照强度采集、自动控灯以及窗帘自控任务*/
void light_intensity(void *p_arg)
{
	u16 ir,ps;
	OS_ERR err;
	unsigned char CountON;  //计数光照强度达到一定程度某个次数才开灯
	unsigned char countOFF; //计数光照强度达到一定程度某个次数才关灯

	static unsigned char AutoLampOnFlag = 0;
	p_arg = p_arg;
	while(1)
	{	
		AP3216C_ReadData(&ir,&ps,&g_uAls);  //采集光照强度
#if 0
		printf("light = %d\r\n",als);
#endif
		if(LampAutoFlag && g_uAls <= 10 && AutoLampOnFlag == 0)  //开启自动控灯以及光照强度小于10cd          
		{
			CountON++;
			if(CountON >= 10)
			{
				CountON = 0;
				if(LED_flag == 0) //如果灯是关了的
				{
					LAMP_SW = 1;    //开灯
					LED_flag = 1;
					AutoLampOnFlag = 1;
				}
			}
			
		}
		if(g_uAls > 10 && LampAutoFlag && AutoLampOnFlag)  //如果光强大于10cd和开启了自动控灯
		{
			countOFF++;
			if(countOFF >= 10)
			{
				countOFF = 0;
				if(LED_flag)   //如果灯是开了的
				{
					LED_flag = 0;
					LAMP_SW = 0;   //关灯
					AutoLampOnFlag = 0;
				}
			}
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);       //延时1s
	}
}

//NRF24L01数据发送任务
void RX_TX_task(void *p_arg) 
{
	OS_MSG_SIZE	 msg_size;
	OS_ERR       err;
	CPU_TS	     ts;
	char *g_cRX_TX_Buf;
	while(1)
	{
		g_cRX_TX_Buf =  OSTaskQPend(0,
												 OS_OPT_PEND_BLOCKING,
												 &msg_size,   /* 此参数是接收到数据个数 */
												 &ts,
												 &err);
		OSSchedLock(&err);  //调度器加锁
		NRF24L01_TX_Mode();   //设置发送模式
		NRF24L01_TxPacket((unsigned char*)g_cRX_TX_Buf); //发送数据   
		OSSchedUnlock(&err);	//调度器解锁
	}
}

extern  unsigned char CountAla;

//防盗报警app开关任务
void alarmset_task(void *pdata)
{
//	unsigned char count;
	OS_ERR err;
	OS_MSG_SIZE	 msg_size;
	CPU_TS	     ts;
	char *Buf;
	pdata = pdata;

	while(1)
	{
		Buf =  OSTaskQPend(0,
											 OS_OPT_PEND_BLOCKING,
											 &msg_size,   /* 此参数是接收到数据个数 */
											 &ts,
											 &err);
		if(strcmp(Buf,"ALAON") == 0)  //如果开启报警功能
		{
			ENABLE_ALA;           //开启报警
		}
		if(strcmp(Buf,"ALAOFF") == 0)
		{       
			DISABLE_ALA;           //关闭报警
			PCF8574_WriteBit(BEEP_IO,1);	//关闭蜂鸣器
			AlaSetFlag = 0;
		}
#if 0
		printf("%s\n\r",Buf);
#endif
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//延时1S
	}
}



//HC_SR501人体红外检测报警任务
void HC_SR501(void *p_arg)
{
	OS_ERR err;
	OS_FLAGS flags_num;
	
	while(1)
	{
		flags_num = OSFlagPend((OS_FLAG_GRP*)&AlaFlagGrp,   //收到触发报警标志
													 (OS_FLAGS	) ALA_SET, 
													 (OS_TICK     )0,
													 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME ,  //+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_NON_BLOCKING
													 (CPU_TS*     )0,
													 (OS_ERR*	    )&err);
		if(flags_num & ALA_SET)  //如果检测到热释电传感器信号
		{
			AlaSetFlag = 1;       //报警标志位置1
		}		
	}
}

//蜂鸣器报警任务   防盗报警或烟雾可燃气都会报警
void beep_task(void *pdata)
{
	OS_ERR err;
	unsigned char beepsta = 1;
	pdata = pdata;
	
	while(1)
	{		
		if(AlaSetFlag)
		{
#if 0
//			printf("%s\r\n",Buf);
//			printf("shou dao\n\r");
//			AlaSetFlag = 0;
#endif
			beepsta=!beepsta;					//蜂鸣器状态取反
			PCF8574_WriteBit(BEEP_IO,beepsta);	//控制蜂鸣器
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);//延时100ms
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}



//TOUCH任务
void touch_task(void *p_arg) 
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}


//OSStatTaskCPUUsage  CPU使用率
