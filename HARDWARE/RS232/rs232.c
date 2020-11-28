#include "rs232.h"
#include "protocol.h"

volatile u8 RS232_Rxbuf[RS232_BUFF_LEN_MAX], RS232_Txbuf[RS232_BUFF_LEN_MAX], received_RS232_len = 0;

/****************************************************************************
* ��    ��: void RS232_Init(u32 bound)
* ��    �ܣ�RS232���߳�ʼ��
* ��ڲ�����bound ������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void RS232_Init(u32 bound)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RS232_USART_PORT_TIMER,ENABLE);     //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RS232_USART_TIMER,ENABLE);          //ʹ��USART1ʱ��
 
	GPIO_PinAFConfig(RS232_USART_PORT,RS232_USART_PORT_GPIO_PinSource_TX,RS232_USART_PORT_GPIO_AF);  //GPIOA9���Ÿ���ΪUSART1 TX
	GPIO_PinAFConfig(RS232_USART_PORT,RS232_USART_PORT_GPIO_PinSource_RX,RS232_USART_PORT_GPIO_AF);  //GPIOA10���Ÿ���ΪUSART1 RX
	
    GPIO_InitStructure.GPIO_Pin = RS232_USART_PORT_TX | RS232_USART_PORT_RX;       //RS232_USART1��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                           //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                   //����
	GPIO_Init(RS232_USART_PORT,&GPIO_InitStructure);                               //��ʼ��PA9|PA10
	
	USART_InitStructure.USART_BaudRate = bound;                                     //RS232_USART1��ʼ������ ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //�շ�ģʽ
    USART_Init(RS232_USART, &USART_InitStructure);   

    USART_Cmd(RS232_USART, ENABLE);                                                //ʹ��RS232_USART1
	USART_ClearFlag(RS232_USART, USART_FLAG_TC);
    
    USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);                            //USART NVIC ����  �ж�ʹ��
    NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_NVIC_IRQChannel;              //ʹ��RS232_USART1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=RS232_PRE_PRIORITY;       //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =RS232_SUB_PRIORITY;		       //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                               //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

/****************************************************************************
* ��    ��: void RS232_USART_IRQHandler(void)   
* ��    �ܣ�USART1�����жϷ�����
* ��ڲ�������
* ���ز�������
* ˵    ���� 
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void RS232_USART_IRQHandler(void)                	
{
	u8 Res;
    static u8 RxCnt = 0;
	if(USART_GetITStatus(RS232_USART, USART_IT_RXNE) != RESET)   //�����ж�
	{
		Res = USART_ReceiveData(RS232_USART);                     //(USART->DR);	//��ȡ���յ������� 
        RS232_Rxbuf[ RxCnt ] = Res;
        if(RxCnt == 0)
        {
            RxCnt = (HEAD1 != Res) ? 0 : RxCnt + 1;
        }
        else if(RxCnt == 1)
        {
            RxCnt = (HEAD2 != Res) ? 0 : RxCnt + 1;
        }
        else if(RxCnt == 2)
        {
            received_RS232_len = Res;
            RxCnt++;
        }
        else if(RxCnt > 2)
        {
            RxCnt++;
            if(RxCnt == received_RS232_len + 3)
            {
                protocol_RS232(RS232_Rxbuf,received_RS232_len);
                received_RS232_len = 0;
                RxCnt = 0;
            }     
        }   
    } 
}

/****************************************************************************
* ��    ��: void RS232_Send_Data(volatile const u8* send_buff,u8 length)
* ��    �ܣ�RS485����length���ֽ�
* ��ڲ�������
* ���ز�������
* ˵    ����(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�����ݳ��Ȳ�Ҫ����128���ֽ�)     
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void RS232_Send_Data(volatile const u8* TX_buff,u8 length)
{	   
	u8 t;
  	for(t=0;t<length;t++)		                                         //ѭ����������
	{
        while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC) == RESET); //�ȴ����ͽ���		
        USART_SendData(RS232_USART, TX_buff[t]);                        //��������
	}	 
	while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC) == RESET);     //�ȴ����ͽ���		     
}  
