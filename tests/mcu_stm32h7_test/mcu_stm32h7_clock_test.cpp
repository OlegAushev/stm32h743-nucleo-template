/**
 * @file mcu_stm32h7_clock_test.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_stm32h7_test.h"


void McuTest::ClockTest()
{
	auto taskLedToggle = []()
	{
		bsp::ledGreen.toggle();
		return mcu::SystemClock::TaskStatus::SUCCESS;
	};
	mcu::SystemClock::registerTask(0, taskLedToggle);
	mcu::SystemClock::setTaskPeriod(0, 100);

	mcu::SystemClock::registerDelayedTask([](){ bsp::ledRed.toggle(); }, 200);

	for (auto i = 1; i <= 4; ++i)
	{
		mcu::delay_ms(101);
		mcu::SystemClock::runTasks();
		if (i % 2 != 0)
		{
			EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::ACTIVE);
		}
		else
		{
			EMB_ASSERT_EQUAL(bsp::ledGreen.read(), emb::PinState::INACTIVE);
		}

		if ((i < 2) || (i == 4))
		{
			EMB_ASSERT_EQUAL(bsp::ledRed.read(), emb::PinState::INACTIVE);
		}
		else
		{
			EMB_ASSERT_EQUAL(bsp::ledRed.read(), emb::PinState::ACTIVE);
		}

		if (i == 2)
		{
			mcu::SystemClock::registerDelayedTask([](){ bsp::ledRed.toggle(); }, 200);
		}
	}
}