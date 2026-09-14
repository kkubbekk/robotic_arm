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
