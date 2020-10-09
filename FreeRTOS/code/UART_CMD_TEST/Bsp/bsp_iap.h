/***********************************************************************************
 * �� �� ��   : bsp_iap.h
 * �� �� ��   : jishubao
 * ��������   : 2020��2��27��
 * �ļ�����   : bsp_iap.c ��ͷ�ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/

#ifndef __BSP_IAP_H__
#define __BSP_IAP_H__
#include "stm32f4xx_hal.h"
#include "string.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#define DEVICE_INFO_ADDR_START 0X080E0000

#define DEVICE_FLAG 1
#define DEVICE_SERIALNUMBER 200924002
#define DEVICE_DATA 20200924
#define DEVICE_AUTHOR "jishubao"
#define DEVICE_EMAIL "shubaoji@gmail.com"
#define FIRMWARE_VER "V1.0.1"
#define DEVICE_AUTHOR_MAXLEN 20
#define DEVICE_EMAIL_MAXLEN 24
#define FIRMWARE_VER_MAXLEN 12



typedef struct{
    uint32_t flag_update;               //�Ƿ���д�豸��Ϣ��־ 1:����д 
    uint32_t serialNumber;              //�豸���к�
    uint32_t DateProduction;            //�豸�������� 0x20200227 ��ʾ������
    char author[20];     				//����
	char email[24];     				//�����ʼ�
	char firmware_ver[12];     			//�̼��汾
}DEVICE_INFO;


extern DEVICE_INFO device_info;
void Read_Device_info(void);
int8_t SetFirmwareInfo(uint32_t serialNumber, uint32_t DateProduction, char *author, char *email);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __BSP_IAP_H__ */
