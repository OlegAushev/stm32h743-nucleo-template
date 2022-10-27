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


namespace detail {


std::array<FDCAN_HandleTypeDef*, 2> irqHandles = {nullptr, nullptr};


}


} // namespace can


} // namespace mcu


extern "C" void FDCAN1_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(mcu::can::detail::irqHandles[0]);
}


extern "C" void FDCAN2_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(mcu::can::detail::irqHandles[1]);
}


extern "C" void FDCAN1_IT1_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(mcu::can::detail::irqHandles[0]);
}


extern "C" void FDCAN2_IT1_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(mcu::can::detail::irqHandles[1]);
}


/*extern "C" void FDCAN_CAL_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan);
}*/


