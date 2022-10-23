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
	//CAN_InitTypeDef init;
};


template <unsigned int Module>
class Can : private emb::NonCopyable
{
private:
	//CAN_HandleTypeDef m_handle;
	mcu::gpio::Input rxPin;
	mcu::gpio::Output txPin;
public:
	Can(const RxPinConfig& rxPinCfg, TxPinConfig txPinCfg, const Config& cfg)
	{
#if defined(STM32F7)
		static_assert(Module == 1);
#elif defined(STM32H7)
		static_assert(Module == 1 || Module == 2);
#else
#error "CAN driver is unimplemented."
#endif
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

		//if constexpr (Module == 1)	{ __HAL_RCC_CAN1_CLK_ENABLE(); m_handle.Instance = CAN1; }
#if defined(STM32H7)
		//else if constexpr (Module == 2) { __HAL_RCC_CAN2_CLK_ENABLE(); m_handle.Instance = CAN2; }
#endif
		//else { fatal_error("invalid CAN module"); }

		//m_handle.Init = cfg.init;
		//if (HAL_CAN_Init(&m_handle) != HAL_OK)
		{
			fatal_error("CAN module initialization failed");
		}

	}
};











} // namespace can


} // namespace mcu


