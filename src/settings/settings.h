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

#include "mcu_stm32h7/adc/mcu_adc.h"
#include "mcu_stm32h7/uart/mcu_uart.h"
#include "mcu_stm32h7/can/mcu_can.h"
#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"


class Settings : public emb::monostate<Settings>
{
public:
	static void init()
	{
		set_initialized();
	}
public:
	struct Mcu
	{
		static const mcu::uart::RxPinConfig uart2RxPinConfig;
		static const mcu::uart::TxPinConfig uart2TxPinConfig;
		static const mcu::uart::Config uart2Config;
		
		static const mcu::can::RxPinConfig can1RxPinConfig;
		static const mcu::can::TxPinConfig can1TxPinConfig;
		static const std::vector<FDCAN_FilterTypeDef> can1RxFilters;
		static const mcu::can::Config can1Config;

		static const mcu::can::RxPinConfig can2RxPinConfig;
		static const mcu::can::TxPinConfig can2TxPinConfig;
		static const std::vector<FDCAN_FilterTypeDef> can2RxFilters;
		static const mcu::can::Config can2Config;

		static const mcu::adc::Config adc3Config;
	} mcu;

	struct AdcChannels
	{
		static const ADC_ChannelConfTypeDef internalTempChannelConfig;
	} adcChannels;
};


