/**
 * @defgroup mcu_uart UART
 * @ingroup mcu
 * 
 * @file mcu_uart.h
 * @ingroup mcu mcu_uart
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "emb/emb_interfaces/emb_uart.h"
#include "emb/emb_core.h"

#include "../mcu_def.h"
#include "../system/mcu_system.h"
#include "../gpio/mcu_gpio.h"


namespace mcu {


namespace uart {


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
	UART_InitTypeDef init;
	UART_AdvFeatureInitTypeDef advanced;
};


template <unsigned int Module>
class Uart : public emb::IUart, private emb::NonCopyable
{
private:
	UART_HandleTypeDef m_handle;
	mcu::gpio::Input rxPin;
	mcu::gpio::Output txPin;

	static constexpr uint32_t TIMEOUT_ms = 1000;
public:
	/**
	 * @brief Construct a new Uart object
	 * 
	 * @param rxPinCfg 
	 * @param txPinCfg 
	 * @param cfg 
	 */
	Uart(const RxPinConfig& rxPinCfg, const TxPinConfig& txPinCfg, const Config& cfg)
	{
		static_assert(Module >= 1 && Module <= 8);

		rxPin.init({
			.port = rxPinCfg.port, 
			.pin = {
				.Pin = rxPinCfg.pin,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_HIGH,
				.Alternate = rxPinCfg.afSelection
			},
			.activeState = emb::PinActiveState::HIGH});
				
		txPin.init({
			.port = txPinCfg.port,
			.pin = {
				.Pin = txPinCfg.pin,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_HIGH,
				.Alternate = txPinCfg.afSelection
			},
			.activeState = emb::PinActiveState::HIGH});

		if constexpr (Module == 1)	{ __HAL_RCC_USART1_CLK_ENABLE(); m_handle.Instance = USART1; }
		else if constexpr (Module == 2)	{ __HAL_RCC_USART2_CLK_ENABLE(); m_handle.Instance = USART2; }
		else if constexpr (Module == 3)	{ __HAL_RCC_USART3_CLK_ENABLE(); m_handle.Instance = USART3; }
		else if constexpr (Module == 4)	{ __HAL_RCC_UART4_CLK_ENABLE(); m_handle.Instance = UART4; }
		else if constexpr (Module == 5)	{ __HAL_RCC_UART5_CLK_ENABLE(); m_handle.Instance = UART5; }
		else if constexpr (Module == 6)	{ __HAL_RCC_USART6_CLK_ENABLE(); m_handle.Instance = USART6; }
		else if constexpr (Module == 7)	{ __HAL_RCC_UART7_CLK_ENABLE(); m_handle.Instance = UART7; }
		else if constexpr (Module == 8)	{ __HAL_RCC_UART8_CLK_ENABLE(); m_handle.Instance = UART8; }
		else { fatal_error("invalid UART module"); }

		m_handle.Init = cfg.init;
		m_handle.AdvancedInit = cfg.advanced;
		if (HAL_UART_Init(&m_handle) != HAL_OK)
		{
			fatal_error("UART module initialization failed");
		}
	}
	

	virtual int recv(char& ch) override
	{
		if (HAL_UART_Receive(&m_handle, reinterpret_cast<uint8_t*>(&ch), 1, 0) != HAL_OK)
		{
			return 0;
		}
		return 1;
	}


	virtual int recv(char* buf, size_t len) override
	{
		size_t i = 0;
		char ch = 0;

		while ((i < len) && (recv(ch) == 1))
		{
			buf[i++] = ch;
		}
		return i;
	}


	virtual int send(char ch) override
	{
		if (HAL_UART_Transmit(&m_handle, reinterpret_cast<const uint8_t*>(&ch), 1, TIMEOUT_ms) != HAL_OK)
		{
			return 0;
		}
		return 1;
	}


	virtual int send(const char* buf, size_t len) override
	{
		if (HAL_UART_Transmit(&m_handle, reinterpret_cast<const uint8_t*>(buf), static_cast<uint16_t>(len), TIMEOUT_ms) != HAL_OK)
		{
			return 0;
		}
		return 1;
	}
};


} // namespace uart


} // namespace mcu


