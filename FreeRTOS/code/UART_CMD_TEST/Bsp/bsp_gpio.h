/***********************************************************************************
 * �� �� ��   : bsp_gpio.h
 * �� �� ��   : jishubao
 * ��������   : 2020��9��23��
 * �ļ�����   : bsp_gpio.c ��ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "sys_define.h"

#define LedRunToggle()  HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin)
#define LedAlarmToggle()  HAL_GPIO_TogglePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin)





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __BSP_GPIO_H__ */
