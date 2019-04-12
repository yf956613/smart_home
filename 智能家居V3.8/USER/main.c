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
 UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
 ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 ���ȼ�0���жϷ������������� OS_IntQTask()
 ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 ���ȼ�2����ʱ���� OS_TmrTask()
 ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
************************************************/

//�������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С	
#define START_STK_SIZE 				512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//��ʼ����������   ���ڴ�����������ȵ�
void start_task(void *p_arg);


//�������ȼ�
#define GIZWITS_TASK_PRIO				3
//�����ջ��С	
#define GIZWITS_STK_SIZE 				256
//������ƿ�
OS_TCB GizwitsTaskTCB;
//�����ջ	
CPU_STK GIZWITS_TASK_STK[GIZWITS_STK_SIZE];
//��ʼ����������   ���ڴ�����������ȵ�
void gizwits_task(void *p_arg);


//NRF24L01�����շ��������ȼ�
#define RX_TX_TASK_PRIO				4
//�����ջ��С	
#define RX_TX_STK_SIZE 				128
//������ƿ�
OS_TCB RX_TX_TaskTCB;
//�����ջ	
CPU_STK RX_TX_TASK_STK[RX_TX_STK_SIZE];
////NRF24L01�����շ�
void RX_TX_task(void *p_arg);


//���������������
//�����������ȼ�
#define AIRQUALITY_TASK_PRIO 				5
//�����ջ��С
#define AIRQUALITY_STK_SIZE				128
//������ƿ�
OS_TCB AIRQUALITYTaskTCB;
//�����ջ
CPU_STK AIRQUALITY_TASK_STK[AIRQUALITY_STK_SIZE];
//���������������
void air_quality_task(void *p_arg);

//�����������ȼ�
#define HC_SR501_TASK_PRIO 				6
//�����ջ��С
#define HC_SR501_STK_SIZE				  64
//������ƿ�
OS_TCB HC_SR501TaskTCB;
//�����ջ
CPU_STK HC_SR501_TASK_STK[HC_SR501_STK_SIZE];
void HC_SR501(void *p_arg);

//DHT11����
//�����������ȼ�
#define DHT11_TASK_PRIO			6
//�����ջ��С
#define DHT11_STK_SIZE			128
//������ƿ�
OS_TCB DHT11TaskTCB;
//�����ջ
CPU_STK DHT11_TASK_STK[DHT11_STK_SIZE];
//��ʪ�ȼ������
void dht11_task(void *p_arg); 


//��ȼ��/�������ݴ�������
//�����������ȼ�
#define AIRDATADEAL_TASK_PRIO			7
//�����ջ��С
#define AIRDATADEAL_STK_SIZE			128
//������ƿ�
OS_TCB AIRDATADEALTaskTCB;
//�����ջ
CPU_STK AIRDATADEAL_TASK_STK[AIRDATADEAL_STK_SIZE];
//�����������ݴ�������
void air_quality_deal_task(void *p_arg);

//�����������ݴ�������
//�����������ȼ�
#define FLAMMABLE_GAS_TASK_PRIO			8
//�����ջ��С
#define FLAMMABLE_GAS_STK_SIZE			128
//������ƿ�
OS_TCB FLAMMABLE_GASTaskTCB;
//�����ջ
CPU_STK FLAMMABLE_GAS_TASK_STK[FLAMMABLE_GAS_STK_SIZE];
//��ȼ��\����������
void flammable_gas_task(void *p_arg);

//�����������ȼ�
#define LIGHT_INTENSITY_TASK_PRIO			9
//�����ջ��С
#define LIGHT_INTENSITY_STK_SIZE			64
//������ƿ�
OS_TCB LIGHT_INTENSITYTaskTCB;
//�����ջ
CPU_STK LIGHT_INTENSITY_TASK_STK[LIGHT_INTENSITY_STK_SIZE];
void light_intensity(void *p_arg);

