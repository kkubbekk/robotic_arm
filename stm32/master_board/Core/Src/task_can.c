/*
 * task_can.c
 *
 *  Created on: Sep 14, 2026
 *      Author: kubekpc
 */

//
// 	 this task takes data from uart queue and packs it in order to send can frames, slave boards will
//	 procces data and calculate proper stepper motor velocity. After all joint can frames will have been delievered task gonna transmit
//	 start commend to synchronize movement

#include "cmsis_os.h"
#include "task_can.h"

extern osMessageQueueId_t QueueUartCanHandle;
//narazie z dupy buffer trzeba napisac hardware interface w ros control zeby zobaczyc jak bedziem pakowac dane 49 bajtow chyba
//
typedef struct
{
	float position[6];
	float velocity[6];
} data_t;

void task_can(void *arg)
{
	data_t buff;

	for(;;)
	{
		if (osMessageQueueGet(QueueUartCanHandle,&buff, NULL, osWaitForever) == osOK)
		{

		}
		osDelay(10);
	}
}
