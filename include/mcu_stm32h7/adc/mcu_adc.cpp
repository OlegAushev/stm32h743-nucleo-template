/**
 * @file mcu_adc.cpp
 * @ingroup mcu mcu_adc
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_adc.h"



extern "C" void DMA1_Stream1_IRQHandler(void)
{
	using namespace mcu::adc;
	HAL_DMA_IRQHandler(Module<Peripheral::ADC_3>::instance().handle().DMA_Handle);
}

extern "C" void DMA1_Stream2_IRQHandler(void)
{
	using namespace mcu::adc;
	HAL_DMA_IRQHandler(Module<Peripheral::ADC_3>::instance().handle().DMA_Handle);
}

extern "C" void DMA1_Stream3_IRQHandler(void)
{
	using namespace mcu::adc;
	HAL_DMA_IRQHandler(Module<Peripheral::ADC_3>::instance().handle().DMA_Handle);
}