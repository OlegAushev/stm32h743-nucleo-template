/**
 * @file mcu_stm32h7_test.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "emb/emb_testrunner/emb_testrunner.h"

#include "mcu_stm32h7/system/mcu_system.h"
#include "mcu_stm32h7/gpio/mcu_gpio.h"
#include "mcu_stm32h7/clock/mcu_clock.h"
#include "bsp_h743_nucleo/leds/leds.h"



class McuTest
{
public:
	static void GpioTest();
	static void ClockTest();
};