//�����������ȼ�
#define ALARM_TASK_PRIO			10
//�����ջ��С
#define ALARM_STK_SIZE			64
//������ƿ�
OS_TCB  AlarmTaskTCB;
//�����ջ
CPU_STK ALARM_TASK_STK[ALARM_STK_SIZE];
void alarmset_task(void *pdata);

//�����������ȼ�
#define BEEP_TASK_PRIO			11
//�����ջ��С
#define BEEP_STK_SIZE			  64
//������ƿ�
OS_TCB  BeepTaskTCB;
//�����ջ
CPU_STK BEEP_TASK_STK[BEEP_STK_SIZE];
void beep_task(void *pdata);

////�����������ȼ�
//#define WIN_AUTO_TASK_PRIO			12
////�����ջ��С
//#define WIN_AUTO_STK_SIZE			  64
////������ƿ�
//OS_TCB  WinAUtoTaskTCB;
////�����ջ
//CPU_STK WINAUTO_TASK_STK[WIN_AUTO_STK_SIZE];
//void win_auto_task(void *p_arg);


//�����������ȼ�
#define SCREENLIGHT_TASK_PRIO			5                 //�����ȼ�12
//�����ջ��С
#define SCREENLIGHT_STK_SIZE			128
//������ƿ�
OS_TCB SCREENLIGHTTaskTCB;
//�����ջ
CPU_STK SCREENLIGHT_TASK_STK[SCREENLIGHT_STK_SIZE];
//��Ļ�Զ�Ϩ������������������
void screen_LightIsOffOrON(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				13
//�����ջ��С
#define TOUCH_STK_SIZE				128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//EMWINDEMO����
//�����������ȼ�
#define EMWINDEMO_TASK_PRIO			14
//�����ջ��С
#define EMWINDEMO_STK_SIZE			4096
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task����
void emwindemo_task(void *p_arg);

void MainTask(void);
void Gizwits_Init(void);     //������Э����س�ʼ��
void SmartHomeIOInit(void);  //����GPIO�ĳ�ʼ��

int main(void)
{
  OS_ERR err;
	CPU_SR_ALLOC();
  
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��Ϊ180Mhz   
	HAL_Init();                     //��ʼ��HAL��
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART1  ����printf
	uart2_init(9600);               //��ʼ��usart2  ���ڼҵ����app��ʵ��
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	DHT11_Init();                   //��ʼ��DHT11
	RTC_Init();                     //��ʼ��RTC
	NRF24L01_Init();                //��ʼ��NRF24L01
	MY_ADC_Init();                  //��ʼ��ADC
	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //����WAKE UP�ж�,1�����ж�һ�� 
	SDRAM_Init();                   //SDRAM��ʼ��
	TFTLCD_Init();  		           //LCD��ʼ��
	AP3216C_Init();                   //���մ�������ʼ��
	TIM3_PWM_Init(500-1,90-1);      //��PWM������LCD���� 90M/90=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
	FTL_Init();                     //NAND��ʼ��
	TP_Init();				              //��������ʼ��
	SmartHomeIOInit();              //���ƵƲ�����GPIO��ʼ��
	
	Gizwits_Init();                //������Э���ʼ��
	gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link ģʽ����
//	NRF24L01_TX_Mode();  //����ΪNRF24L01����ģʽ
	my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
	
	exfuns_init();			        //Ϊfatfs��ر��������ڴ�				 
  f_mount(fs[0],"0:",1); 		    //����SD�� 
	
	while(font_init())		//��ʼ���ֿ�
	{
		printf("Font Error!\n\r");
		while(SD_Init())	//���SD��
		{
			printf("SD Card Failed!\r\n");
			delay_ms(200);
			printf("SD Card Failed!\r\n");;
			delay_ms(200);		    
		}
		update_font(30,90,16,"0:");	//����ֿⲻ���ھ͸����ֿ�
		delay_ms(2000);
		LCD_Clear(WHITE);	//����
		break;
	}
	
	while(NRF24L01_Check())
	{
		printf("NRF24L01 is not exist!\r\n");
		delay_ms(200);
	}
	
	while(PCF8574_Init())                 //IO��չ��ʼ��  ���Ʒ�����
	{
		printf("PCF8574 is not exist!\r\n");
		delay_ms(200);
	}
    
  OSInit(&err);		            //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();            //�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	            //�˳��ٽ���	 
	OSStart(&err);                  //����UCOSIII
	while(1);
}

extern OS_TMR 	g_AutoLight;
extern OS_FLAG_GRP  AlaFlagGrp;
//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������ CPUʹ����               
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//ʹ��CRCʱ��  STM32ʹ��emwin�����CRCʱ��

	OS_CRITICAL_ENTER();	       //�����ٽ���	

	//�����¼��¼���־��
	OSFlagCreate((OS_FLAG_GRP*)&ScreenLightEventFlags,		//ָ���¼���־��
                 (CPU_CHAR*	  )"ScreenLightEventFlags",	//����
                 (OS_FLAGS	  )0,	//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);			//������	
								 
	//���������¼��¼���־��
	OSFlagCreate((OS_FLAG_GRP*)&AlaFlagGrp,		//ָ���¼���־��
                 (CPU_CHAR*	  )"AlaFlagGrp",	//����
                 (OS_FLAGS	  )0,	//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);			//������	
								 
		//STemWin gui����							
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
								 
		//�������ݷ�����������
	OSTaskCreate((OS_TCB*     )&RX_TX_TaskTCB,		
								(CPU_CHAR*   )"RX_TX task", 		
                 (OS_TASK_PTR )RX_TX_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )RX_TX_TASK_PRIO,     
                 (CPU_STK*    )&RX_TX_TASK_STK[0],	
                 (CPU_STK_SIZE)RX_TX_STK_SIZE/10,	
                 (CPU_STK_SIZE)RX_TX_STK_SIZE,		
                 (OS_MSG_QTY  )10,					      //NRF24L01�����ڽ���Ϣ���г���6
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			
								 
		//��ȼ������
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
	//����������
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
	//DHt11����
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
								 
	//�����������ݼ������
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
	 //��������Զ�Ϩ������					 
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
			//����ǿ�Ȳɼ�����			 
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
								 
		//������Э�鴦������			 
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
		
		//�������ܴ�������			 
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
		
		//������ģ�ⱨ������			 
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
		
		//��������			 
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
		
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWIND  GUI����
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
//�������ϱ���������
void gizwits_task(void *p_arg)
{
	p_arg=p_arg;
	OS_ERR err;
	while(1)
	{ 
		OSSchedLock(&err);  //�������
		gizwitsHandle((dataPoint_t *)&currentDataPoint);   //������Э�鴦��
		userHandle();         //�û����ݲɼ�
		OSSchedUnlock(&err);	//�������	
		
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);//��ʱ200ms			
	}
}

//��ʪ������
void dht11_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		DHT11_Read_Data(&temperature,&humitiy);	           //��ʪ�Ȳɼ�
#if 0 
		printf("�¶�:%d,ʪ��:%d\r\n",temperature,humitiy);     //������Ϣ��ӡ
#endif
		LED0 = !LED0;                                      //��־ϵͳ����
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms		
	}
}

