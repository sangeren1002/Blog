/***********************************************************************************
 * �� �� ��   : bsp_usart.h
 * �� �� ��   : jishubao
 * ��������   : 2020��9��23��
 * �ļ�����   : bsp_usart.c ��ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef __BSP_USART_H__
#define __BSP_USART_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "sys_define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


#define RX_LEN 100  
#define TX_LEN 100	 
typedef struct  
{  
    u8  RX_flag:1;        //IDLE receive flag
    u8  TX_flag:1;        //IDLE send flag
    u8  RX_Size;          //receive length
    u8  RX_pData[RX_LEN]; //DMA receive buffer
}USART_RECEIVETYPE; 
typedef struct  
{  
    u16  TX_Size;          //receive length
    u8  TX_pData[RX_LEN]; //DMA receive buffer
}USART_SENDTYPE;


extern osThreadId Upload2PCTaskHandle;
extern osMessageQId U1RecvQueueHandle;
extern osMessageQId U2RecvQueueHandle;
extern osMessageQId U2SendQueueHandle;
extern osMessageQId U1SendQueueHandle;
extern osMutexId PrintfMutexHandle;

extern void UsartInit(void);
extern void AppPrintf(char *format, ...);
extern  u8 USART2_Send_DMA(u8 *data,u8 len);
extern void UsartReceive_IDLE(UART_HandleTypeDef *huart);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __BSP_USART_H__ */
