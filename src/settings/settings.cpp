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


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"


/* UART2 */
const mcu::uart::RxPinConfig Settings::Mcu::uart2RxPinConfig = {	
	.port = CN9_USART_B_RX_PORT,
	.pin = CN9_USART_B_RX_PIN,
	.afSelection = CN9_USART_B_RX_AF
};
const mcu::uart::TxPinConfig Settings::Mcu::uart2TxPinConfig = {
	.port = CN9_USART_B_TX_PORT,
	.pin = CN9_USART_B_TX_PIN,
	.afSelection = CN9_USART_B_TX_AF
};
const mcu::uart::Config Settings::Mcu::uart2Config = {
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


/* CAN1 */
const mcu::can::RxPinConfig Settings::Mcu::can1RxPinConfig = {
	.port = CN9_CAN1_RX_PORT,
	.pin = CN9_CAN1_RX_PIN,
	.afSelection = CN9_CAN1_RX_AF
};
const mcu::can::TxPinConfig Settings::Mcu::can1TxPinConfig = {
	.port = CN9_CAN1_TX_PORT,
	.pin = CN9_CAN1_TX_PIN,
	.afSelection = CN9_CAN1_TX_AF
};
FDCAN_FilterTypeDef can1RxFilter1 = {
	.IdType = FDCAN_STANDARD_ID,
	.FilterIndex = 0,
	.FilterType = FDCAN_FILTER_MASK,
	.FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
	.FilterID1 = 0x000,
	.FilterID2 = 0x000,
};
std::vector<FDCAN_FilterTypeDef> Settings::Mcu::can1RxFilters = {can1RxFilter1};
const mcu::can::Config Settings::Mcu::can1Config = {
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
		.RxFifo0ElmtsNbr = 64,
		.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8,
		.RxFifo1ElmtsNbr = 64,
		.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8,
		.RxBuffersNbr = 0,
		.TxEventsNbr = 0,
		.TxBuffersNbr = 0,
		.TxFifoQueueElmtsNbr = 32,
		.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION,
		.TxElmtSize = FDCAN_DATA_BYTES_8,				
	}
};


/* CAN2 */
const mcu::can::RxPinConfig Settings::Mcu::can2RxPinConfig = {
	.port = CN12_CAN2_RX_PORT,
	.pin = CN12_CAN2_RX_PIN,
	.afSelection = CN12_CAN2_RX_AF
};
const mcu::can::TxPinConfig Settings::Mcu::can2TxPinConfig = {
	.port = CN12_CAN2_TX_PORT,
	.pin = CN12_CAN2_TX_PIN,
	.afSelection = CN12_CAN2_TX_AF
};
std::vector<FDCAN_FilterTypeDef> Settings::Mcu::can2RxFilters;
const mcu::can::Config Settings::Mcu::can2Config = {
	.init = {
		.FrameFormat = FDCAN_FRAME_CLASSIC,
		.Mode = FDCAN_MODE_EXTERNAL_LOOPBACK,
		.AutoRetransmission = ENABLE,
		.TransmitPause = DISABLE,
		.ProtocolException = ENABLE,
		.NominalPrescaler = 25,
		.NominalSyncJumpWidth = 1,
		.NominalTimeSeg1 = 13, /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
		.NominalTimeSeg2 = 2,
		.MessageRAMOffset = 1280,
		.StdFiltersNbr = 0,
		.ExtFiltersNbr = 0,
		.RxFifo0ElmtsNbr = 64,
		.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8,
		.RxFifo1ElmtsNbr = 64,
		.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8,
		.RxBuffersNbr = 0,
		.TxEventsNbr = 0,
		.TxBuffersNbr = 0,
		.TxFifoQueueElmtsNbr = 32,
		.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION,
		.TxElmtSize = FDCAN_DATA_BYTES_8,				
	}
};


