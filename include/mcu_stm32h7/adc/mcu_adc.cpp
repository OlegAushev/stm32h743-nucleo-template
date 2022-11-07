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


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* handle)
{
	using namespace mcu::adc;
	if (handle->Instance == ADC1)
	{
		Module<Peripheral::ADC_1>::instance().onDmaHalfDone();
	}
	else if (handle->Instance == ADC2)
	{
		Module<Peripheral::ADC_2>::instance().onDmaHalfDone();
	}
	else if (handle->Instance == ADC3)
	{
		Module<Peripheral::ADC_3>::instance().onDmaHalfDone();
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* handle)
{
	using namespace mcu::adc;
	if (handle->Instance == ADC1)
	{
		Module<Peripheral::ADC_1>::instance().onDmaDone();
	}
	else if (handle->Instance == ADC2)
	{
		Module<Peripheral::ADC_2>::instance().onDmaDone();
	}
	else if (handle->Instance == ADC3)
	{
		Module<Peripheral::ADC_3>::instance().onDmaDone();
	}
}


void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *handle)
{
	using namespace mcu::adc;
	if (handle->Instance == ADC1)
	{
		Module<Peripheral::ADC_1>::instance().onDmaError();
	}
	else if (handle->Instance == ADC2)
	{
		Module<Peripheral::ADC_2>::instance().onDmaError();
	}
	else if (handle->Instance == ADC3)
	{
		Module<Peripheral::ADC_3>::instance().onDmaError();
	}
}