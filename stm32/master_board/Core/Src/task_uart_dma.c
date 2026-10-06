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
#include "shared_data.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
extern osMessageQueueId_t QueueUartCanHandle;
extern osMessageQueueId_t QueueCanUartHandle;
extern osSemaphoreId_t	uartsem;


volatile HAL_StatusTypeDef dupa = HAL_ERROR;
volatile uint32_t tx_attempts = 0;
volatile uint32_t tx_ok_count = 0;

UartFrame frame_uart_can;



uart_single_joint_t buff;



uint8_t control_data[2] = {0x1c , 0xff};


volatile uint32_t uart_rx_events = 0;
volatile uint16_t uart_rx_size = 0;
volatile float debug_vel = 0.0f;
volatile float debug_pos = 0.0f;


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if(huart->Instance == USART2 && Size == sizeof(frame_uart_can.data) && frame_uart_can.data[0] == control_data[0] && frame_uart_can.data[1] == control_data[1])
	{


		if(osMessageQueuePut(QueueUartCanHandle, &frame_uart_can, 0, 0) != osOK)
		{
			//TODO:
		}






	}
	if (HAL_UARTEx_ReceiveToIdle_DMA(&huart2,frame_uart_can.data,sizeof(frame_uart_can.data)) != HAL_OK)
		{

	    }
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	osSemaphoreRelease(uartsem);
}



void task_uart(void *arg)
{

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, frame_uart_can.data, sizeof(frame_uart_can.data));

		//wylaczamy przerwanie w polowie
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);



	for(;;)
	{

		//jesli mamy cos z kolejki cana rozpakowujemy ramke i ja przesylamy tutaj trzeba myslec juz na cantoolsami
		if(osMessageQueueGet(QueueCanUartHandle,&buff, NULL,100) == osOK)
		{

			if(osSemaphoreAcquire(uartsem,10)== osOK)
			{
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buff, sizeof(uart_single_joint_t));
			}
		} else
		{
			//cosik jest kurwa nie tak mozna odsylac do rosa jakis stan fatal czy cosik
		}












		osDelay(2);
	}
}


