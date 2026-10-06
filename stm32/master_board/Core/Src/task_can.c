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
#include <stdbool.h>
#include "arm.h"

#define JOINT_COUNT 6

extern osMessageQueueId_t QueueUartCanHandle;
extern osMessageQueueId_t QueueCanUartHandle;
volatile uint16_t gowno = 0;
//narazie z dupy buffer trzeba napisac hardware interface w ros control zeby zobaczyc jak bedziem pakowac dane 49 bajtow chyba
//

void task_can(void *arg)
{
	Can_init(&hcan1,0,0);

	UartFrame buff;

	Can_Msg_t can_data;

	extern uint8_t control_data[2];

	uart_single_joint_t to_ros;

	memcpy(&to_ros.ctrl[0],&control_data[0],sizeof(control_data));
	//ctrl bita

//	trzeba jakos dodac cantoolsy i sie wybierze id
//	#include "arm.h"

	volatile uint32_t can_tx_timeouts = 0;

	//setup komendy start
	uint8_t start_payload[1] = {0};
	struct arm_start_command_t start_msg = { .start_sig = 1 };
	arm_start_command_pack(start_payload, &start_msg, 1);



	for (;;)
	{
		//---------------------czytanie z cana ramek i parsowanie je na ros----------------
	    if (Can_Read_Data(&can_data))
	    {
	    	float received_vel = 0.0f;
	    	float received_pos = 0.0f;
	    	bool is_valid_frame = true;
						switch(can_data.id)
			            {
			                case ARM_JOINT0_FRAME_ID: {
			                    struct arm_joint0_t joint0;
			                    arm_joint0_unpack(&joint0, can_data.data, can_data.dlc);
			                    received_vel = (float)joint0.joint0_vel;
			                    received_pos = (float)joint0.joint0_pos;
			                    break;
			                }
			                case ARM_JOINT1_FRAME_ID: {
			                    struct arm_joint1_t joint1;
			                    arm_joint1_unpack(&joint1, can_data.data, can_data.dlc);
			                    received_vel = (float)joint1.joint1_vel;
			                    received_pos = (float)joint1.joint1_pos;
			                    break;
			                }
			                case ARM_JOINT2_FRAME_ID: {
			                    struct arm_joint2_t joint2;
			                    arm_joint2_unpack(&joint2, can_data.data, can_data.dlc);
			                    received_vel = (float)joint2.joint2_vel;
			                    received_pos = (float)joint2.joint2_pos;
			                    break;
			                }
			                case ARM_JOINT3_FRAME_ID: {
			                    struct arm_joint3_t joint3;
			                    arm_joint3_unpack(&joint3, can_data.data, can_data.dlc);
			                    received_vel = (float)joint3.joint3_vel;
			                    received_pos = (float)joint3.joint3_pos;
			                    break;
			                }
			                case ARM_JOINT4_FRAME_ID: {
			                    struct arm_joint4_t joint4;
			                    arm_joint4_unpack(&joint4, can_data.data, can_data.dlc);
			                    received_vel = (float)joint4.joint4_vel;
			                    received_pos = (float)joint4.joint4_pos;
			                    break;
			                }
			                case ARM_JOINT5_FRAME_ID: {
			                    struct arm_joint5_t joint5;
			                    arm_joint5_unpack(&joint5, can_data.data, can_data.dlc);
			                    received_vel = (float)joint5.joint5_vel;
			                    received_pos = (float)joint5.joint5_pos;
			                    break;
			                }
			                default:
			                	is_valid_frame = false;
			                	break;
			            	}

			               if(is_valid_frame)
			               {
			            	   memcpy(&to_ros.data[0],&received_pos,sizeof(received_pos));
			            	   memcpy(&to_ros.data[4],&received_vel,sizeof(received_vel));
			            	   to_ros.joint_id = can_data.id;

							   if(osMessageQueuePut(QueueCanUartHandle,&to_ros,0,0) == osOK)
			            		  {
			            	   	   	   //sigma boi
			            		  }
			               }

			               osDelay(1);



	    			}

//		--------------------odbieranie danych z kolejeki i pakowanie je na jointy do wyslania po can ros->uuart-> can----------------



	    					if(osMessageQueueGet(QueueUartCanHandle,&buff,0,0) == osOK)
	 			               	{
	    							uint32_t joint_send_cldwn = osKernelGetTickCount();
	    							bool joint_sent = true;

	 			            	   for(int i = 0; i < JOINT_COUNT;)
	 			            	   {
	 			            			uint8_t can_payload[8] = {0};
	 			            			uint32_t current_id;
										uint8_t current_dlc;
	 			            			float tx_vel,tx_pos;

	 			            		   memcpy(&tx_vel,&buff.data[2+4*i],sizeof(tx_vel));
	 			            		   memcpy(&tx_pos,&buff.data[2+4*i+4*JOINT_COUNT],sizeof(tx_pos));


	 			            		   switch(i)
	 			            		   {
											case 0: {
												struct arm_joint0_t joint0;
												joint0.joint0_pos = tx_pos;
												joint0.joint0_vel = tx_vel;
												current_id = ARM_JOINT0_FRAME_ID;
												current_dlc = ARM_JOINT0_LENGTH;
												arm_joint0_pack(can_payload, &joint0,  ARM_JOINT0_LENGTH);
												break;
											}
	 			            		   	   case 1: {
	 			            		   		   struct arm_joint1_t joint1;
	 			            		   		   joint1.joint1_pos = tx_pos;
	 			            		   		   joint1.joint1_vel = tx_vel;
	 			            		   		   current_id = ARM_JOINT1_FRAME_ID;
	 			            		   		   current_dlc = ARM_JOINT1_LENGTH;
	 			            		   		   arm_joint1_pack(can_payload, &joint1,  ARM_JOINT1_LENGTH);
	 			            		   		   break;
											}
											case 2: {
												struct arm_joint2_t joint2;
												joint2.joint2_pos = tx_pos;
												joint2.joint2_vel = tx_vel;
												current_id = ARM_JOINT2_FRAME_ID;
												current_dlc = ARM_JOINT2_LENGTH;
												arm_joint2_pack(can_payload, &joint2,  ARM_JOINT2_LENGTH);
												break;
											}
											case 3: {
												struct arm_joint3_t joint3;
												joint3.joint3_pos = tx_pos;
												joint3.joint3_vel = tx_vel;
												current_id = ARM_JOINT3_FRAME_ID;
												current_dlc = ARM_JOINT3_LENGTH;
												arm_joint3_pack(can_payload, &joint3,  ARM_JOINT3_LENGTH);
												break;
											}
											case 4: {
												struct arm_joint4_t joint4;
												joint4.joint4_pos = tx_pos;
												joint4.joint4_vel = tx_vel;
												current_id = ARM_JOINT4_FRAME_ID;
												current_dlc = ARM_JOINT4_LENGTH;
												arm_joint4_pack(can_payload, &joint4,  ARM_JOINT4_LENGTH);
												break;
											}
											case 5: {
												struct arm_joint5_t joint5;
												joint5.joint5_pos = tx_pos;
												joint5.joint5_vel = tx_vel;
												current_id = ARM_JOINT5_FRAME_ID;
												current_dlc = ARM_JOINT5_LENGTH;
												arm_joint5_pack(can_payload, &joint5,  ARM_JOINT5_LENGTH);
												break;
											}
	 			            		   		//i tu reszta caseow zara prompt engineering wlecii
	 			            		   }
	 			            		   //po znalezieniu i zlozeniu pieknej ramki mozna ja wypchnac w swiat   //po wszystkim
	 			            		   if(Can_Send_Data(&hcan1,current_id, can_payload,current_dlc) )
	 			            		   {
	 			            			   //jezeli sie uda wepchnac ramke w swiat to zwiekszmay licznik
	 			            			   i++;
	 			            			   continue;
	 			            		   }
	 			            		   if((uint32_t)(osKernelGetTickCount()-joint_send_cldwn) > 20U)
	 			            		   {
	 			            			   joint_sent = false;
										   can_tx_timeouts++;
										   break;
	 			            		   }
	 			            		   //TODO: warto moze dodac timer jezeli sie nie wysle przez iles czasu


	 			            	   }

								   //po wyslaniu wszystkich ramek z ruchem wysylamy komende start
	 			            		  if(joint_sent)
	 			            		  {
	 			            			 uint32_t start_cmd_tick = osKernelGetTickCount();

	 			            			 while(!Can_Send_Data(&hcan1,ARM_START_COMMAND_FRAME_ID, start_payload,1))
	 			            			 {
	 			            				 if ((uint32_t)(osKernelGetTickCount() - start_cmd_tick) >= 5U)  break;
	 			            				osDelay(1);
	 			            			 }
	 			            		  }
	 			            	}





	}

//










}
