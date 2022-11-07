/**
 * @defgroup mcu_dma DMA
 * @ingroup mcu
 * 
 * @file mcu_dma.h
 * @ingroup mcu mcu_dma
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "../mcu_def.h"
#include "../system/mcu_system.h"


namespace mcu {


namespace dma {


/// @brief 
enum class Peripheral
{
	DMA1_STREAM0,
	DMA1_STREAM1,
	DMA1_STREAM2,
	DMA1_STREAM3,
	DMA1_STREAM4,
	DMA1_STREAM5,
	DMA1_STREAM6,
	DMA2_STREAM0,
	DMA2_STREAM1,
	DMA2_STREAM2,
	DMA2_STREAM3,
	DMA2_STREAM4,
	DMA2_STREAM5,
	DMA2_STREAM6,
};


/**
 * @brief 
 * 
 */
struct Config
{
	DMA_InitTypeDef init;
};


namespace impl {


inline constexpr std::array<IRQn_Type, 14> irqNumbers = {	
	DMA1_Stream0_IRQn,
	DMA1_Stream1_IRQn,
	DMA1_Stream2_IRQn,
	DMA1_Stream3_IRQn,
	DMA1_Stream4_IRQn,
	DMA1_Stream5_IRQn,
	DMA1_Stream6_IRQn,
	DMA2_Stream0_IRQn,
	DMA2_Stream1_IRQn,
	DMA2_Stream2_IRQn,
	DMA2_Stream3_IRQn,
	DMA2_Stream4_IRQn,
	DMA2_Stream5_IRQn,
	DMA2_Stream6_IRQn
};


/**
 * @brief 
 * 
 */
class StreamBase
{
private:
	static inline std::array<bool, 2> m_isClockEnabled{};
protected:
	void enableClock(Peripheral instance)
	{
		switch (instance)
		{
		case Peripheral::DMA1_STREAM0:
		case Peripheral::DMA1_STREAM1:
		case Peripheral::DMA1_STREAM2:
		case Peripheral::DMA1_STREAM3:
		case Peripheral::DMA1_STREAM4:
		case Peripheral::DMA1_STREAM5:
		case Peripheral::DMA1_STREAM6:
			if (m_isClockEnabled[0]) return;
			__HAL_RCC_DMA1_CLK_ENABLE();
			m_isClockEnabled[0] = true;
			break;
		case Peripheral::DMA2_STREAM0:
		case Peripheral::DMA2_STREAM1:
		case Peripheral::DMA2_STREAM2:
		case Peripheral::DMA2_STREAM3:
		case Peripheral::DMA2_STREAM4:
		case Peripheral::DMA2_STREAM5:
		case Peripheral::DMA2_STREAM6:
			if (m_isClockEnabled[1]) return;
			__HAL_RCC_DMA2_CLK_ENABLE();
			m_isClockEnabled[1] = true;
			break;
		default:
			fatal_error("invalid DMA stream");
			break;
		}
	}	
};


} // namespace impl


/**
 * @brief 
 * 
 * @tparam Instance 
 */
template <Peripheral Instance>
class Stream : public impl::StreamBase, public emb::interrupt_invoker<Stream<Instance>>
{
private:
	DMA_HandleTypeDef m_handle;
	DMA_HandleTypeDef* m_handlePeripheral{nullptr};
public:
	/**
	 * @brief Construct a new Stream object
	 * 
	 * @param cfg 
	 */
	Stream(const Config& cfg)
		: emb::interrupt_invoker<Stream<Instance>>(this)
	{
		if constexpr (Instance == Peripheral::DMA1_STREAM0) { m_handle.Instance = DMA1_Stream0; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM1) { m_handle.Instance = DMA1_Stream1; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM2) { m_handle.Instance = DMA1_Stream2; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM3) { m_handle.Instance = DMA1_Stream3; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM4) { m_handle.Instance = DMA1_Stream4; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM5) { m_handle.Instance = DMA1_Stream5; }
		else if constexpr (Instance == Peripheral::DMA1_STREAM6) { m_handle.Instance = DMA1_Stream6; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM0) { m_handle.Instance = DMA2_Stream0; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM1) { m_handle.Instance = DMA2_Stream1; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM2) { m_handle.Instance = DMA2_Stream2; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM3) { m_handle.Instance = DMA2_Stream3; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM4) { m_handle.Instance = DMA2_Stream4; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM5) { m_handle.Instance = DMA2_Stream5; }
		else if constexpr (Instance == Peripheral::DMA2_STREAM6) { m_handle.Instance = DMA2_Stream6; }
		else { []<bool flag=false>(){ static_assert(flag); }(); }

		enableClock(Instance);

		m_handle.Init = cfg.init;
		if (HAL_DMA_DeInit(&m_handle) != HAL_OK)
		{
			fatal_error("DMA stream deinitialization failed");
		}
		if (HAL_DMA_Init(&m_handle) != HAL_OK)
		{
			fatal_error("DMA stream initialization failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @return DMA_HandleTypeDef& 
	 */
	DMA_HandleTypeDef& handle() { return m_handle; }


	DMA_HandleTypeDef& handlePeripheral() { return *m_handlePeripheral; }

	/**
	 * @brief 
	 * 
	 * @param priority 
	 */
	void initInterrupts(DMA_HandleTypeDef* dmaHandlePeripheral, mcu::InterruptPriority priority)
	{
		m_handlePeripheral = dmaHandlePeripheral;
		HAL_NVIC_SetPriority(impl::irqNumbers[static_cast<size_t>(Instance)], priority.value(), 0);
	}

	/**
	 * @brief 
	 * 
	 */
	void enableInterrupts()
	{
		HAL_NVIC_EnableIRQ(impl::irqNumbers[static_cast<size_t>(Instance)]);
	}

	/**
	 * @brief 
	 * 
	 */
	void disableInterrupts()
	{
		HAL_NVIC_DisableIRQ(impl::irqNumbers[static_cast<size_t>(Instance)]);
	}
};


/**
 * @brief 
 * 
 * @tparam Size 
 */
template <typename T, uint32_t Size>
struct Buffer
{
	T data[Size] __attribute__((aligned(32)));
	static constexpr uint32_t size = Size; 
	void invalidateDCache(size_t offset_, size_t size_)
	{
		SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(&data[offset_]), size_ * sizeof(T));
	}
};


}


}