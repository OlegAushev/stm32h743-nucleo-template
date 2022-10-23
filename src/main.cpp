/**
 * @file main.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "stm32h7xx.h"

#include "mcu_stm32h7/system/mcu_system.h"
#include "mcu_stm32h7/gpio/mcu_gpio.h"

#include "bsp_h743_nucleo/leds/leds.h"


/**
 * @brief 
 * 
 * @return int 
 */
int main()
{
	HAL_Init();
	mcu::initDeviceClock();
	HAL_Delay(500);
	mcu::gpio::enableClocks();

	bsp::initLedGreen();
	bsp::initLedBlue();
	bsp::initLedRed();

	while (true)
	{
		bsp::ledGreen.set();
		mcu::delay_ms(500);
		bsp::ledBlue.set();
		mcu::delay_ms(500);
		bsp::ledRed.set();
		mcu::delay_ms(500);

		bsp::ledGreen.reset();
		mcu::delay_ms(500);
		bsp::ledBlue.reset();
		mcu::delay_ms(500);
		bsp::ledRed.reset();
		mcu::delay_ms(500);
	}
}











