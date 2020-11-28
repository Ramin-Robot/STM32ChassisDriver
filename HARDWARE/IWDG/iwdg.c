#include "iwdg.h"

/****************************************************************************
* ��    ��: void IWDG_Init(u8 prer,u16 rlr)
* ��    �ܣ���ʼ���������Ź�
* ��ڲ�����prer:��Ƶ��:0~7 rlr:�Զ���װ��ֵ,0~0XFFF
* ���ز�������
* ˵    ������Ƶ����=4*2^prer.�����ֵֻ����256!
            ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	IWDG_SetPrescaler(prer);                      //����IWDG��Ƶ��:0~7
	IWDG_SetReload(rlr);                          //����IWDG�Զ���װ��ֵ,0~0XFFF
	IWDG_ReloadCounter();                         //reload
	IWDG_Enable();                                //ʹ�ܿ��Ź�
}

/****************************************************************************
* ��    ��: void IWDG_Feed(void)
* ��    �ܣ�ι��
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/	
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
