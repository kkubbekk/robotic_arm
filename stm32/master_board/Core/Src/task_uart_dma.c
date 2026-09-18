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
volatile HAL_StatusTypeDef dupa = HAL_ERROR;
volatile uint32_t tx_attempts = 0;
volatile uint32_t tx_ok_count = 0;

UartFrame frame_uart_can;

UartFrame frame_can_uart;

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


		if (HAL_UARTEx_ReceiveToIdle_DMA(&huart2,frame_uart_can.data,sizeof(frame_uart_can.data)) != HAL_OK)
		{

	    }

	    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	}
}




void task_uart(void *arg)
{

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, frame_uart_can.data, sizeof(frame_uart_can.data));

		//wylaczamy przerwanie w polowie
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);


	float test_vel = 1.023f;
	float test_pos = 2.34f;

	uart_single_joint_t test;

	test.ctrl[0] = 0x1C;
	test.ctrl[1] = 0xFF;
	test.joint_id=0;


	float test_vel1 = -1.023f;
		float test_pos1 = 5.34f;

		uart_single_joint_t test1;

		test1.ctrl[0] = 0x1C;
		test1.ctrl[1] = 0xFF;
		test1.joint_id=1;

		memcpy(&test1.data[0], &test_pos1, sizeof(test_pos1));
		memcpy(&test1.data[4], &test_vel1, sizeof(test_vel1));


	memcpy(&test.data[0], &test_pos, sizeof(test_pos));
	memcpy(&test.data[4], &test_vel, sizeof(test_vel));

	for(;;)
	{
		//jesli mamy cos z kolejki cana rozpakowujemy ramke i ja przesylamy tutaj trzeba myslec juz na cantoolsami
//		if(osMessageQueueGet(mq_id, msg_ptr, msg_prio, timeout) == HAL_OK)
//		{
//
//
//		}

		tx_attempts++;


		dupa = HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&test, sizeof(test));
//
		osDelay(20);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&test1, sizeof(test1));
		if(dupa == HAL_OK) tx_ok_count++;







		osDelay(100);
	}
}


