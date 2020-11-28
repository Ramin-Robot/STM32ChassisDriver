#include "can.h"
#include "feedback.h"
#include "usart.h"	

u8 CAN_Txbuf[CAN_BUFF_LEN_MAX];

/****************************************************************************
* ��    ��: void CAN1_Configuration(void)
* ��    �ܣ�CAN��ʼ��
* ��ڲ�������  
* ���ز�������
* ˵    ����mode Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
            baud = 42M / (CAN_SJW + CAN_BS1 + CAN_BS2) / CAN_Prescaler
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void CAN1_Configuration(void)
{
    GPIO_InitTypeDef       GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);       //ʹ��PORTBʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);        //ʹ��CAN1ʱ��	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;       //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);                      //��ʼ��PB8,PB9
	
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);      //GPIOB8���Ÿ���ΪCAN1H
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);      //GPIOB9���Ÿ���ΪCAN1L
	  
   	CAN_InitStructure.CAN_TTCM=DISABLE;	                        //��ʱ�䴥��ͨ��ģʽ     	//CAN��Ԫ����
  	CAN_InitStructure.CAN_ABOM=DISABLE;	                        //����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;                         //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	                        //��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	                        //���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	                        //���ȼ��ɱ��ı�ʶ������ 
    
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;                //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	                    //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq;                     //ʱ���1��ʱ�䵥Ԫ.  Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;                      //ʱ���2��ʱ�䵥Ԫ.  Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=6;                          //��Ƶϵ��(Fdiv)Ϊbrp+1	 1 - 1024
  	CAN_Init(CAN1, &CAN_InitStructure);                         //��ʼ��CAN2
    
    CAN_FilterInitStructure.CAN_FilterNumber=0;	                        //������0����
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;      //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                    //32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;                //32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;  //������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;                //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);                           //�˲�����ʼ��
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;           //�����ȼ�Ϊ2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                  //�����ȼ�Ϊ2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ��    ��: u8 CAN1_Send_Msg(u16 stdid,u8 ext_en u16 extid,u8 remote_en, u8* msg, u8 len)
* ��    �ܣ�can����һ������(��׼֡,����֡)
* ��ڲ�����stdid ��׼��ʶ�� extid ��չ��ʾ�� len:���ݳ���(���Ϊ8)  msg:����ָ��,���Ϊ8���ֽ�
* ���ز�����0,�ɹ�;����,ʧ��;
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
u8 CAN1_Send_Msg(u16 stdid, u8 ext_en, u16 extid, u8 remote_en, u8* msg, u8 len)
{	
    u8 mbox;
    u16 i=0;
    CanTxMsg TxMessage;
    TxMessage.StdId=stdid;	                    // ��׼��ʶ��
    if(ext_en)
    {
        TxMessage.ExtId=extid;	                // ��չ��ʾ����29λ��
        TxMessage.IDE=CAN_ID_EXT;		        // ʹ����չ��ʶ��
    }
    else
    {
        TxMessage.IDE=CAN_ID_STD;		        // ʹ�ñ�׼��ʶ��        
    }
    if(remote_en)
    {
        TxMessage.RTR=CAN_RTR_Remote;		    // ��Ϣ����Ϊң��֡
    }
    else
    {
        TxMessage.RTR=CAN_RTR_DATA;		        // ��Ϣ����Ϊ����֡��һ֡8λ        
    }

    TxMessage.DLC=len;						
    for(i=0;i<len;i++)
    {
        TxMessage.Data[i]=msg[i];				// ��һ֡��Ϣ          
    }
    mbox= CAN_Transmit(CAN1, &TxMessage);   
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
    if(i>=0XFFF)return 1;
    return 0;			
}

/****************************************************************************
* ��    ��: void CAN1_RX0_IRQHandler(void)
* ��    �ܣ�CAN1�����жϺ���
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    int32_t temp;
    CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);  /* �˺��������ͷ���������˵�,�ڷǱ�Ҫʱ,����Ҫ�Լ��ͷ� */
	if(RxMessage.StdId == 0x181)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            ChassisDrvFB.A_WH_v = (int16_t)(temp*100*0.83/42.17);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x181 DLC error !\r\n");
    }
    else if(RxMessage.StdId == 0x182)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            ChassisDrvFB.B_WH_v = (int16_t)(-temp*100*0.83/42.17);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x182 DLC error !\r\n");
    }
    else if(RxMessage.StdId == 0x183)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            BotArmFB.RDArm_Deg = (uint16_t)(temp%415000);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x183 DLC error !\r\n");        
    }
    else if(RxMessage.StdId == 0x184)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            BotArmFB.LDArm_Deg = (uint16_t)(temp%415000);  
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x184 DLC error !\r\n");        
    }
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);  /* ��������ж� */
}





