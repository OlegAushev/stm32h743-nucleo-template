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

	int i = 0;
	while (true)
	{
		HAL_Delay(1000);
		++i;
	}
}











