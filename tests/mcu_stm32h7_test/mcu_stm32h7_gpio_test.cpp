/**
 * @file mcu_stm32h7_gpio_test.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_stm32h7_test.h"


void McuTest::GpioTest()
{
	bsp::ledGreen.reset();
	EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::INACTIVE);
	EMB_ASSERT_EQUAL(bsp::ledGreen.readValue(), 0);

	bsp::ledGreen.set();
	EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::ACTIVE);
	EMB_ASSERT_EQUAL(bsp::ledGreen.readValue(), 1);

	bsp::ledGreen.reset();
	bsp::ledGreen.deinit();

	mcu::gpio::Config altConfig = bsp::ledGreenConfig;
	altConfig.activeState = emb::PinActiveState::LOW;

	bsp::ledGreen.init(altConfig);
	bsp::ledGreen.reset();
	EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::INACTIVE);
	EMB_ASSERT_EQUAL(bsp::ledGreen.readValue(), 1);

	bsp::ledGreen.set();
	EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::ACTIVE);
	EMB_ASSERT_EQUAL(bsp::ledGreen.readValue(), 0);

	bsp::ledGreen.deinit();
	bsp::ledGreen.init(bsp::ledGreenConfig);
}


