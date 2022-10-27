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
	m_taskPeriods.fill(0);
	m_taskTimestamps.fill(0);
	m_tasks.fill(emptyTask);

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
	for (size_t i = 0; i < TASK_COUNT; ++i)
	{
		if (m_taskPeriods[i] != 0)
		{
			if (now() >= (m_taskTimestamps[i] + m_taskPeriods[i]))
			{
				if (m_tasks[i]() == TaskStatus::SUCCESS)
				{
					m_taskTimestamps[i] = now();
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


