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


/// @brief 
enum class Peripheral
{
	ADC_1,
	ADC_2,
	ADC_3
};


/**
 * @brief 
 * 
 */
struct PinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
};


/**
 * @brief 
 * 
 */
struct Config
{
	ADC_InitTypeDef init;
	ADC_ChannelConfTypeDef channel; 
};


namespace impl {


inline constexpr std::array<ADC_TypeDef*, 3> adcModules = {ADC1, ADC2, ADC3};


/**
 * @brief 
 * 
 */
class ModuleBase
{
private:
	static inline std::array<bool, 2> m_isClockEnabled{};
public:
	void enableClock(ADC_TypeDef* adc)
	{
		if ((adc == ADC1) || (adc == ADC2))
		{
			__HAL_RCC_ADC12_CLK_ENABLE();
		}
		else if (adc == ADC3)
		{
			__HAL_RCC_ADC3_CLK_ENABLE();
		}
		else
		{
			fatal_error("invalid ADC module");
		}
	}
};


} // namespace impl



template <Peripheral Instance>
class Module : public impl::ModuleBase, private emb::noncopyable, public emb::interrupt_invoker<Module<Instance>>
{

};









} // namespace adc


} // namespace mcu


