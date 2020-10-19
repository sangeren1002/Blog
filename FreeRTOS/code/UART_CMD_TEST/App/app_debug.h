/***********************************************************************************
 * �� �� ��   : app_debug.h
 * �� �� ��   : jishubao
 * ��������   : 2020��9��23��
 * �ļ�����   : app_debug.c ��ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef __APP_DEBUG_H__
#define __APP_DEBUG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "stm32f4xx_hal.h"
#include "sys_define.h"


#define ARG_NUM     8          //����������Ĳ�������
#define CMD_LEN     20         //������ռ�õ�����ַ�����
#define CMD_BUF_LEN 60         //��������󳤶�
#define ADMIN_PASSWORD "666666"
#define ADMIN_PASSWORD_LEN 6


typedef struct {
    char rec_buf[CMD_BUF_LEN];            //�����������
    char processed_buf[CMD_BUF_LEN];      //�洢�ӹ��������(ȥ�������ַ�)
    int32_t cmd_arg[ARG_NUM];             //��������Ĳ���
}cmd_analyze_struct;
typedef struct {
	char const *cmd_name;                        //�����ַ���
	int32_t max_args;                            //��������Ŀ
	void (*handle)(int argc,void * cmd_arg);     //����ص�����
	char  *help;                                 //������Ϣ
}cmd_list_struct;

extern cmd_analyze_struct cmd_analyze;
extern const cmd_list_struct cmd_list[];
extern s32 CmdArgNumAnalyze(char * rec_buf, u16 len);
extern void DebugCmdArgAnalyze(char *buff, uint16_t len);
extern void DebugCmdks103addrprintf(int32_t argc,void * cmd_arg);
extern void DebugCmdks103chaaddr(int32_t argc,void * cmd_arg);
extern void DebugCmdMov(int32_t argc,void * cmd_arg);
extern void DebugCmdReboot(int32_t argc,void * cmd_arg);
extern void DebugCmdshutdown(int32_t argc,void * cmd_arg);
extern void DebugCmdTaskinfo(int32_t argc,void * cmd_arg);
extern void DebugCmdUltmsg(int32_t argc,void * cmd_arg);
extern void DebugCmdUltprintf(int32_t argc,void * cmd_arg);
extern uint32_t get_true_char_stream(char *dest,const char *src);
extern void handle_arg(int32_t argc,void * cmd_arg);
extern void printf_hello(int32_t argc,void *cmd_arg);
extern void printf_help(int32_t argc,void * cmd_arg);
extern int32_t string_to_dec(uint8_t *buf,uint32_t len);
extern void DebugCmdsetfirmware(int32_t argc,void * cmd_arg);
extern void DebugCmdreadfirmware(int32_t argc,void * cmd_arg);
extern void DebugCmdAdmin(int32_t argc,void * cmd_arg);
extern void Show_SYS_INFO_Task(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __APP_DEBUG_H__ */
