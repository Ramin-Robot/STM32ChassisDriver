#include "spi.h"
#include "delay.h"

/****************************************************************************
* ��    ��: void SPI2_Init(void)
* ��    �ܣ���ʼ��SPI
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void SPI2_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
    RCC_AHB1PeriphClockCmd(SPI_PORT_TIMER, ENABLE);                             //ʹ��GPIOBʱ��
    RCC_APB1PeriphClockCmd(SPI_TIMER, ENABLE);                                  //ʹ��SPI2ʱ��
 
    GPIO_InitStructure.GPIO_Pin = SPI_PORT_PIN;                                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                              //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                          //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                //����
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);                                   //��ʼ��
	
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_SCL,SPI_PORT_GPIO_AF);    //����Ϊ SPI SCL
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_MISO,SPI_PORT_GPIO_AF);   //����Ϊ SPI MISO
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_MOSI,SPI_PORT_GPIO_AF);   //����Ϊ SPI MOSI
 
	RCC_APB1PeriphResetCmd(SPI_TIMER,ENABLE);                                   //��λSPI2
	RCC_APB1PeriphResetCmd(SPI_TIMER,DISABLE);                                  //ֹͣ��λSPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����lSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ
	SPI_Init(SPI, &SPI_InitStructure);                                  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI, ENABLE);                                               //ʹ��SPI����	 
}   

/****************************************************************************
* ��    ��: u8 SPI2_ReadByte(u8 TxData)
* ��    �ܣ�SPI��ȡһ���ֽ�
* ��ڲ�����TxData д����������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
u8 SPI2_ReadByte(u8 TxData)
{		 			 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}    //�ȴ���������  
	SPI_I2S_SendData(SPI2, TxData);                                     //ͨ������SPIx����һ��byte  ����
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){}   //�ȴ�������һ��byte	
	return SPI_I2S_ReceiveData(SPI2);                                   //����ͨ��SPIx������յ�����		    	
}

/****************************************************************************
* ��    ��: void SPI2_WriteByte(u8 TxData)
* ��    �ܣ�SPIд��һ���ֽ�
* ��ڲ�����TxData д����������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void SPI2_WriteByte(u8 TxData)
{		 			 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}    //�ȴ���������  
	SPI_I2S_SendData(SPI2, TxData);                                     //ͨ������SPIx����һ��byte  ����
}

/****************************************************************************
* ��    ��: void SPI2_Write(u8 TxData1 ,u16 TxData2)
* ��    �ܣ�SPIд���ض�����
* ��ڲ�����TxData1 TxData2 д�����ݵ�����
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void SPI2_Write(u8 TxData1 ,u16 TxData2)
{
	SPI2_CS=1;
    SPI2_WriteByte(TxData1);    //A0~A5  C0:0Write 1Read   C1:0  D0~D15
	SPI2_WriteByte(TxData2);
	SPI2_WriteByte(TxData2>>8);
	delay_us(5);
	SPI2_CS=0;
}

/****************************************************************************
* ��    ��: u16 SPI2_Read(u8 TxData)
* ��    �ܣ�SPI��ȡ�ض�����
* ��ڲ�����TxData ���ȡ��ַ
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
u16 SPI2_Read(u8 TxData)
{
	u16 RxData=0,RxData1=0,RxData2=0;
	SPI2_CS=1;
	SPI2_WriteByte(TxData|0x40);
	RxData1=SPI2_ReadByte(0xFF);
	RxData2=SPI2_ReadByte(0xFF);
	RxData=RxData1|(RxData2<<8);
	delay_us(5);
	SPI2_CS=0;	
	return RxData;
}






