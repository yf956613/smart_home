/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "usart3.h"
#include "timer.h"
#include "dht11.h"

static uint32_t timerMsCount;
//uint8_t aRxBuffer;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;


/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_FangDaoBaoJin_SW:
        currentDataPoint.valueFangDaoBaoJin_SW = dataPointPtr->valueFangDaoBaoJin_SW;
        GIZWITS_LOG("Evt: EVENT_FangDaoBaoJin_SW %d \n", currentDataPoint.valueFangDaoBaoJin_SW);
        if(0x01 == currentDataPoint.valueFangDaoBaoJin_SW)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_LED_ON_OFF:
        currentDataPoint.valueLED_ON_OFF = dataPointPtr->valueLED_ON_OFF;
        GIZWITS_LOG("Evt: EVENT_LED_ON_OFF %d \n", currentDataPoint.valueLED_ON_OFF);
        if(0x01 == currentDataPoint.valueLED_ON_OFF)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_Socket_ON_OFF:
        currentDataPoint.valueSocket_ON_OFF = dataPointPtr->valueSocket_ON_OFF;
        GIZWITS_LOG("Evt: EVENT_Socket_ON_OFF %d \n", currentDataPoint.valueSocket_ON_OFF);
        if(0x01 == currentDataPoint.valueSocket_ON_OFF)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_Curtain:
        currentDataPoint.valueCurtain = dataPointPtr->valueCurtain;
        GIZWITS_LOG("Evt: EVENT_Curtain %d \n", currentDataPoint.valueCurtain);
        if(0x01 == currentDataPoint.valueCurtain)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;




      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}


//Gizwits协议初始化
void Gizwits_Init(void)
{
	TIM2_Init(10-1,9000-1); //1MS系统定时 
	uart3_init(9600);//WIFI初始化 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
	
}
/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/

extern u16 air_adc_val;
extern u16 gas_adc_val;
void userHandle(void)
{
	currentDataPoint.valuetemperature = temperature;//Add Sensor Data Collection
	currentDataPoint.valuehumidity = humitiy;//Add Sensor Data Collection
	currentDataPoint.valueAirQuality = air_adc_val;//Add Sensor Data Collection
	currentDataPoint.valueKeRanQiYanWu = gas_adc_val;//Add Sensor Data Collection
	currentDataPoint.valueKeRanQiYanWu_Alarm = 0;//Add Sensor Data Collection
	currentDataPoint.valueFangDaoBaojin = 0;//Add Sensor Data Collection
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueFangDaoBaoJin_SW = ;
      currentDataPoint.valueLED_ON_OFF = ;
      currentDataPoint.valueSocket_ON_OFF = ;
      currentDataPoint.valueCurtain = ;
      currentDataPoint.valuetemperature = ;
      currentDataPoint.valuehumidity = ;
      currentDataPoint.valueAirQuality = ;
      currentDataPoint.valueKeRanQiYanWu = ;
      currentDataPoint.valueKeRanQiYanWu_Alarm = ;
      currentDataPoint.valueFangDaoBaojin = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART1 and Loop until the end of transmission */
//  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
// 
//  return ch;
//}

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim==&htim2)
//	{
////			keyHandle((keysTypedef_t *)&keys);
//			gizTimerMs();
//	}
//}

///**
//* @brief Timer TIM3 init function

//* @param none
//* @return none
//*/
//void timerInit(void)
//{
//	HAL_TIM_Base_Start_IT(&htim2);
//}

/**
  * @brief  This function handles USART IDLE interrupt.
  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)  
//{  
//    if(UartHandle->Instance == USART2)  
//    {  
//				gizPutData((uint8_t *)&aRxBuffer, 1);

//        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
//    }  
//}  

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
//void uartInit(void)
//{
//	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
//}

/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
                //USART_SendData(UART, buf[i]);//STM32 test demo
        //实现串口发送函数,将buf[i]发送到模组
				HAL_UART_Transmit(&UART3_Handler,&buf[i],1,1000);
        while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TXE)== RESET);

        if(i >=2 && buf[i] == 0xFF)
        {
						//实现串口发送函数,将0x55发送到模组
					HAL_UART_Transmit(&UART3_Handler,crc,1,1000);
          while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TXE)==RESET);
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
}  
