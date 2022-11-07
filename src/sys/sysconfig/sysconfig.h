/**
 * @file sysconfig.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "mcu_stm32h7/adc/mcu_adc.h"
#include "mcu_stm32h7/uart/mcu_uart.h"
#include "mcu_stm32h7/can/mcu_can.h"
#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"


namespace sysconfig {


namespace uart2 {
extern const mcu::uart::RxPinConfig rxPinConfig;
extern const mcu::uart::TxPinConfig txPinConfig;
extern const mcu::uart::Config config;
}


namespace can1 {
extern const mcu::can::RxPinConfig rxPinConfig;
extern const mcu::can::TxPinConfig txPinConfig;
extern const std::vector<FDCAN_FilterTypeDef> rxFilters;
extern const mcu::can::Config config;
}


namespace can2 {
extern const mcu::can::RxPinConfig rxPinConfig;
extern const mcu::can::TxPinConfig txPinConfig;
extern const std::vector<FDCAN_FilterTypeDef> rxFilters;
extern const mcu::can::Config config;
}


namespace adc3 {
extern const mcu::adc::Config config;

namespace channels {
extern const ADC_ChannelConfTypeDef internalTemp;
extern const ADC_ChannelConfTypeDef internalVref;
}

}


} // namespace sysconfig


