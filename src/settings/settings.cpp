/**
 * @file settings.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "settings.h"

/* UART2 */
const mcu::uart::RxPinConfig Settings::Mcu::UART2_RX_PIN_CONFIG = {	
	.port = CN9_USART_B_RX_PORT,
	.pin = CN9_USART_B_RX_PIN,
	.afSelection = CN9_USART_B_RX_AF
};
const mcu::uart::TxPinConfig Settings::Mcu::UART2_TX_PIN_CONFIG = {
	.port = CN9_USART_B_TX_PORT,
	.pin = CN9_USART_B_TX_PIN,
	.afSelection = CN9_USART_B_TX_AF
};
const mcu::uart::Config Settings::Mcu::UART2_CONFIG = {
	.init = {	
		.BaudRate = 9600,
		.WordLength = UART_WORDLENGTH_8B,
		.StopBits = UART_STOPBITS_1,
		.Parity = UART_PARITY_NONE,
		.Mode = UART_MODE_TX_RX,
		.HwFlowCtl = UART_HWCONTROL_NONE
	},
	.advanced = {
		.AdvFeatureInit = UART_ADVFEATURE_NO_INIT
	}
};


/* CAN2 */
const mcu::can::RxPinConfig Settings::Mcu::CAN1_RX_PIN_CONFIG = {
	.port = CN9_CAN_RX_PORT,
	.pin = CN9_CAN_RX_PIN,
	.afSelection = CN9_CAN_RX_AF
};
const mcu::can::TxPinConfig Settings::Mcu::CAN1_TX_PIN_CONFIG = {
	.port = CN9_CAN_TX_PORT,
	.pin = CN9_CAN_TX_PIN,
	.afSelection = CN9_CAN_TX_AF
};
const mcu::can::Config Settings::Mcu::CAN1_CONFIG = {
	.init = {
		.FrameFormat = FDCAN_FRAME_CLASSIC,
		.Mode = FDCAN_MODE_NORMAL,
		.AutoRetransmission = ENABLE,
		.TransmitPause = DISABLE,
		.ProtocolException = ENABLE,
		.NominalPrescaler = 25,
		.NominalSyncJumpWidth = 1,
		.NominalTimeSeg1 = 13, /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
		.NominalTimeSeg2 = 2,
		.MessageRAMOffset = 0,
		.StdFiltersNbr = 0,
		.ExtFiltersNbr = 0,
		.RxFifo0ElmtsNbr = 1,
		.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8,
		.RxFifo1ElmtsNbr = 0,
		.RxBuffersNbr = 0,
		.TxEventsNbr = 0,
		.TxBuffersNbr = 0,
		.TxFifoQueueElmtsNbr = 1,
		.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION,
		.TxElmtSize = FDCAN_DATA_BYTES_8,				
	}
};


