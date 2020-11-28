#include "usart.h"	

u8 Debug_rx_buff[DEBUG_USART_LEN_MAX],USART_RX_STA; //���ջ���,���DEBUG_USART_LEN_MAX���ֽ�.ĩ�ֽ�Ϊ���з� 

#pragma import(__use_no_semihosting)        //�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
         
struct __FILE                               //��׼����Ҫ��֧�ֺ���        
{ 
    int handle; 
}; 

FILE __stdout;       

_sys_exit(int x)                            //����_sys_exit()�Ա���ʹ�ð�����ģʽ    
{ 
    x = x; 
} 

int fputc(int ch, FILE *f)                  //�ض���fputc���� 
{ 	
    while((DEBUG_USART->SR&0X40)==0);            //ѭ������,ֱ���������   
    DEBUG_USART->DR = (u8) ch;   
	return ch;
}

/****************************************************************************
* ��    ��: void RS485_Init(u32 bound)
* ��    �ܣ�RS485���߳�ʼ��
* ��ڲ�����bound ������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void DEBUG_USART_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DEBUG_USART_PORT_TIMER,ENABLE);     //ʹ��DEBUG_USART_PORTʱ��
	RCC_APB1PeriphClockCmd(DEBUG_USART_TIMER,ENABLE);          //ʹ��DEBUG_USARTʱ�� 
 
    GPIO_PinAFConfig(DEBUG_USART_PORT,DEBUG_USART_PORT_GPIO_PinSource_TX,DEBUG_USART_PORT_GPIO_AF);   //GPIOB10���Ÿ���ΪUSART3 TX DEBUG_USART
	GPIO_PinAFConfig(DEBUG_USART_PORT,DEBUG_USART_PORT_GPIO_PinSource_RX,DEBUG_USART_PORT_GPIO_AF);   //GPIOB11���Ÿ���ΪUSART3 RX DEBUG_USART
    
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_PORT_TX | DEBUG_USART_PORT_RX;    //DEBUG_USART_PORT���� TX PB10 RX PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                        //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                              //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                //����
	GPIO_Init(DEBUG_USART_PORT,&GPIO_InitStructure);                            //��ʼ�� 

	USART_InitStructure.USART_BaudRate = bound;                                      //DEBUG_USART ��ʼ������ ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 //�շ�ģʽ
    USART_Init(DEBUG_USART, &USART_InitStructure);                                   //��ʼ��DEBUG_USART
	
    USART_Cmd(DEBUG_USART, ENABLE);                                         //DEBUG_USART
	USART_ClearFlag(DEBUG_USART, USART_FLAG_TC);
	
    USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);                     //DEBUG_USART NVIC ����  �ж�ʹ��
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_NVIC_IRQChannel;       //USART3-DEBUG_USART�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                 //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		                //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                        //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

/****************************************************************************
* ��    ��: void DEBUG_USART_IRQHandler(void)   
* ��    �ܣ�DEBUG_USART�����жϷ�����
* ��ڲ�������
* ���ز�������
* ˵    ����  
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void DEBUG_USART_IRQHandler(void)                	                //DEBUG_USART�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(DEBUG_USART);                    //(DEBUG_USART->DR);��ȡ���յ�������  	
		if((USART_RX_STA&0x8000)==0)                            //����δ���
		{
			if(USART_RX_STA&0x4000)                             //���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;                    //���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	                    //��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					Debug_rx_buff[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(DEBUG_USART_LEN_MAX-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		         
    } 
} 
/****************************************************************************
* ��    ��: void DEBUG_USART_Send(const u8* send_buff,u8 length)
* ��    �ܣ�debug����length���ֽ�
* ��ڲ�����send_buff �����׵�ַ length �������ݳ���
* ���ز�������
* ˵    ����(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�����ݳ��Ȳ�Ҫ����128���ֽ�)  
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void DEBUG_USART_Send(const u8* TX_buff,u8 length)
{
	u8 t;
  	for(t=0;t<length;t++)		                                      //ѭ����������
	{
        while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
        USART_SendData(DEBUG_USART,TX_buff[t]);                       //��������
	}	 
	while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET);     //�ȴ����ͽ���		  
}
 



