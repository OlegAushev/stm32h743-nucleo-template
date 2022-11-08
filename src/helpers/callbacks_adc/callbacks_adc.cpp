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
	mcu::gpio::DurationLogger<mcu::gpio::DurationLoggerMode::SET_RESET> dl(GPIOC, GPIO_PIN_11);
	adc3DmaBuffer.invalidateDCache(0, adc3DmaBuffer.size()/2);
	Sysinfo::saveMcuTemperature(mcu::calculateMcuTemperature(adc3DmaBuffer[0]));
}



void onAdc3DmaCompleted()
{
	mcu::gpio::DurationLogger<mcu::gpio::DurationLoggerMode::SET_RESET> dl(GPIOC, GPIO_PIN_12);
	adc3DmaBuffer.invalidateDCache(adc3DmaBuffer.size()/2, adc3DmaBuffer.size()/2);
	Sysinfo::saveMcuVref(mcu::calculateMcuVref(adc3DmaBuffer[1]));
}



void onAdc3DmaError()
{
	mcu::fatal_error("adc3 dma error");
}