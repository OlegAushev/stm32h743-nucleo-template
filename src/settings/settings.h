/**
 * @file settings.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "emb/emb_core.h"

#include "mcu_stm32h7/uart/mcu_uart.h"
#include "mcu_stm32h7/can/mcu_can.h"
#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"


class Settings : public emb::Monostate<Settings>
{
public:
	static void init()
	{
		setInitialized();
	}
public:
	struct Mcu
	{
		static const mcu::uart::RxPinConfig UART2_RX_PIN_CONFIG;
		static const mcu::uart::TxPinConfig UART2_TX_PIN_CONFIG;
		static const mcu::uart::Config UART2_CONFIG;
		
		static const mcu::can::RxPinConfig CAN1_RX_PIN_CONFIG;
		static const mcu::can::TxPinConfig CAN1_TX_PIN_CONFIG;
		static const mcu::can::Config CAN1_CONFIG;
	} mcu;
};


