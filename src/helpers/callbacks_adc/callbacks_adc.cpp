/**
 * @file callbacks_adc.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "callbacks_adc.h"


mcu::dma::Buffer<uint16_t, 2> adc3DmaBuffer __attribute__((section(".dma_buf_section")));



void onAdc3DmaHalfCompleted()
{
	adc3DmaBuffer.invalidateDCache(0, adc3DmaBuffer.size()/2);
	Sysinfo::saveMcuTemperature(mcu::calculateMcuTemperature<ADC_RESOLUTION_16B>(adc3DmaBuffer[0]));
}



void onAdc3DmaCompleted()
{
	adc3DmaBuffer.invalidateDCache(adc3DmaBuffer.size()/2, adc3DmaBuffer.size()/2);
	Sysinfo::saveMcuVref(mcu::calculateMcuVref<ADC_RESOLUTION_16B>(adc3DmaBuffer[1]));
}



void onAdc3DmaError()
{
	mcu::fatal_error("adc3 dma error");
}