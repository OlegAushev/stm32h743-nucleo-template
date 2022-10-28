/**
 * @file leds.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "leds.h"


namespace bsp {


mcu::gpio::Output ledGreen;
const mcu::gpio::Config ledGreenConfig = {	
	.port = GPIOB,
	.pin = {
		.Pin = GPIO_PIN_0,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = 0
	},
	.activeState = emb::PinActiveState::HIGH
};


mcu::gpio::Output ledBlue;
const mcu::gpio::Config ledBlueConfig = {	
	.port = GPIOB,
	.pin = {
		.Pin = GPIO_PIN_7,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = 0
	},
	.activeState = emb::PinActiveState::HIGH
};


mcu::gpio::Output ledRed;
const mcu::gpio::Config ledRedConfig = {
	.port = GPIOB,
	.pin = {
		.Pin = GPIO_PIN_14,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = 0
	},
	.activeState = emb::PinActiveState::HIGH
};


void initLedGreen()
{
	ledGreen.init(ledGreenConfig);
}


void initLedBlue()
{
	ledBlue.init(ledBlueConfig);
}


void initLedRed()
{
	ledRed.init(ledRedConfig);
}




} // namespace bsp


