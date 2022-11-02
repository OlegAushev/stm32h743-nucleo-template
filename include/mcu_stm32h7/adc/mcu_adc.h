/**
 * @defgroup mcu_adc ADC
 * @ingroup mcu
 * 
 * @file mcu_adc.h
 * @ingroup mcu mcu_adc
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "../mcu_def.h"
#include "../system/mcu_system.h"
#include "../gpio/mcu_gpio.h"


namespace mcu {


namespace adc {


struct PinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
};


struct Config
{
	ADC_InitTypeDef init;
	ADC_ChannelConfTypeDef channel; 
};





namespace detail {
	inline constexpr std::array<ADC_TypeDef*, 3> adcModules = {ADC1, ADC2, ADC3};
	inline std::array<std::function<void(void)>, 3> adcClkEnableFuncs = {
		[](){__HAL_RCC_ADC12_CLK_ENABLE();},
		[](){__HAL_RCC_ADC12_CLK_ENABLE();},
		[](){__HAL_RCC_ADC3_CLK_ENABLE();}	
	};
}






} // namespace adc


} // namespace mcu


