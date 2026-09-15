/*
 * task_uart_dma.c
 *
 *  Created on: Sep 14, 2026
 *      Author: kubekpc
 */
#include "task_uart_dma.h"
#include <stdint.h>
#include "usart.h"
#include <string.h>
#include "cmsis_os.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
extern osMessageQueueId_t QueueUartCanHandle;

uint8_t RxBuf[RxBuf_SIZE];





void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2 && Size == 25)
	{
		osMessageQueuePut(QueueUartCanHandle, RxBuf, 0, 0);
	}
}


void task_uart(void *arg)
{

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxBuf,RxBuf_SIZE);

		//wylaczamy przerwanie w polowie
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	for(;;)
	{
		//jesli mamy cos z kolejki cana rozpakowujemy ramke i ja przesylamy tutaj trzeba myslec juz na cantoolsami
//		if(osMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout) == HAL_OK)
//		{
//
//		}





		static uint8_t message[] = "UART DMA DZIALA\r\n";


		  HAL_UART_Transmit_DMA(&huart2,message,sizeof(message) - 1);
		  osDelay(100);
	}
}