extern unsigned char g_ucSwitchAirStatus; 
extern float g_Air_test;
u16 air_adc_val;
//���������������
void air_quality_task(void *p_arg)
{
	OS_ERR err;
	float temp;

	while(1)
	{
		air_adc_val = Get_Adc_Average(ADC_CHANNEL_5,10);  //�ɼ�adc1ͨ��5��ֵ
		temp=(float)air_adc_val*(3.3/4096);               //�������Ӧ�ĵ�ѹֵ	3.3�ǲο���ѹ��4096��ADC��12λ��2^12 = 4096
		temp = temp * 2 ;
		g_Air_test = temp;
#if 0
		sprintf(buf,"��ѹ��%f",g_Air_test);     //������Ϣ��ӡ
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
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//��ʱ100ms
		
	}
}

unsigned char AlaSetFlag = 0;  //����������־λ
extern float  g_GasConcentration;
extern unsigned char g_ucSwitchMQ2Status;
u16 gas_adc_val;
unsigned char GizwitsGasFlag = 0;
//��ȼ����������
void flammable_gas_task(void *p_arg)
{
	OS_ERR err;

	float temp;

	while(1)
	{
		gas_adc_val = Get_Adc_Average(ADC_CHANNEL_0,10);  //�ɼ�adc1ͨ��10��ֵ
		temp=(float)gas_adc_val * (3.3 / 4096);   		//�������Ӧ�ĵ�ѹֵ	
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
			OSFlagPost((OS_FLAG_GRP*)&AlaFlagGrp,      //���ͱ����¼���־
							(OS_FLAGS	  )ALA_SET,
							(OS_OPT	  )OS_OPT_POST_FLAG_SET,
							(OS_ERR*	  )&err);
			GizwitsGasFlag = 1;
		}
		else
		{
			PCF8574_WriteBit(BEEP_IO,1);	//�رշ�����
			AlaSetFlag = 0;
			GizwitsGasFlag = 0;
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//��ʱ100ms
//		
	}
}

