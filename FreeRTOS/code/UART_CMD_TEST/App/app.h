/***********************************************************************************
 * �� �� ��   : app.h
 * �� �� ��   : jishubao
 * ��������   : 2020��9��23��
 * �ļ�����   : app.c ��ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef __APP_H__
#define __APP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "sys_define.h"

#define LEDRUN_FRE  1   //ϵͳ����ָʾ��Ƶ�� Hz
#define LEDRUN_CYCLE    1000/(LEDRUN_FRE*2)
typedef struct{
	uint32_t UID[3];
	uint8_t chip_id[12];
	uint16_t flashsize;
	uint16_t package;
}CHIP_INFO;

extern osThreadId defaultTaskHandle;
extern osThreadId DebugCmdTaskHandle;
extern osThreadId LedRunTaskHandle;
extern osThreadId Upload2PCTaskHandle;
extern osThreadId SysInitTaskHandle;
extern osMessageQId U1RecvQueueHandle;
extern osMessageQId U2RecvQueueHandle;
extern osMessageQId U2SendQueueHandle;
extern osMessageQId U1SendQueueHandle;
extern osMutexId PrintfMutexHandle;

extern void ProcessSysInit(void);
extern void ProcessDebugCmdTask(void);
extern void ProcessLedRun(void);

extern void StartSysInitTask(void const * argument);
extern void StartDebugCmdTask(void const * argument);
extern void StartLedRunTask(void const * argument);
extern void StartUpload2PCTask(void const * argument);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __APP_H__ */
