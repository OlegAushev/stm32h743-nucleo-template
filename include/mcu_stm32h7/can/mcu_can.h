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
#include "emb/emb_core.h"

#include "../mcu_def.h"
#include "../system/mcu_system.h"
#include "../gpio/mcu_gpio.h"


namespace mcu {


namespace can {


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
class Can : public CanBase, private emb::NonCopyable
{
private:
	FDCAN_HandleTypeDef m_handle;
	mcu::gpio::Input rxPin;
	mcu::gpio::Output txPin;
public:
	Can(const RxPinConfig& rxPinCfg, const TxPinConfig& txPinCfg, const Config& cfg,
		std::vector<FDCAN_FilterTypeDef>& rxFilters)
	{

		static_assert(Module == 1 || Module == 2);

		rxPin.init({	.port = rxPinCfg.port,
				.pin = {.Pin = rxPinCfg.pin,
					.Mode = GPIO_MODE_AF_PP,
					.Pull = GPIO_PULLUP,
					.Speed = GPIO_SPEED_FREQ_HIGH,
					.Alternate = rxPinCfg.afSelection},
				.activeState = emb::PinActiveState::HIGH});

		txPin.init({	.port = txPinCfg.port,
				.pin = {.Pin = txPinCfg.pin,
					.Mode = GPIO_MODE_AF_PP,
					.Pull = GPIO_PULLUP,
					.Speed = GPIO_SPEED_FREQ_HIGH,
					.Alternate = txPinCfg.afSelection},
				.activeState = emb::PinActiveState::HIGH});

		if constexpr (Module == 1)	{ m_handle.Instance = FDCAN1; }
		else if constexpr (Module == 2) { m_handle.Instance = FDCAN2; }
		else { fatal_error("invalid CAN module"); }

		enableClock();

		m_handle.Init = cfg.init;
		if (HAL_FDCAN_Init(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module initialization failed");
		}

		/* Configure Rx filters */
		assert_param(rxFilters.size() == (cfg.init.StdFiltersNbr + cfg.init.ExtFiltersNbr));
		for (auto& filter : rxFilters)
		{
			if (HAL_FDCAN_ConfigFilter(&m_handle, &filter) != HAL_OK)
			{
				fatal_error("CAN module Rx filter configuration failed");
			}
		}

		/* Configure global filter to reject all non-matching frames */
		//HAL_FDCAN_ConfigGlobalFilter(&m_handle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);

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

	FDCAN_HandleTypeDef& handle() { return m_handle; }
};











} // namespace can


} // namespace mcu


