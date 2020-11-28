#include "motor.h"
#include "protocol.h"
#include "can.h"
#include "delay.h"
#include "usart.h"	

enum WHEEL_NAME
{
      WH_A=1, WH_B,
}wheel_name;

/****************************************************************************
* ��    ��: void MOTOR_Init(void)
* ��    �ܣ���ʼ�����̵��
* ��ڲ�������
* ���ز�������
* ˵    �����Ե�����г�ʼ��
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void MOTOR_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(MOTOR_PORT_TIMER, ENABLE);      //ʹ��PORTʱ��
    GPIO_InitStructure.GPIO_Pin = MOTOR_PORT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //����
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);                 //��ʼ��GPIO
       
    MOTOR_POWER_EN = 1;
    delay_ms(1500);
    SetMotor_EN(WH_A);
    SetMotor_EN(WH_B);
    delay_ms(500);
}

/****************************************************************************
* ��    ��: void SetMotor_Speed(void)
* ��    �ܣ����õ��̵��ת�ٻ�λ��
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
void SetMotor_Speed(void)
{
    uint8_t ret_A = 1, ret_B = 1;
    ret_A = SetMotor_speed(WH_A,(s32)ChassisDrvCtrl.AWH_v);
    ret_B = SetMotor_speed(WH_B,(s32)-ChassisDrvCtrl.BWH_v);
//    WOODS_DBG_PRINTF("ret A=%d B=%d !\r\n",ret_A, ret_B);
//    WOODS_DBG_PRINTF("wheel_speed A=%d B=%d !\r\n",ChassisDrvCtrl.AWH_v, ChassisDrvCtrl.BWH_v);
}

/****************************************************************************
* ��    ��: uint8_t SetMotor_speed(uint8_t wheel_name, int32_t speed)
* ��    �ܣ����õ��̵��ת��
* ��ڲ�������
* ���ز�������
* ˵    ��������账���ٶ�ģʽ
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetMotor_speed(uint8_t wheel_name, int32_t speed)
{
    uint8_t ret;
    CAN_Txbuf[0] = 0xFF;
    CAN_Txbuf[1] = 0x00;
    CAN_Txbuf[2] = 0x03;
    CAN_Txbuf[3] = (uint8_t)speed;
    CAN_Txbuf[4] = (uint8_t)(speed >> 8);
    CAN_Txbuf[5] = (uint8_t)(speed >> 16);
    CAN_Txbuf[6] = (uint8_t)(speed >> 24);
    ret = CAN1_Send_Msg((0x200 + wheel_name),0,0,0,CAN_Txbuf,7);
    return ret;
}

/****************************************************************************
* ��    ��: uint8_t SetMotor_position(uint8_t wheel_name, int16_t position)
* ��    �ܣ����õ��̵��λ��
* ��ڲ�������
* ���ز�������
* ˵    ��������账��λ��ģʽ
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetMotor_position(uint8_t wheel_name, int32_t position)
{
    uint8_t ret;
    CAN_Txbuf[0] = 0xFF;
    CAN_Txbuf[1] = 0x00;
    CAN_Txbuf[2] = 0x01;
    CAN_Txbuf[3] = (uint8_t)position;
    CAN_Txbuf[4] = (uint8_t)(position >> 8);
    CAN_Txbuf[5] = (uint8_t)(position >> 16);
    CAN_Txbuf[6] = (uint8_t)(position >> 24);
    ret = CAN1_Send_Msg((0x200 + wheel_name),0,0,0,CAN_Txbuf,7);
    return ret;
}

/****************************************************************************
* ��    ��: uint8_t SetMotor_EN(uint8_t wheel_name)
* ��    �ܣ����õ��̵��ʹ��
* ��ڲ�������
* ���ز�������
* ˵    ����
* ��    �ߣ�Ƕ��ʽС��Woods
****************************************************************************/
uint8_t SetMotor_EN(uint8_t wheel_name)
{
    uint8_t ret;
    CAN_Txbuf[0] = 0x01;
    CAN_Txbuf[1] = wheel_name;
    ret = CAN1_Send_Msg(0,0,0,0,CAN_Txbuf,2);
    return ret;
}


