/**
 * @file button.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "button.h"


namespace bsp {


mcu::gpio::Input buttonUser;
const mcu::gpio::Config buttonUserConfig = {
	.port = GPIOC,
	.pin = {
		.Pin = GPIO_PIN_13,
		.Mode = GPIO_MODE_IT_RISING_FALLING,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = 0
	},
	.activeState = emb::PinActiveState::HIGH
};


void initButtonUser()
{
	buttonUser.init(buttonUserConfig);
}


__attribute__((weak)) void onButtonUserInterrupt()
{
	/* DO NOTHING */
}


} // namespace bsp


