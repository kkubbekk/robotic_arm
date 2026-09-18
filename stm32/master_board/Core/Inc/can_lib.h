/*
 * can_lib.h
 *
 *  Created on: 12 wrz 2026
 *      Author: styka
 */

#ifndef SRC_CAN_LIB_H_
#define SRC_CAN_LIB_H_

#include <stdint.h>
#include <stdbool.h>
#include "can.h"


#define RX_BUFFER_SIZE 64

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[8];
} Can_Msg_t;

void Can_init(CAN_HandleTypeDef* hcan,uint32_t node_id,uint32_t mask);

void Can_Set_Strict_Filter(CAN_HandleTypeDef* hcan, uint32_t my_motor_id);
//ringbuffer
bool Can_Read_Data(Can_Msg_t *msg_out);

typedef struct
{
	volatile uint16_t wr_ptr;
	volatile uint16_t rd_ptr;
	volatile Can_Msg_t buffer[RX_BUFFER_SIZE];
	volatile uint8_t is_full;
	volatile uint8_t dropped_frames;
} RingBuf;


void ring_buff_init(RingBuf *rb);
void ring_buff_write(RingBuf *rb, int value);
int avg_ring_buff(RingBuf *rb);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

bool Can_Send_Data(
    CAN_HandleTypeDef *hcan,
    Can_Msg_t *msg_send,
    uint32_t id,
    uint8_t *payload,
    uint8_t dlc);
#endif /* SRC_CAN_LIB_H_ */


