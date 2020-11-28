#include "led.h" 
#include "usart.h"	

/****************************************************************************
* ��    ��: void LED_Init(void)
* ��    �ܣ�LED��ʼ��
* ��ڲ�������
* ���ز�������
* ˵    ����С���ݳ�ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED_PORT_TIMER, ENABLE);     //ʹ��GPIOGʱ��

    GPIO_InitStructure.GPIO_Pin = LED_PORT_PIN;         //LED0/LED1/led2��ӦIO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(LED_PORT, &GPIO_InitStructure);           //��ʼ��GPIO
	
    GPIO_SetBits(LED_PORT,LED_PORT_PIN);                //���øߣ�����
    
    WOODS_DBG_PRINTF("END of LED_Init() !\r\n");
}

/****************************************************************************
* ��    ��: void LED_Light(void)
* ��    �ܣ�LED��˸
* ��ڲ�������
* ���ز�������
* ˵    ����С���ݳ�ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void LED_Light(void)
{
    static uint8_t count;
    switch (count)
    {
        case 0 :
            {
                LED0 = 0;
                LED1 = 1;
                LED2 = 1;
                count++;   
            }break;
        case 1 :
            {
                LED0 = 1;
                LED1 = 0;
                LED2 = 1;
                count++; 
            } break;
        case 2 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 0;
                count++; 
            } break;
        case 3 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 1;
                count++; 
            } break; 
        case 4 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 0;
                count++; 
            } break;          
        case 5 :
            {
                LED0 = 1;
                LED1 = 0;
                LED2 = 1;
                count++; 
            } break;
        case 6 :
            {
                LED0 = 0;
                LED1 = 1;
                LED2 = 1;
                count++; 
            } break; 
        case 7 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 1;
                count = 0; 
            } break;             
        default :
            break; 
    }
    WOODS_DBG_PRINTF("led is lighting !\r\n");
}

/****************************************************************************
* ��    ��: void BEEP_Init(void)
* ��    �ܣ�BEEP��ʼ��
* ��ڲ�������
* ���ز�������
* ˵    ������������ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void BEEP_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BEEP_PORT_TIMER, ENABLE);      //ʹ��BEEPʱ��
  
    GPIO_InitStructure.GPIO_Pin = BEEP_PORT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //����
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);            //��ʼ��GPIO
	
    GPIO_ResetBits(BEEP_PORT,BEEP_PORT_PIN);              //��������Ӧ����GPIOG7����,�رշ�����
    
    WOODS_DBG_PRINTF("END of BEEP_Init() !\r\n");
}

/****************************************************************************
* ��    ��: void BEEP_Init(void)
* ��    �ܣ�BEEP��ʼ��
* ��ڲ�������
* ���ز�������
* ˵    ������������ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void BEEP_Call(void)
{
    BEEP = ~BEEP;       
    WOODS_DBG_PRINTF("BEEP is ring !\r\n");
}



