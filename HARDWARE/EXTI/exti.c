#include "exti.h"
#include "delay.h" 
	   
/****************************************************************************
* ��    ��: void EXTIX_Init(void)
* ��    �ܣ��ⲿ�жϳ�ʼ������
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void EXTIX_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              //ʹ��SYSCFGʱ��
    RCC_AHB1PeriphClockCmd(EXTI_GPIO_TIMER, ENABLE);                    //ʹ��EXTI_PORTʱ��
 
    GPIO_InitStructure.GPIO_Pin = EXTI_PORT_PIN;                        //KEY��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                        //��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //����
    GPIO_Init(EXTI_PORT, &GPIO_InitStructure);                          //��ʼ��EXTI_PORT_PIN
	
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE1);    //PF6 ���ӵ��ж���6
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE2);    //PF7 ���ӵ��ж���7
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE3);    //PF8 ���ӵ��ж���8
	
	EXTI_InitStructure.EXTI_Line = EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                 //�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //�ж���ʹ��
    EXTI_Init(&EXTI_InitStructure);                                     //����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;                          //EXTI9_5_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_PRE_PRIORITY;   //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_SUB_PRIORITY;          //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                                             //����	   
}

/****************************************************************************
* ��    ��: void EXTI9_5_IRQHandler(void)
* ��    �ܣ��ⲿ�жϷ������
* ��ڲ�������
* ���ز�������
* ˵    ������ʱ 10 ms�������
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//����

	if(KEY2 == 1)	                         //���������º����IO�Ӹߵ�ƽ��Ϊ�͵�ƽ�����ⲿ�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line8);  //���LINE8�ϵ��жϱ�־λ  KEY1 ��ӦIO��PF8
	}	
	
	if(KEY1 == 0)	                         //���������º����IO�Ӹߵ�ƽ��Ϊ�͵�ƽ�����ⲿ�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE7�ϵ��жϱ�־λ  KEY2 ��ӦIO��PF7
	}	
	
	if(KEY0 == 0)	                         //���������º����IO�Ӹߵ�ƽ��Ϊ�͵�ƽ�����ⲿ�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line6);  //���LINE6�ϵ��жϱ�־λ  KEY3 ��ӦIO��PF6
	}
	
}