extern unsigned int LightVal;
extern unsigned char g_ScreenLightTime;
extern unsigned char g_ucAutoToCloseScreenLightFlag;
extern unsigned char SrceenOffFlag;
/*ʵ����Ļ�Զ�Ϩ��������������*/
void screen_LightIsOffOrON(void *p_arg)
{
	static u32 countSec = 0;
	u32 x,y;
	OS_FLAGS flags_num;
	OS_ERR err;
	p_arg = p_arg;
	for(;;)
	{
		flags_num = OSFlagPend((OS_FLAG_GRP*)&ScreenLightEventFlags,   //�յ�Ϩ���ź���
													 (OS_FLAGS	) SCREEN_OFF_FLAG, 
													 (OS_TICK     )0xf8,
													 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_NON_BLOCKING,
													 (CPU_TS*     )0,
													 (OS_ERR*	    )&err);
		if(flags_num & SCREEN_OFF_FLAG)  //�յ�Ϩ���ź���
		{
			TIM_SetTIM3Compare2(0);   //Ϩ����Ļ
			SrceenOffFlag = 1;        //��ĻϨ���־λ��1
			LightVal = 0;
		}
		if(g_ucAutoToCloseScreenLightFlag || SrceenOffFlag)  //�����������Ļ�Զ��رջ���Ļ����Ĳ�ɨ�败�����Ƿ��д���
		{
			if(x != GUI_TOUCH_X_MeasureX() && GUI_TOUCH_X_MeasureY() != y)        //����д���
			{		
				x = GUI_TOUCH_X_MeasureX();//������һ�εĴ���ֵ
				y = GUI_TOUCH_X_MeasureY();
				countSec = 0;                //��ʱ��������
//				LED0 = !LED0;
				if(SrceenOffFlag)
				{
					SrceenOffFlag = 0;
					TIM_SetTIM3Compare2(500);  //������Ļ
				}
				LightVal = 500;
			}
			else if(g_ScreenLightTime > 0 && g_ucAutoToCloseScreenLightFlag)
			{
				countSec++;
				if(countSec >= g_ScreenLightTime*60)         //���ﶨʱ����ʱ��
				{
					 SrceenOffFlag = 1;
					 TIM_SetTIM3Compare2(0);                   //Ϩ����Ļ
				}
			}
		}
	#if 0
		printf("x=%d,y=%d\n",GUI_TOUCH_X_MeasureX(),GUI_TOUCH_X_MeasureY());
	#endif
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);       //��ʱ1s
	}
}

