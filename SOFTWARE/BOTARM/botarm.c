#include "botarm.h"
#include "protocol.h"
#include "can.h"
#include "delay.h"
#include "usart.h"	

enum BOTARM_NAME
{
    RUArm =0, RDArm = 3, LUArm, LDArm = 4,
}botarm_name;

/****************************************************************************
* ��    ��: void BOTARM_Init(void)
* ��    �ܣ���ʼ����е�ۡ�ҡ�۵��
* ��ڲ�������
* ���ز�������
* ˵    �����Ե�����г�ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void BOTARM_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(BOTARM_PORT_TIMER, ENABLE);      //ʹ��PORTʱ��
    GPIO_InitStructure.GPIO_Pin = BOTARM_PORT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //����
    GPIO_Init(BOTARM_PORT, &GPIO_InitStructure);           //��ʼ��GPIO
       
    BOTARM_POWER_EN = 1;
    delay_ms(1500);
    SetBotArm_EN(RDArm);
    SetBotArm_EN(LDArm);
    delay_ms(500);
}

/****************************************************************************
* ��    ��: void SetBotArm_Speed(void)
* ��    �ܣ����û�е�ۡ�ҡ�۵��ת�ٻ�λ��
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void SetBotArm_Speed(void)
{
    uint8_t ret_RDArm = 1, ret_LDArm = 1;
    uint8_t temp_BotArm_v = 0;
    BotArmCtrl.RDArm_Ctrl = 0xE0;
    BotArmCtrl.LDArm_Ctrl = 0xE0;
    if((BotArmCtrl.RDArm_Ctrl & 0x80) == 0x80)
    {
        temp_BotArm_v = BotArmCtrl.RDArm_Ctrl & 0x3F;
        if(temp_BotArm_v != 0)
        {
            if((BotArmCtrl.RDArm_Ctrl & 0x40) == 0x40)
            {   
                ret_RDArm = SetBotArm_speed(RDArm,(s32)-2000); 
            }
            else
            {
                ret_RDArm = SetBotArm_speed(RDArm,(s32)2000); 
            }        
        }
        else
        {
            ret_RDArm = SetBotArm_speed(RDArm,0);            
        }
    }
    else
    {
        ret_RDArm = SetBotArm_speed(RDArm,-0);  
    }
    
    if((BotArmCtrl.LDArm_Ctrl & 0x80) == 0x80)
    {
        temp_BotArm_v = BotArmCtrl.LDArm_Ctrl & 0x3F;
        if(temp_BotArm_v != 0)
        {
            if((BotArmCtrl.LDArm_Ctrl & 0x40) == 0x40)
            {
                ret_LDArm = SetBotArm_speed(LDArm,(s32)2000);
            }
            else
                ret_LDArm = SetBotArm_speed(LDArm,(s32)-2000); 
        }
        else 
            ret_LDArm = SetBotArm_speed(LDArm,0); 
    }
    else
        ret_LDArm = SetBotArm_speed(LDArm,0);
//    WOODS_DBG_PRINTF("ret RDArm=%d LDArm=%d !\r\n",ret_RDArm, ret_LDArm);
//    WOODS_DBG_PRINTF("wheel_speed A=%d B=%d !\r\n",ChassisDrvCtrl.AWH_v, ChassisDrvCtrl.BWH_v);
}

/****************************************************************************
* ��    ��: uint8_t SetBotArm_speed(uint8_t botarm_name, int32_t speed)
* ��    �ܣ����û�е�ۡ�ҡ�۵��ת��
* ��ڲ�������
* ���ز�������
* ˵    ��������账���ٶ�ģʽ
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetBotArm_speed(uint8_t botarm_name, int32_t speed)
{
    uint8_t ret;
    CAN_Txbuf[0] = 0xFF;
    CAN_Txbuf[1] = 0x00;
    CAN_Txbuf[2] = 0x03;
    CAN_Txbuf[3] = (uint8_t)speed;
    CAN_Txbuf[4] = (uint8_t)(speed >> 8);
    CAN_Txbuf[5] = (uint8_t)(speed >> 16);
    CAN_Txbuf[6] = (uint8_t)(speed >> 24);
    ret = CAN1_Send_Msg((0x200 + botarm_name),0,0,0,CAN_Txbuf,7);
    return ret;
}

/****************************************************************************
* ��    ��: uint8_t SetBotArm_position(uint8_t botarm_name,int32_t position)
* ��    �ܣ����û�е�ۡ�ҡ�۵��λ��
* ��ڲ�������
* ���ز�������
* ˵    ��������账��λ��ģʽ
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetBotArm_position(uint8_t botarm_name,int32_t position)
{
    uint8_t ret;
//    CAN_Txbuf[0] = 0xFF;                                           //PD0ģʽ
//    CAN_Txbuf[1] = 0x00;
//    CAN_Txbuf[2] = 0x01;
//    CAN_Txbuf[3] = (uint8_t)position;
//    CAN_Txbuf[4] = (uint8_t)(position >> 8);
//    CAN_Txbuf[5] = (uint8_t)(position >> 16);
//    CAN_Txbuf[6] = (uint8_t)(position >> 24);
//    ret = CAN1_Send_Msg((0x200 + botarm_name),0,0,0,CAN_Txbuf,7);

    CAN_Txbuf[0] = 0x23;                                           //SD0ģʽ
    CAN_Txbuf[1] = 0x7A;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = (uint8_t)position;
    CAN_Txbuf[5] = (uint8_t)(position >> 8);
    CAN_Txbuf[6] = (uint8_t)(position >> 16);
    CAN_Txbuf[7] = (uint8_t)(position >> 24);
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    delay_ms(10);   
    CAN_Txbuf[0] = 0x2B;                                           
    CAN_Txbuf[1] = 0x40;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x7F;
    CAN_Txbuf[5] = 0x00;
    CAN_Txbuf[6] = 0x00;
    CAN_Txbuf[7] = 0x00;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    return ret;
}

/****************************************************************************
* ��    ��: uint8_t SetBotArm_EN(uint8_t botarm_name)
* ��    �ܣ����û�е�ۡ�ҡ�۵��ʹ��
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetBotArm_EN(uint8_t botarm_name)
{
    uint8_t ret;
//    CAN_Txbuf[0] = 0x01;                              //PD0ģʽ
//    CAN_Txbuf[1] = botarm_name;
//    ret = CAN1_Send_Msg(0,0,0,0,CAN_Txbuf,2);
    
    CAN_Txbuf[0] = 0x2B;                               //SD0ģʽ
    CAN_Txbuf[1] = 0x40;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x06;
    CAN_Txbuf[5] = 0x00;
    CAN_Txbuf[6] = 0x00;
    CAN_Txbuf[7] = 0x00;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    delay_ms(30);
    CAN_Txbuf[4] = 0x0F;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);   

    delay_ms(30);    
    CAN_Txbuf[0] = 0x2F;                               //����ΪPPMģʽ
    CAN_Txbuf[1] = 0x60;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x01;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    return ret;
}








