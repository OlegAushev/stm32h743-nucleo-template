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
			m_isClockEnabled[0] = true;
		}
		else if (adc == ADC3)
		{
			__HAL_RCC_ADC3_CLK_ENABLE();
			m_isClockEnabled[1] = true;
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
private:
	ADC_HandleTypeDef m_handle;
public:
	/**
	 * @brief Construct a new Module object
	 * 
	 * @param cfg 
	 */
	Module(const Config& cfg)
		: emb::interrupt_invoker<Module<Instance>>(this)
	{
		if constexpr (Instance == Peripheral::ADC_1) { m_handle.Instance = ADC1; }
		else if constexpr (Instance == Peripheral::ADC_2) { m_handle.Instance = ADC2; }
		else if constexpr (Instance == Peripheral::ADC_3) { m_handle.Instance = ADC3; }
		else { []<bool flag=false>(){ static_assert(flag); }(); }

		enableClock(m_handle.Instance);

		m_handle.Init = cfg.init;
		if (HAL_ADC_Init(&m_handle) != HAL_OK)
		{
			fatal_error("ADC module initialization failed");
		}
	}

	/**
	 * @brief 
	 * 
	 */
	void calibrate()
	{
		/* Run the ADC calibration in single-ended mode */
		if (HAL_ADCEx_Calibration_Start(&m_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
		{
			fatal_error("ADC calibration failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @param pinCfg 
	 * @param channelCfg 
	 */
	void addRegularChannel(PinConfig pinCfg, ADC_ChannelConfTypeDef channelCfg)
	{
		mcu::gpio::Input input({
			.port = pinCfg.port,
			.pin = {
				.Pin = pinCfg.pin,
				.Mode = GPIO_MODE_ANALOG,
				.Pull = GPIO_NOPULL}});

		if (HAL_ADC_ConfigChannel(&m_handle, &channelCfg) != HAL_OK)
		{
			fatal_error("ADC regular channel initialization failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @param channelCfg 
	 */
	void addInternalChannel(ADC_ChannelConfTypeDef channelCfg)
	{
		if (HAL_ADC_ConfigChannel(&m_handle, &channelCfg) != HAL_OK)
		{
			fatal_error("ADC internal channel initialization failed");
		}
	}


	/**
	 * @brief 
	 * 
	 */
	void startRegular()
	{
		if (HAL_ADC_Start(&m_handle) != HAL_OK)
		{
			fatal_error("start regular ADC conversion failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @return HalStatus 
	 */
	HalStatus pollForConversion()
	{
		return HAL_ADC_PollForConversion(&m_handle, 0);
	}
	
	/**
	 * @brief 
	 * 
	 * @return uint32_t 
	 */
	uint32_t readRegular()
	{
		return HAL_ADC_GetValue(&m_handle);
	}

	/**
	 * @brief 
	 * 
	 * @param injectedRank 
	 * @return uint32_t 
	 */
	uint32_t readInjected(uint32_t injectedRank)
	{
		return HAL_ADCEx_InjectedGetValue(&m_handle, injectedRank);
	}




};









} // namespace adc


} // namespace mcu


