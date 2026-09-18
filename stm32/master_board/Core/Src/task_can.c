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
#include "shared_data.h"
#include "can_lib.h"
#include <string.h>

extern osMessageQueueId_t QueueUartCanHandle;
extern osMessageQueueId_t QueueCanUartHandle;
volatile uint16_t gowno = 0;
//narazie z dupy buffer trzeba napisac hardware interface w ros control zeby zobaczyc jak bedziem pakowac dane 49 bajtow chyba
//
#define ARM_JOINT1_FRAME_ID 200
#define ARM_JOINT1_LENGTH 8
void task_can(void *arg)
{
	Can_init(&hcan1,0,0);

	UartFrame buff;
	UartFrame can_uart;
	Can_Msg_t can_data;
	uart_single_joint_t to_ros;
//	trzeba jakos dodac cantoolsy i sie wybierze id
//	#include "arm.h"

	volatile uint32_t can_tx_timeouts = 0;
	//test


	Can_Msg_t can_frames_joints[JOINT_COUNT] = {0};
	//zapchanie idikow jeszce sie uzgodni jak beda cantoolsy i inne jointy
	for (uint32_t i = 0; i < JOINT_COUNT; ++i)
	{
	    can_frames_joints[i].id = ARM_JOINT1_FRAME_ID + i;
	    can_frames_joints[i].dlc = ARM_JOINT1_LENGTH;
	}

	for (;;)
	{
	    if (Can_Read_Data(&can_data))
	    {
	        //pakowanie do uartsinglejoint i przesyl kolejka do uart
	    }

	    if (osMessageQueueGet(QueueUartCanHandle, &buff, NULL, 0) == osOK)
	    {

	        for (uint32_t i = 0; i < JOINT_COUNT; ++i)
	        {
	            memcpy(&can_frames_joints[i].data[0],
	                   &buff.data[2 + 4 * i],
	                   sizeof(float));

	            memcpy(&can_frames_joints[i].data[4],
	                   &buff.data[2 + 4 * JOINT_COUNT + 4 * i],
	                   sizeof(float));
	        }

	        bool all_queued = true;
	        uint32_t start_tick = osKernelGetTickCount();

	        for (uint32_t i = 0; i < JOINT_COUNT; )
	        {
	            Can_Msg_t *frame = &can_frames_joints[i];

	            if (Can_Send_Data(
	                    &hcan1, frame, frame->id, frame->data, frame->dlc))
	            {
	                ++i;
	                continue;
	            }

	            if ((uint32_t)(osKernelGetTickCount() - start_tick) >= 20U)
	            {
	                all_queued = false;
	                break;
	            }

	            osDelay(1);
	        }

	        if (!all_queued)
	        {
	            can_tx_timeouts++;
	        }
	    }
	    //dodac po wszystkich rozeslaniach komende start
	    //kurwa chujowo napisale ten kod xD

	    osDelay(2);
	}
}
