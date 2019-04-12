#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//SPI��������	   	
////////////////////////////////////////////////////////////////////////////////// 	 

SPI_HandleTypeDef SPI5_Handler;  //SPI5���
SPI_HandleTypeDef SPI2_Handler;  //SPI2���

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI5�ĳ�ʼ��
void SPI5_Init(void)
{
    SPI5_Handler.Instance=SPI5;                         //SP5
    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI5_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI5_Handler);
    
    __HAL_SPI_ENABLE(&SPI5_Handler);                    //ʹ��SPI5
    SPI5_ReadWriteByte(0Xff);                           //��������
}

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
{
    SPI2_Handler.Instance=SPI2;                      //SP2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;          //����SPI����ģʽ������Ϊ��ģʽ
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //�ر�TIģʽ
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI2_Handler.Init.CRCPolynomial=7;               //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI2_Handler);
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                 //ʹ��SPI2
    SPI2_ReadWriteByte(0Xff);                        //��������
}
//SPI5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();       //ʹ��GPIOFʱ��
    __HAL_RCC_SPI5_CLK_ENABLE();        //ʹ��SPI5ʱ��
    __HAL_RCC_SPI2_CLK_ENABLE();        //ʹ��SPI2ʱ��
    
    //SPI5���ų�ʼ��PF7,8,9
    GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����            
    GPIO_Initure.Alternate=GPIO_AF5_SPI5;           //����ΪSPI5
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //SPI2���ų�ʼ��PB13,14,15
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;   
    GPIO_Initure.Alternate=GPIO_AF5_SPI2;           //����ΪSPI2
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);             //��ʼ��
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI5_Handler);            //�ر�SPI
    SPI5_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI5_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI5_Handler);             //ʹ��SPI
    
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI2_Handler);            //�ر�SPI
    SPI2_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI2_Handler);             //ʹ��SPI
    
}

//SPI5 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI5_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI5_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI2_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}
