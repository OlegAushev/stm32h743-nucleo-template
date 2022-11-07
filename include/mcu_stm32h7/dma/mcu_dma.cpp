/**
 * @file mcu_dma.cpp
 * @ingroup mcu mcu_dma
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_dma.h"


extern "C" void DMA1_Stream0_IRQHandler(void)
{
	using namespace mcu::dma;
	HAL_DMA_IRQHandler(&Stream<Peripheral::DMA1_STREAM0>::instance().handlePeripheral());
}


extern "C" void DMA1_Stream1_IRQHandler(void)
{
	using namespace mcu::dma;
	HAL_DMA_IRQHandler(&Stream<Peripheral::DMA1_STREAM1>::instance().handlePeripheral());
}


extern "C" void DMA1_Stream2_IRQHandler(void)
{
	using namespace mcu::dma;
	HAL_DMA_IRQHandler(&Stream<Peripheral::DMA1_STREAM2>::instance().handlePeripheral());
}


extern "C" void DMA1_Stream3_IRQHandler(void)
{
	using namespace mcu::dma;
	HAL_DMA_IRQHandler(&Stream<Peripheral::DMA1_STREAM3>::instance().handlePeripheral());
}


