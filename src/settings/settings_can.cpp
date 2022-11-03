/**
 * @file settings_can.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "settings.h"


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"


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
const FDCAN_FilterTypeDef can1RxFilter1 = {
	.IdType = FDCAN_STANDARD_ID,
	.FilterIndex = 0,
	.FilterType = FDCAN_FILTER_MASK,
	.FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
	.FilterID1 = 0x000,
	.FilterID2 = 0x000,
};
const std::vector<FDCAN_FilterTypeDef> Settings::Mcu::can1RxFilters = {can1RxFilter1};
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
const std::vector<FDCAN_FilterTypeDef> Settings::Mcu::can2RxFilters;
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


