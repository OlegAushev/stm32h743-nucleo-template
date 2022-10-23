/**
 * @file leds.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "mcu_stm32h7/gpio/mcu_gpio.h"


namespace bsp {


extern mcu::gpio::Output ledGreen;
extern const mcu::gpio::Config ledGreenConfig;

extern mcu::gpio::Output ledBlue;
extern const mcu::gpio::Config ledBlueConfig;

extern mcu::gpio::Output ledRed;
extern const mcu::gpio::Config ledRedConfig;


void initLedGreen();
void initLedBlue();
void initLedRed();


} // namespace bsp


