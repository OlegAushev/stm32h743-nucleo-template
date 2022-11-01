/**
 * @file mcu_can.cpp
 * @ingroup mcu mcu_can
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_can.h"


namespace mcu {


namespace can {





} // namespace can


} // namespace mcu


extern "C" void FDCAN1_IT0_IRQHandler(void)
{
	using namespace mcu::can;
	HAL_FDCAN_IRQHandler(&Module<Peripheral::FDCAN_1>::instance().handle());
}


extern "C" void FDCAN2_IT0_IRQHandler(void)
{
	using namespace mcu::can;
	HAL_FDCAN_IRQHandler(&Module<Peripheral::FDCAN_2>::instance().handle());
}


extern "C" void FDCAN1_IT1_IRQHandler(void)
{
	using namespace mcu::can;
	HAL_FDCAN_IRQHandler(&Module<Peripheral::FDCAN_1>::instance().handle());
}


extern "C" void FDCAN2_IT1_IRQHandler(void)
{
	using namespace mcu::can;
	HAL_FDCAN_IRQHandler(&Module<Peripheral::FDCAN_2>::instance().handle());
}


/*extern "C" void FDCAN_CAL_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan);
}*/


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* handle, uint32_t intrFlags)
{
	using namespace mcu::can;

	if ((intrFlags & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		do
		{
			can_frame frame;
			FDCAN_RxHeaderTypeDef header;
			HAL_FDCAN_GetRxMessage(handle, FDCAN_RX_FIFO0, &header, frame.data.data());
			frame.id = header.Identifier;
			frame.len = header.DataLength >> 16;

			if (handle->Instance == FDCAN1)
			{
				Module<Peripheral::FDCAN_1>::onFrameReceived(frame);
			}
			else
			{
				Module<Peripheral::FDCAN_2>::onFrameReceived(frame);
			}
		} while (HAL_FDCAN_GetRxFifoFillLevel(handle, FDCAN_RX_FIFO0) > 0); 
	}
}





