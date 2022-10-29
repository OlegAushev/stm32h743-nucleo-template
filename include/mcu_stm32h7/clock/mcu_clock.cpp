/**
 * @file mcu_clock.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_clock.h"


/**
 * @brief Mandatory HAL-lib tick handler.
 * 
 */
extern "C" void SysTick_Handler()
{
	HAL_IncTick();
	mcu::SystemClock::onInterrupt();
}


namespace mcu {


/// 
///
///
void SystemClock::init()
{
	for (auto task : m_tasks)
	{
		task.period = 0;
		task.timestamp = 0;
		task.func = emptyTask;
	}

	m_delayedTaskStart = 0;
	m_delayedTaskDelay = 0;
	m_delayedTask = emb::empty_function;
	
	set_initialized();
}


///
///
///
void SystemClock::runTasks()
{
	for (auto task : m_tasks)
	{
		if (task.period != 0)
		{
			if (now() >= (task.timestamp + task.period))
			{
				if (task.func() == TaskStatus::SUCCESS)
				{
					task.timestamp = now();
				}
			}
		}
	}

	if (m_delayedTaskDelay != 0)
	{
		if (now() >= (m_delayedTaskStart + m_delayedTaskDelay))
		{
			m_delayedTask();
			m_delayedTaskDelay = 0;
		}
	}
}


} // namespace mcu


