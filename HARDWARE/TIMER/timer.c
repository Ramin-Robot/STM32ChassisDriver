#include "timer.h"

uint8_t LED_Scan    = 0;
uint8_t IMU_Scan    = 0;
uint8_t MOTOR_Scan  = 0;
uint8_t MPU_Scan    = 0;
uint8_t IRQ_Scan    = 0;
uint8_t Batt_Scan   = 0;
uint8_t ANO_Scan    = 0;
uint8_t MODBUS_Scan = 0;

/****************************************************************************
* ��    ��: void SYS_BASE_TIMER_INIT(void)
* ��    �ܣ�ͨ�ö�ʱ��5�жϳ�ʼ��
* ��ڲ�������
* ���ز�������
* ˵    ������ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us. 
            Ft=��ʱ������Ƶ��,��λ:Mhz
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void SYS_BASE_TIMER_INIT(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(SYS_BASE_TIMER_CLK,ENABLE);                  //ʹ�ܶ�ʱ��ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	                    //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                        //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;       //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(SYS_BASE_TIMER,&TIM_TimeBaseInitStructure);        //��ʼ����ʱ��
	
	TIM_ITConfig(SYS_BASE_TIMER,TIM_IT_Update,ENABLE);                  //����ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel=SYS_BASE_TIMER_NVIC_IRQChannel;  //��ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PRE_PRIORITY;  //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=SUB_PRIORITY;         //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(SYS_BASE_TIMER,ENABLE);                                     //ʹ�ܶ�ʱ��
}

/****************************************************************************
* ��    ��: void SYS_BASE_TIMER_IRQ(void)
* ��    �ܣ���ʱ�����жϺ���
* ��ڲ�������
* ���ز�������
* ˵    ����ϵͳ��ʱʱ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void SYS_BASE_TIMER_IRQ(void)
{
    static uint16_t ms5 = 0, ms10 = 0, ms40 = 0, ms50 = 0,ms100 = 0, ms200 = 0, ms500 = 0; //��Ƶϵ��
	if(TIM_GetITStatus(SYS_BASE_TIMER,TIM_IT_Update)==SET) //����ж�
	{
		ms5++;
		ms10++;
        ms40++;
        ms50++;
		ms100++;
		ms200++;
		ms500++;
        
		ANO_Scan = 1;
		if(ms5 >= 5)            //200Hz
		{
			ms5 = 0;
			MPU_Scan = 1;
		}
		if(ms10 >= 10)          //100Hz
		{
			ms10 = 0;
			IMU_Scan = 1;
		}
        if(ms40 >= 40)          //25Hz
        {
            ms40 =0;
            MOTOR_Scan =1;
        }
		if(ms50 >= 50)          //20Hz
		{
			ms50 = 0;
		}
		if(ms100 >= 100)        //10Hz
		{
			ms100 = 0;
            MODBUS_Scan = 1;
		}
		if(ms200 >= 200)        //5Hz
		{
			ms200 = 0;
		}
		if(ms500 >= 500)        //2Hz
		{
			ms500 = 0;
			LED_Scan = 1;
		}
	}
	TIM_ClearITPendingBit(SYS_BASE_TIMER,TIM_IT_Update);  //����жϱ�־λ
}
