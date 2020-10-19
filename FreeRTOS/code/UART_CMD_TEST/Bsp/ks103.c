/***********************************************************************************
 * �� �� ��   : ks103.c
 * �� �� ��   : jishubao
 * ��������   : 2020��1��3��
 * �ļ�����   : ��������KS103 iic������ģ�������ļ�
 * ��Ȩ˵��   : Copyright (c) 2008-2020   ���ݹ��������˿Ƽ����޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
/*
 *KS101B/KS103/KS103S Ĭ�ϵ�ַΪ 0xe8
 *��ģ�齫 I2C �ӻ���ַ�� 8 λ���� 7 λΪ��ַ���� 1 λΪ��дλ��
 *���Ʊ�ģ��������豸����ֻ֧�� 7 λ�� I2C �ӻ���ַ����ʱ��Ҫ�� 8 λ��ַ���� 1 λ��Ϊ��ַ��
 *ʹ�á����磬��ģ��Ĭ�ϵ�ַ 0xe8����Ӧ 7 λ�ĵ�ַ 0x74
 *
 */
#include "ks103.h"
//#include "bsp_iic.h"
#include "myiic.h"
#include "bsp_usart.h"
#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

Ultrasonic_STATUS_T ultrasonic_sensor[ULTRASONIC_NUM];
HW_STATUS_T hw_sensor[HW_NUM];

uint8_t KS103_Init(uint8_t *p_year, uint8_t *p_week)
{
    uint8_t i;
    uint8_t temp_addr=0;
	IIC_Init();
#if 0 //��ΪӲ��IIC2
    *p_year = I2C2_DEV_Read(KS103_ADDR_7BIT, 0x00);
	*p_week = I2C2_DEV_Read(KS103_ADDR_7BIT, 0x01);
#else  //��Ϊ���ģ��IIC2
//    Change_i2c_addr_KS103(0xe8, 0xea);
     for(i=0;i<ULTRASONIC_NUM;i++)
    {
            temp_addr = KS103_ADDR_7BIT+i*2;
        	*p_year = KS103IIC_ReadOneByte(temp_addr, 0x00);
            *p_week = KS103IIC_ReadOneByte(temp_addr, 0x01);
            if(*p_year == KS103_DEVICE_YEAR_DEFAULT && *p_week == KS103_DEVICE_WEEK_DEFAULT)
                ultrasonic_sensor[i].init_status = 1;
            else
                 ultrasonic_sensor[i].init_status = 0;
			ultrasonic_sensor[i].productionData.year = *p_year;
			ultrasonic_sensor[i].productionData.week = *p_week;
			ultrasonic_sensor[i].addr_7bit = temp_addr;
			if(ultrasonic_sensor[i].init_status == 1)
				AppPrintf("[ OK ] KS103 manufactured on addr %#02x %d year of %d week is sucess\r\n",temp_addr,*p_year,*p_week);
			else
				AppPrintf("[ Warning ] KS103 manufactured on addr %#02x %d year of %d week is failed\r\n",temp_addr,*p_year,*p_week);
    }
#endif
     return 0;
}

