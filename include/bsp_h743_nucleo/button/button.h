/**
 * @file button.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "mcu_stm32h7/gpio/mcu_gpio.h"


namespace bsp {


extern mcu::gpio::Input buttonUser;
extern const mcu::gpio::Config buttonUserConfig;
void initButtonUser();
void onButtonUserInterrupt();


} // namespace bsp


