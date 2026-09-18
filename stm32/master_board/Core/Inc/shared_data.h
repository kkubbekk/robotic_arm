/*
 * shared_data.h
 *
 *  Created on: Sep 17, 2026
 *      Author: kubekpc
 */

#ifndef SRC_SHARED_DATA_H_
#define SRC_SHARED_DATA_H_

#define JOINT_COUNT 1
#define UART_FRAME_SIZE (2 + 8 * JOINT_COUNT)



typedef union
{
    uint8_t data[UART_FRAME_SIZE];

    struct
    {
        uint8_t ctrl_byte[2];
        uint8_t vel_data[4 * JOINT_COUNT];
        uint8_t pos_data[4 * JOINT_COUNT];
    } fields;

} UartFrame;


typedef struct{
	uint8_t ctrl[2];
	uint8_t joint_id;
	uint8_t data[8];
} uart_single_joint_t;


#endif /* SRC_SHARED_DATA_H_ */