unsigned short g_uAls;  //����ǿ��
extern unsigned char LampAutoFlag;
extern unsigned char LED_flag;
/* ����ǿ�Ȳɼ����Զ��ص��Լ������Կ�����*/
void light_intensity(void *p_arg)
{
	u16 ir,ps;
	OS_ERR err;
	unsigned char CountON;  //��������ǿ�ȴﵽһ���̶�ĳ�������ſ���
	unsigned char countOFF; //��������ǿ�ȴﵽһ���̶�ĳ�������Źص�

	static unsigned char AutoLampOnFlag = 0;
	p_arg = p_arg;
	while(1)
	{	
		AP3216C_ReadData(&ir,&ps,&g_uAls);  //�ɼ�����ǿ��
#if 0
		printf("light = %d\r\n",als);
#endif
		if(LampAutoFlag && g_uAls <= 10 && AutoLampOnFlag == 0)  //�����Զ��ص��Լ�����ǿ��С��10cd          
		{
			CountON++;
			if(CountON >= 10)
			{
				CountON = 0;
				if(LED_flag == 0) //������ǹ��˵�
				{
					LAMP_SW = 1;    //����
					LED_flag = 1;
					AutoLampOnFlag = 1;
				}
			}
			
		}
		if(g_uAls > 10 && LampAutoFlag && AutoLampOnFlag)  //�����ǿ����10cd�Ϳ������Զ��ص�
		{
			countOFF++;
			if(countOFF >= 10)
			{
				countOFF = 0;
				if(LED_flag)   //������ǿ��˵�
				{
					LED_flag = 0;
					LAMP_SW = 0;   //�ص�
					AutoLampOnFlag = 0;
				}
			}
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);       //��ʱ1s
	}
}

//NRF24L01���ݷ�������
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
												 &msg_size,   /* �˲����ǽ��յ����ݸ��� */
												 &ts,
												 &err);
		OSSchedLock(&err);  //����������
		NRF24L01_TX_Mode();   //���÷���ģʽ
		NRF24L01_TxPacket((unsigned char*)g_cRX_TX_Buf); //��������   
		OSSchedUnlock(&err);	//����������
	}
}

extern  unsigned char CountAla;

//��������app��������
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
											 &msg_size,   /* �˲����ǽ��յ����ݸ��� */
											 &ts,
											 &err);
		if(strcmp(Buf,"ALAON") == 0)  //���������������
		{
			ENABLE_ALA;           //��������
		}
		if(strcmp(Buf,"ALAOFF") == 0)
		{       
			DISABLE_ALA;           //�رձ���
			PCF8574_WriteBit(BEEP_IO,1);	//�رշ�����
			AlaSetFlag = 0;
		}
#if 0
		printf("%s\n\r",Buf);
#endif
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//��ʱ1S
	}
}



//HC_SR501��������ⱨ������
void HC_SR501(void *p_arg)
{
	OS_ERR err;
	OS_FLAGS flags_num;
	
	while(1)
	{
		flags_num = OSFlagPend((OS_FLAG_GRP*)&AlaFlagGrp,   //�յ�����������־
													 (OS_FLAGS	) ALA_SET, 
													 (OS_TICK     )0,
													 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME ,  //+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_NON_BLOCKING
													 (CPU_TS*     )0,
													 (OS_ERR*	    )&err);
		if(flags_num & ALA_SET)  //�����⵽���͵紫�����ź�
		{
			AlaSetFlag = 1;       //������־λ��1
		}		
	}
}

//��������������   ���������������ȼ�����ᱨ��
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
			beepsta=!beepsta;					//������״̬ȡ��
			PCF8574_WriteBit(BEEP_IO,beepsta);	//���Ʒ�����
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);//��ʱ100ms
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}



//TOUCH����
void touch_task(void *p_arg) 
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}


//OSStatTaskCPUUsage  CPUʹ����
