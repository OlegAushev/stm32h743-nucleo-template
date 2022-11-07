/**
 * @defgroup mcu_can CAN
 * @ingroup mcu
 * 
 * @file mcu_can.h
 * @ingroup mcu mcu_can
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <vector>
#include <functional>
#include "emb/emb_core.h"
#include "emb/emb_interfaces/emb_can.h"

#include "../mcu_def.h"
#include "../system/mcu_system.h"
#include "../gpio/mcu_gpio.h"


namespace mcu {


namespace can {


inline constexpr bool STRICT_ERROR_CONTROL = true;


/// @brief 
enum class Peripheral
{
	FDCAN_1,
	FDCAN_2
};


/**
 * @brief 
 * 
 */
struct RxPinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
	uint32_t afSelection;
};


/**
 * @brief 
 * 
 */
struct TxPinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
	uint32_t afSelection;
};


/**
 * @brief 
 * 
 */
struct Config
{
	FDCAN_InitTypeDef init;
};


namespace impl {


inline constexpr std::array<IRQn_Type, 16> irq0Numbers = {	
	FDCAN1_IT0_IRQn,
	FDCAN2_IT0_IRQn,
};


inline constexpr std::array<IRQn_Type, 16> irq1Numbers = {	
	FDCAN1_IT1_IRQn,
	FDCAN2_IT1_IRQn,
};


/**
 * @brief 
 * 
 */
class ModuleBase
{
private:
	static inline bool m_isClockEnabled{false};
protected:
	void enableClock()
	{
		if (m_isClockEnabled) return;
		__HAL_RCC_FDCAN_CLK_ENABLE();
		m_isClockEnabled = true;
	}
};


} // namespace impl


/**
 * @brief 
 * 
 * @tparam Instance 
 */
template <Peripheral Instance>
class Module : public impl::ModuleBase, private emb::noncopyable, public emb::interrupt_invoker<Module<Instance>>
{
	friend void ::HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef*, uint32_t);
private:
	FDCAN_HandleTypeDef m_handle{};
	mcu::gpio::Input m_rxPin;
	mcu::gpio::Output m_txPin;

	uint64_t m_txErrorCounter{0};

	static constexpr std::array<uint32_t, 9> s_dataLengthCodes = {	
		FDCAN_DLC_BYTES_0,
		FDCAN_DLC_BYTES_1,
		FDCAN_DLC_BYTES_2,
		FDCAN_DLC_BYTES_3,
		FDCAN_DLC_BYTES_4,
		FDCAN_DLC_BYTES_5,
		FDCAN_DLC_BYTES_6,
		FDCAN_DLC_BYTES_7,
		FDCAN_DLC_BYTES_8
	};
