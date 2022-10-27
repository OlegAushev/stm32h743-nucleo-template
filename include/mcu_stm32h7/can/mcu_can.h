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


constexpr bool STRICT_ERROR_CONTROL = true;


namespace detail {


extern std::array<FDCAN_HandleTypeDef*, 2> irqHandles;


}


struct RxPinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
	uint32_t afSelection;
};


struct TxPinConfig
{
	GPIO_TypeDef* port;
	uint32_t pin;
	uint32_t afSelection;
};


struct Config
{
	FDCAN_InitTypeDef init;
};


/**
 * @brief 
 * 
 */
class CanBase
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


/**
 * @brief 
 * 
 * @tparam Module 
 */
template <unsigned int Module>
class Can : public CanBase, private emb::noncopyable, public emb::irq_singleton<Can<Module>>
{
private:
	FDCAN_HandleTypeDef m_handle;
	mcu::gpio::Input m_rxPin;
	mcu::gpio::Output m_txPin;

	uint64_t m_txErrorCounter{0};
public:
	static constexpr std::array<uint32_t, 9> DATA_LENGTH_CODES = {	FDCAN_DLC_BYTES_0,
									FDCAN_DLC_BYTES_1,
									FDCAN_DLC_BYTES_2,
									FDCAN_DLC_BYTES_3,
									FDCAN_DLC_BYTES_4,
									FDCAN_DLC_BYTES_5,
									FDCAN_DLC_BYTES_6,
									FDCAN_DLC_BYTES_7,
									FDCAN_DLC_BYTES_8};
public:
	/**
	 * @brief Construct a new Can object
	 * 
	 * @param rxPinCfg 
	 * @param txPinCfg 
	 * @param cfg 
	 * @param rxFilters 
	 */
	Can(const RxPinConfig& rxPinCfg, const TxPinConfig& txPinCfg, const Config& cfg,
		std::vector<FDCAN_FilterTypeDef>& rxFilters)
		: emb::irq_singleton<Can<Module>>(this)
	{

		static_assert(Module == 1 || Module == 2);

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

		if constexpr (Module == 1)	{ m_handle.Instance = FDCAN1; }
		else if constexpr (Module == 2) { m_handle.Instance = FDCAN2; }
		else { fatal_error("invalid CAN module"); }

		// Register handle
		detail::irqHandles[Module-1] = &m_handle;

		enableClock();

		/* Initialize FDCAN */
		m_handle.Init = cfg.init;
		if (HAL_FDCAN_Init(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module initialization failed");
		}

		/* Configure Rx filters */


		/* Configure Rx filter */
		/*FDCAN_FilterTypeDef sFilterConfig;
		sFilterConfig.IdType = FDCAN_STANDARD_ID;
		sFilterConfig.FilterIndex = 0;
		sFilterConfig.FilterType = FDCAN_FILTER_MASK;
		sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
		sFilterConfig.FilterID1 = 0x321;
		sFilterConfig.FilterID2 = 0x7FF;
		if (HAL_FDCAN_ConfigFilter(&m_handle, &sFilterConfig) != HAL_OK)
		{
			fatal_error("CAN module Rx filter configuration failed");
		}*/



		/*assert_param(rxFilters.size() == (cfg.init.StdFiltersNbr + cfg.init.ExtFiltersNbr));
		for (auto& filter : rxFilters)
		{
			if (HAL_FDCAN_ConfigFilter(&m_handle, &filter) != HAL_OK)
			{
				fatal_error("CAN module Rx filter configuration failed");
			}
		}*/

		/* Configure global filter to reject all non-matching frames */
		//HAL_FDCAN_ConfigGlobalFilter(&m_handle, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

		/* Start the FDCAN module */
		if (HAL_FDCAN_Start(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module start failed");
		}

		//if (HAL_FDCAN_ActivateNotification(&m_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		//{
		//	fatal_error("CAN module notification error");
		//}
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
	void send(can_frame frame)
	{
		FDCAN_TxHeaderTypeDef header = {
			.Identifier = frame.id,
			.IdType = FDCAN_STANDARD_ID,
			.TxFrameType = FDCAN_DATA_FRAME,
			.DataLength = DATA_LENGTH_CODES[frame.len],
			.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
			.BitRateSwitch = FDCAN_BRS_OFF,
			.FDFormat = FDCAN_CLASSIC_CAN,
			.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
			.MessageMarker = 0
		};
		if (HAL_FDCAN_AddMessageToTxFifoQ(&m_handle, &header, frame.data.data()) != HAL_OK)
		{
			++m_txErrorCounter;
			if constexpr (STRICT_ERROR_CONTROL)
			{
				emb::fatal_error("CAN tx error");
			}
		}
	}

	/**
	 * @brief 
	 * 
	 * @param header 
	 * @param data 
	 */
	void send(FDCAN_TxHeaderTypeDef& header, std::array<uint8_t, 8>& data)
	{
		if (HAL_FDCAN_AddMessageToTxFifoQ(&m_handle, &header, data.data()) != HAL_OK)
		{
			++m_txErrorCounter;
			if constexpr (STRICT_ERROR_CONTROL)
			{
				emb::fatal_error("CAN tx error");
			}
		}		
	}


	void initRxInterrupt(void (*handler)(FDCAN_HandleTypeDef*, uint32_t RxFifo0ITs), InterruptPriority priority)
	{
		if (HAL_FDCAN_ActivateNotification(&m_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		{
			emb::fatal_error("CAN interrupt initialization failed");
		}
		HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
	}
};











} // namespace can


} // namespace mcu


