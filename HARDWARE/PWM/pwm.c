#include "pwm.h"
 
 /****************************************************************************
* ��    ��: void PWM_INIT(u32 arr,u32 psc)
* ��    �ܣ�PWM��ʼ��
* ��ڲ�����arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
* ���ز�����
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void PWM_INIT(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(PWM_TIM_CLK,ENABLE);  	                //TIMʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(PWM_PORT_TIMER, ENABLE); 	            //ʹ��PORTʱ��	

	GPIO_PinAFConfig(PWM_PORT,PWM_PIN_SOURCE1,PWM_PIN_AF);          //GPIO����Ϊ��ʱ��

	GPIO_InitStructure.GPIO_Pin = PWM_PIN1;                         //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	            //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //����
	GPIO_Init(PWM_PORT,&GPIO_InitStructure);                   
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                        //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                           //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseStructure);               //��ʼ����ʱ��3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //��ʼ��TIM3 Channel 1 PWMģʽ ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);                     //����Tָ���Ĳ�����ʼ������TIM3OC1
	TIM_OC1PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);            //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(PWM_TIM,ENABLE);                           //ARPEʹ�� 
	
	TIM_Cmd(PWM_TIM, ENABLE);              		  
}    

