/*
 * can_lib.c
 *
 *  Created on: 12 wrz 2026
 *      Author: styka
 */
#include "can_lib.h"



void Can_init(CAN_HandleTypeDef* hcan,uint32_t node_id,uint32_t mask)
{
	//konfiguracja cana na standardowe wiadomosci i filtr na id

	CAN_FilterTypeDef filter = {0};
	filter.FilterIdHigh = node_id << 5; // id std shift mask
	filter.FilterMaskIdHigh = mask << 5;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	filter.FilterActivation = ENABLE;
	HAL_CAN_ConfigFilter(hcan, &filter);
	HAL_CAN_Start(hcan);
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
};

uint8_t filter_cnt = 0;


void Can_Set_Strict_Filter(CAN_HandleTypeDef* hcan, uint32_t my_motor_id)
{
    CAN_FilterTypeDef filter = {0};

    // 1. Wzorzec: Podajemy dokładne ID naszego węzła (przesunięte o 5 bitów w lewo dla Standard ID)
    filter.FilterIdHigh = my_motor_id << 5;
    filter.FilterIdLow = 0; // Dolna połowa rejestru (nas nie interesuje dla 11-bitowych ID)

    // 2. Maska: 0x7FF wymusza sprawdzenie każdego z 11 bitów.
    // Jeśli choć jeden bit w przychodzącej wiadomości będzie inny, ramka zostanie odrzucona.
    filter.FilterMaskIdHigh = 0x7FF << 5;
    filter.FilterMaskIdLow = 0;

    // 3. Konfiguracja sprzętowa
    filter.FilterBank = filter_cnt++;                      // Używamy pierwszego banku filtrów
    filter.FilterMode = CAN_FILTERMODE_IDMASK;  // Tryb z maską
    filter.FilterScale = CAN_FILTERSCALE_32BIT; // Używamy pełnego, 32-bitowego rejestru
    filter.FilterFIFOAssignment = CAN_RX_FIFO0; // Zgodnie z naszą funkcją przerwania zrzucamy do FIFO0
    filter.FilterActivation = ENABLE;           // Włącz filtr

    // 4. Załadowanie konfiguracji do kontrolera CAN
    HAL_CAN_ConfigFilter(hcan, &filter);
}


//
RingBuf Can_Rx_Data ={0};



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//odebrac dane wpierdolic je do ringbuffera
	CAN_RxHeaderTypeDef RxHeader;

	uint8_t RxData[8];



	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK )
	{
		uint16_t next_wr_pt = (Can_Rx_Data.wr_ptr + 1) % RX_BUFFER_SIZE;

		if(next_wr_pt != Can_Rx_Data.rd_ptr)
		{
			//wrzucamy dane
			Can_Rx_Data.buffer[Can_Rx_Data.wr_ptr].id = RxHeader.StdId;
			Can_Rx_Data.buffer[Can_Rx_Data.wr_ptr].dlc = RxHeader.DLC;

			for(int i =0; i < RxHeader.DLC; i++)
			{
				Can_Rx_Data.buffer[Can_Rx_Data.wr_ptr].data[i] = RxData[i];
			}

			Can_Rx_Data.wr_ptr = next_wr_pt;
		}
		else
		{
			Can_Rx_Data.dropped_frames ++;
		}
	}

}


//teraz odczyt
bool Can_Read_Data(Can_Msg_t *msg_out)
{
	if(Can_Rx_Data.rd_ptr == Can_Rx_Data.wr_ptr)
	{
		//pusto nie ma nic do czytania
		return false;
	}
	*msg_out = Can_Rx_Data.buffer[Can_Rx_Data.rd_ptr];

	Can_Rx_Data.rd_ptr = (Can_Rx_Data.rd_ptr + 1) % RX_BUFFER_SIZE;

	return true;
}

bool Can_Send_Data(CAN_HandleTypeDef* hcan,Can_Msg_t *msg_send,uint32_t id,uint8_t *payload,uint8_t dlc)
{
	CAN_TxHeaderTypeDef TxHeader = {0};

	TxHeader.DLC = dlc;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = id;
	TxHeader.TransmitGlobalTime = DISABLE;

	uint32_t TxMailbox;

	if(HAL_CAN_AddTxMessage(hcan,&TxHeader,payload,&TxMailbox) == HAL_OK)
		{
				return true;
		}
	return false; // szkyrnki pelne albo blond

}



