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
#include "../dma/mcu_dma.h"


namespace mcu {


namespace adc {


inline constexpr bool STRICT_ERROR_CONTROL = true;


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
protected:
	void enableClock(Peripheral instance)
	{
		switch (instance)
		{
		case Peripheral::ADC_1:
		case Peripheral::ADC_2:
			if (m_isClockEnabled[0]) return;
			__HAL_RCC_ADC12_CLK_ENABLE();
			m_isClockEnabled[0] = true;
			break;
		case Peripheral::ADC_3:
			if (m_isClockEnabled[1]) return;
			__HAL_RCC_ADC3_CLK_ENABLE();
			m_isClockEnabled[1] = true;
			break;
		default:
			fatal_error("invalid ADC module");
			break;
		}
	}
};


} // namespace impl



template <Peripheral Instance>
class Module : public impl::ModuleBase, private emb::noncopyable, public emb::interrupt_invoker<Module<Instance>>
{
private:
	ADC_HandleTypeDef m_handle{};
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

		enableClock(Instance);

		m_handle.Init = cfg.init;
		if (HAL_ADC_Init(&m_handle) != HAL_OK)
		{
			fatal_error("ADC module initialization failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @return ADC_HandleTypeDef& 
	 */
	ADC_HandleTypeDef& handle() { return m_handle; }

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


	template <dma::Peripheral DmaInstance>
	void linkDma(dma::Stream<DmaInstance>& dmaStream)
	{
		m_handle.DMA_Handle = &dmaStream.handle();
		dmaStream.handle().Parent = &m_handle;
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
	HalStatus startRegularConversion()
	{
		HalStatus status = HAL_ADC_Start(&m_handle);
		if constexpr (STRICT_ERROR_CONTROL)
		{
			if (status != HAL_OK)
			{
				fatal_error("regular ADC conversion start failed");
			}
		}
		return status;
	}


	template <uint32_t DmaBufSize>
	HalStatus startRegularConversionWithDma(mcu::dma::Buffer<uint16_t, DmaBufSize>& buf)
	{
		HalStatus status = HAL_ADC_Start_DMA(&m_handle, reinterpret_cast<uint32_t*>(buf.data), buf.size);
		if constexpr (STRICT_ERROR_CONTROL)
		{
			if (status != HAL_OK)
			{
				fatal_error("regular ADC conversion with DMA start failed");
			}
		}
		return status;
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
	uint32_t readRegularConversion()
	{
		return HAL_ADC_GetValue(&m_handle);
	}

	/**
	 * @brief 
	 * 
	 * @param injectedRank 
	 * @return uint32_t 
	 */
	uint32_t readInjectedConversion(uint32_t injectedRank)
	{
		return HAL_ADCEx_InjectedGetValue(&m_handle, injectedRank);
	}

	/* INTERRUPTS */
public:
	static inline std::function<void()> onHalfCompleted = [](){ emb::fatal_error("uninitialized callback"); };
	static inline std::function<void()> onCompleted = [](){ emb::fatal_error("uninitialized callback"); };
	static inline std::function<void()> onError = [](){ emb::fatal_error("uninitialized callback"); };
};









} // namespace adc


} // namespace mcu


