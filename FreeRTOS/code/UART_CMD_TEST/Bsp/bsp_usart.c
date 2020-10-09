#include "bsp_usart.h"
#include "string.h"
#include "stdarg.h"


USART_RECEIVETYPE Usart1TypeRecv_pInit;
USART_RECEIVETYPE Usart2TypeRecv_pInit;
USART_RECEIVETYPE Usart3TypeRecv_pInit;
USART_RECEIVETYPE Usart4TypeRecv_pInit;
USART_RECEIVETYPE Usart5TypeRecv_pInit;
USART_RECEIVETYPE Usart6TypeRecv_pInit;
USART_RECEIVETYPE *Usart1TypeRecv_p = &Usart1TypeRecv_pInit;
USART_RECEIVETYPE *Usart2TypeRecv_p = &Usart2TypeRecv_pInit;
USART_RECEIVETYPE *Usart3TypeRecv_p = &Usart3TypeRecv_pInit;
USART_RECEIVETYPE *Usart4TypeRecv_p = &Usart4TypeRecv_pInit;
USART_RECEIVETYPE *Usart5TypeRecv_p = &Usart5TypeRecv_pInit;
USART_RECEIVETYPE *Usart6TypeRecv_p = &Usart6TypeRecv_pInit;

void UsartInit(void)
{
    /* ������ڿ����жϱ�־ */
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);

    /* ���÷�����ģʽ�´��� DMA���� */
    HAL_UART_Receive_DMA(&huart1, Usart1TypeRecv_p->RX_pData, RX_LEN);
    HAL_UART_Receive_DMA(&huart2, Usart2TypeRecv_p->RX_pData, RX_LEN); 

    /* ʹ�ܴ��ڿ����ж� */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

}
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
    u32 temp;  
    BaseType_t pxHigherPriorityTaskWoken;

    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = (*huart).hdmarx->Instance->NDTR;
        if(huart->Instance==USART1)
        {
//            UsartType.RX_Size = RX_LEN - temp;   
//            UsartType.RX_flag = 1;  
//            HAL_UART_Receive_DMA(huart,UsartType.RX_pData,RX_LEN);  
			Usart1TypeRecv_p->RX_Size = RX_LEN - temp;
			Usart1TypeRecv_p->RX_flag = 1;  
			xQueueSendFromISR(U1RecvQueueHandle, (u32*)&Usart1TypeRecv_p, &pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
			HAL_UART_Receive_DMA(huart,Usart1TypeRecv_p->RX_pData,RX_LEN);
        }
        else if(huart->Instance==USART2)
        {
//            Usart2Type.RX_Size = RX_LEN - temp;   
//            Usart2Type.RX_flag = 1;  
//            HAL_UART_Receive_DMA(huart,Usart2Type.RX_pData,RX_LEN);  
			Usart2TypeRecv_p->RX_Size = RX_LEN - temp;
			Usart2TypeRecv_p->RX_flag = 1;  
			
            xQueueSendFromISR(U2RecvQueueHandle, (u32*)&Usart2TypeRecv_p, &pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
			HAL_UART_Receive_DMA(huart,Usart2TypeRecv_p->RX_pData,RX_LEN);
        }                  
    } 
 
}
 u8 USART2_Send_DMA(u8 *data,u8 len)
{
//	 u32 cnt = 0;;
	 /* modify begin by jishubao, 2019-02-27, Mantis��:1 
	ԭ��: ����ǰ��Ҫ�жϴ���״̬������״̬�ڴ��ڷ�����ɻص����������*/
//	while(HAL_UART_STATE_READY != huart2.gState && cnt< HAL_MAX_DELAY-1)cnt++; 
//	taskENTER_CRITICAL();
	


	if(HAL_UART_STATE_READY == huart2.gState)
	{
		if (HAL_UART_Transmit(&huart2, data, len, 0xffff) != HAL_OK)
		{
			/* Transfer error in transmission process */                                                                                  
			//Error_Handler();   
//			taskEXIT_CRITICAL();
			return 1;
		}
//		
	}
	else
	{
//		taskEXIT_CRITICAL();
		return 1;
	}
//	taskEXIT_CRITICAL();

	return 0;
}

/*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AppPrintf(char *format, ...)
{
	char buf[350];

	va_list ap;

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	
	/* ������� */
	osMutexWait(PrintfMutexHandle,osWaitForever);
	printf("%s", buf);
	osMutexRelease(PrintfMutexHandle);
}
//ʵ��printf�����ض��򵽴���1����֧��printf��Ϣ��USART1
 #ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	//myMutex01Handle
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
 
  return ch;
}
