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


/// @brief 
enum class Peripheral
{
	USART_1,
	USART_2,
	USART_3,
	UART_4,
	UART_5,
	USART_6,
	UART_7,
	UART_8
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
	UART_InitTypeDef init;
	UART_AdvFeatureInitTypeDef advanced;
};


/**
 * @brief 
 * 
 * @tparam Instance 
 */
template <Peripheral Instance>
class Module : public emb::IUart, private emb::noncopyable, public emb::interrupt_invoker<Module<Instance>>
{
private:
	UART_HandleTypeDef m_handle;
	mcu::gpio::Input m_rxPin;
	mcu::gpio::Output m_txPin;

	static constexpr uint32_t TIMEOUT_ms = 1000;
public:
	/**
	 * @brief Construct a new Module object
	 * 
	 * @param rxPinCfg 
	 * @param txPinCfg 
	 * @param cfg 
	 */
	Module(const RxPinConfig& rxPinCfg, const TxPinConfig& txPinCfg, const Config& cfg)
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

		if constexpr (Instance == Peripheral::USART_1)		{ __HAL_RCC_USART1_CLK_ENABLE(); m_handle.Instance = USART1; }
		else if constexpr (Instance == Peripheral::USART_2)	{ __HAL_RCC_USART2_CLK_ENABLE(); m_handle.Instance = USART2; }
		else if constexpr (Instance == Peripheral::USART_3)	{ __HAL_RCC_USART3_CLK_ENABLE(); m_handle.Instance = USART3; }
		else if constexpr (Instance == Peripheral::UART_4)	{ __HAL_RCC_UART4_CLK_ENABLE(); m_handle.Instance = UART4; }
		else if constexpr (Instance == Peripheral::UART_5)	{ __HAL_RCC_UART5_CLK_ENABLE(); m_handle.Instance = UART5; }
		else if constexpr (Instance == Peripheral::USART_6)	{ __HAL_RCC_USART6_CLK_ENABLE(); m_handle.Instance = USART6; }
		else if constexpr (Instance == Peripheral::UART_7)	{ __HAL_RCC_UART7_CLK_ENABLE(); m_handle.Instance = UART7; }
		else if constexpr (Instance == Peripheral::UART_8)	{ __HAL_RCC_UART8_CLK_ENABLE(); m_handle.Instance = UART8; }
		else { []<bool flag=false>(){ static_assert(flag); }(); }

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