public:
	/**
	 * @brief Construct a new Module object
	 * 
	 * @param rxPinCfg 
	 * @param txPinCfg 
	 * @param cfg 
	 * @param rxFilters 
	 */
	Module(const RxPinConfig& rxPinCfg, const TxPinConfig& txPinCfg, const Config& cfg,
		const std::vector<FDCAN_FilterTypeDef>& rxFilters)
		: emb::interrupt_invoker<Module<Instance>>(this)
	{
		m_rxPin.init({
			.port = rxPinCfg.port,
			.pin = {
				.Pin = rxPinCfg.pin,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_HIGH,
				.Alternate = rxPinCfg.afSelection
			},
			.activeState = emb::PinActiveState::HIGH});

		m_txPin.init({
			.port = txPinCfg.port,
			.pin = {
				.Pin = txPinCfg.pin,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_HIGH,
				.Alternate = txPinCfg.afSelection
			},
			.activeState = emb::PinActiveState::HIGH});

		/* Initialize FDCAN */
		if constexpr (Instance == Peripheral::FDCAN_1)		{ m_handle.Instance = FDCAN1; }
		else if constexpr (Instance == Peripheral::FDCAN_2)	{ m_handle.Instance = FDCAN2; }
		else { []<bool flag=false>(){ static_assert(flag); }(); }
		
		enableClock();

		m_handle.Init = cfg.init;
		m_handle.Init.StdFiltersNbr = rxFilters.size();	// by default - 0
		if (HAL_FDCAN_Init(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module initialization failed");
		}

		/* Configure Rx filters */
		for (auto filter : rxFilters)
		{
			if (HAL_FDCAN_ConfigFilter(&m_handle, &filter) != HAL_OK)
			{
				fatal_error("CAN module Rx filter configuration failed");
			}
		}

		/* Configure global filter to reject all non-matching frames */
		HAL_FDCAN_ConfigGlobalFilter(&m_handle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
	}

	/**
	 * @brief 
	 * 
	 */
	void start()
	{
		/* Start the FDCAN module */
		if (HAL_FDCAN_Start(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module start failed");
		}
	}

	/**
	 * @brief Returns HAL handle.
	 * 
	 * @return FDCAN_HandleTypeDef& 
	 */
	FDCAN_HandleTypeDef& handle() { return m_handle; }

	/**
	 * @brief 
	 * 
	 * @param frame 
	 */
	HalStatus send(can_frame frame)
	{
		FDCAN_TxHeaderTypeDef header = {
			.Identifier = frame.id,
			.IdType = FDCAN_STANDARD_ID,
			.TxFrameType = FDCAN_DATA_FRAME,
			.DataLength = s_dataLengthCodes[frame.len],
			.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
			.BitRateSwitch = FDCAN_BRS_OFF,
			.FDFormat = FDCAN_CLASSIC_CAN,
			.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
			.MessageMarker = 0
		};

		HalStatus status = HAL_FDCAN_AddMessageToTxFifoQ(&m_handle, &header, frame.data.data());
		if (status != HAL_OK)
		{
			++m_txErrorCounter;
			if constexpr (STRICT_ERROR_CONTROL)
			{
				emb::fatal_error("CAN tx error");
			}
		}
		return status;
	}

	/**
	 * @brief 
	 * 
	 * @param header 
	 * @param data 
	 */
	HalStatus send(FDCAN_TxHeaderTypeDef& header, std::array<uint8_t, 8>& data)
	{
		HalStatus status = HAL_FDCAN_AddMessageToTxFifoQ(&m_handle, &header, data.data());
		if (status != HAL_OK)
		{
			++m_txErrorCounter;
			if constexpr (STRICT_ERROR_CONTROL)
			{
				emb::fatal_error("CAN tx error");
			}
		}
		return status;		
	}

	/* INTERRUPTS */
public:
	static inline std::function<void(can_frame)> onFifo0FrameReceived = [](auto){ emb::fatal_error("uninitialized callback"); };
	static inline std::function<void(can_frame)> onFifo1WatermarkReached = [](auto){ emb::fatal_error("uninitialized callback"); };
	
	/**
	 * @brief 
	 * 
	 * @param interrruptList 
	 * @param interruptLine 
	 */
	void initInterrupts(uint32_t interrruptList, uint32_t interruptLine)
	{
		if (HAL_FDCAN_ConfigInterruptLines(&m_handle, interrruptList, interruptLine) != HAL_OK)
		{
			emb::fatal_error("CAN interrupt configuration failed");
		}
	
		if (HAL_FDCAN_ActivateNotification(&m_handle, interrruptList, 0) != HAL_OK)
		{
			emb::fatal_error("CAN interrupt configuration failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @param fifo 
	 * @param watermark 
	 */
	void setFifoWatermark(uint32_t fifo, uint32_t watermark)
	{
		if (HAL_FDCAN_ConfigFifoWatermark(&m_handle, fifo, watermark) != HAL_OK)
		{
			emb::fatal_error("CAN interrupt configuration failed");
		}
	}

	/**
	 * @brief Set interrupt priorities
	 * 
	 * @param line0Priority 
	 * @param line1Priority 
	 */
	void setInterruptPriority(InterruptPriority line0Priority, InterruptPriority line1Priority)
	{
		HAL_NVIC_SetPriority(impl::irq0Numbers[static_cast<size_t>(Instance)], line0Priority.value(), 0);
		HAL_NVIC_SetPriority(impl::irq1Numbers[static_cast<size_t>(Instance)], line1Priority.value(), 0);
	}

	/**
	 * @brief Enables interrupts.
	 * 
	 */
	void enableInterrupts()
	{
		HAL_NVIC_EnableIRQ(impl::irq0Numbers[static_cast<size_t>(Instance)]);
		HAL_NVIC_EnableIRQ(impl::irq1Numbers[static_cast<size_t>(Instance)]);
	}

	/**
	 * @brief Disables interrupts.
	 * 
	 */
	void disableInterrupts()
	{
		HAL_NVIC_DisableIRQ(impl::irq0Numbers[static_cast<size_t>(Instance)]);
		HAL_NVIC_DisableIRQ(impl::irq1Numbers[static_cast<size_t>(Instance)]);
	}
};











} // namespace can


} // namespace mcu