uint8_t KS103_Read_Distance(uint8_t dev_addr,uint16_t *distance)
{
	uint16_t range = 0;
#if 1 //��Ϊ���ģ��IIC2
	KS103IIC_WriteOneByte((KS103_ADDR_7BIT+dev_addr*2), 0x02, 0xb0);
	osDelay(40);
	range = KS103IIC_ReadOneByte((KS103_ADDR_7BIT+dev_addr*2), 0x02);
    osDelay(2);
	range<<=8;
	range += KS103IIC_ReadOneByte((KS103_ADDR_7BIT+dev_addr*2), 0x03);
    *distance = range; 
#else  //��ΪӲ��IIC2
	I2C2_DEV_Write(KS103_ADDR_7BIT, 0X02, 0XB0);
	osDelay(40);
	range = I2C2_DEV_Read(KS103_ADDR_7BIT, 0x02);
	range<<=8;
	osDelay(1);
	range += I2C2_DEV_Read(KS103_ADDR_7BIT, 0x03);

//    while (!Read_KS103_SCL())
//    {
//        HAL_Delay(1);
//        cnt++;
//        if(cnt>100)
//            return 1;
//    }
//    *distance = I2C2_DEV_Read(KS103_ADDR_7BIT, 0x02);
//    *distance = (*distance<<8) | I2C2_DEV_Read(KS103_ADDR_7BIT, 0x03);
#endif
//	printf("dev_addr %d range %d mm\r\n",dev_addr,range);


    return 0;
}
void Change_i2c_addr_KS103(uint8_t old_addr, uint8_t new_addr)
{
	HAL_Delay(200);
	KS103IIC_WriteOneByte(old_addr,2,0x9a);
	HAL_Delay(10);
	KS103IIC_WriteOneByte(old_addr,2,0x92);
	HAL_Delay(10);
	KS103IIC_WriteOneByte(old_addr,2,0x9e);
	HAL_Delay(10);
	KS103IIC_WriteOneByte(old_addr,2,new_addr);
}
uint8_t KS103IIC_WriteOneByte(uint8_t dev_write_addr, uint8_t reg,uint8_t data) 				 
{ 
    taskENTER_CRITICAL();
    IIC_Start(); 
	IIC_Send_Byte(dev_write_addr);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		
        taskEXIT_CRITICAL();        
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
        taskEXIT_CRITICAL();
		return 1;		 
	}		 
    IIC_Stop();	 
    taskEXIT_CRITICAL();
	return 0;
}
uint8_t KS103IIC_ReadOneByte(uint8_t dev_write_addr, uint16_t RegAddr)
{				  
	uint8_t temp=0;		
	taskENTER_CRITICAL();    
    IIC_Start();  
	IIC_Send_Byte(dev_write_addr);   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(RegAddr);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(dev_write_addr+1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
	delay_us(50);
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	
	taskEXIT_CRITICAL();    
	return temp;
}
uint8_t KS103_AddressLegalJudgment(uint8_t addr)
{
	uint8_t cnt = 0;
	for(uint8_t i=0;i<KS103_START_ADDR_SIZE;i++)
		if(addr == KS103_START_ADDR_BASE+i*KS103_START_ADDR_SPACING)
			cnt++;
	return cnt;
}
void PrintfKS103Msg(uint8_t id)
{
	uint8_t i = id -1;
	if(i<ULTRASONIC_NUM&&i>=0)
	{
		if(ultrasonic_sensor[i].init_status == 1)
			AppPrintf("[ OK ] KS103 manufactured on addr %#02x %d year of %d week is sucess\r\n",ultrasonic_sensor[i].addr_7bit,ultrasonic_sensor[i].productionData.year,ultrasonic_sensor[i].productionData.week);
		else
			AppPrintf("[ Warning ] KS103 manufactured on addr %#02x %d year of %d week is failed\r\n",ultrasonic_sensor[i].addr_7bit,ultrasonic_sensor[i].productionData.year,ultrasonic_sensor[i].productionData.week);
	}
	else
	{
		for(i=0;i<ULTRASONIC_NUM;i++)
		{
			if(ultrasonic_sensor[i].init_status == 1)
				AppPrintf("[ OK ] KS103 manufactured on addr %#02x %d year of %d week is sucess\r\n",ultrasonic_sensor[i].addr_7bit,ultrasonic_sensor[i].productionData.year,ultrasonic_sensor[i].productionData.week);
			else
				AppPrintf("[ Warning ] KS103 manufactured on addr %#02x %d year of %d week is failed\r\n",ultrasonic_sensor[i].addr_7bit,ultrasonic_sensor[i].productionData.year,ultrasonic_sensor[i].productionData.week);
		}
	
	}
}
