/**
 * @file mcu_clock.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <array>
#include <functional>
#include "emb/emb_core.h"

#include "../mcu_def.h"


extern "C" void SysTick_Handler();


namespace mcu {


class SystemClock : private emb::noncopyable, public emb::monostate<SystemClock>
{
	friend void ::SysTick_Handler();
public:
	SystemClock() = delete;
	static void init();
private:
	static inline volatile uint64_t m_time {0};
	static constexpr uint64_t TIME_STEP = 1;
	static constexpr size_t TASK_COUNT = 4;

/* periodic tasks */
public:
	enum class TaskStatus
	{
		SUCCESS,
		FAIL
	};
private:	
	static inline std::array<uint64_t, TASK_COUNT> m_taskPeriods;
	static inline std::array<uint64_t, TASK_COUNT> m_taskTimestamps;	// timestamp of executed task
	static inline std::array<std::function<TaskStatus(void)>, TASK_COUNT> m_tasks;
	static TaskStatus emptyTask() { return TaskStatus::SUCCESS; }
public:
	/**
	 * @brief Set task period.
	 * 
	 * @param index task index
	 * @param period task period in milliseconds
	 * @return (none)
	 */
	static void setTaskPeriod(size_t index, uint64_t period)
	{
		assert(index < TASK_COUNT);
		m_taskPeriods[index] = period;
	}

	/**
	 * @brief Registers periodic task.
	 * 
	 * @param index task period
	 * @param task pointer to task function
	 * @return (none)
	 */
	static void registerTask(size_t index, std::function<TaskStatus(void)>task)
	{
		assert(index < TASK_COUNT);
		m_tasks[index] = task;
	}

/* delayed task */
private:
	static inline uint64_t m_delayedTaskStart;
	static inline uint64_t m_delayedTaskDelay;
	static inline std::function<void(void)> m_delayedTask;
public:
	/**
	 * @brief Registers delayed task.
	 * 
	 * @param task delayed task function
	 * @return (none)
	 */
	static void registerDelayedTask(std::function<void(void)> task, uint64_t delay)
	{
		m_delayedTask = task;
		m_delayedTaskStart = now();
		m_delayedTaskDelay = delay;
	}

public:
	/**
	 * @brief Returns a time point representing the current point in time.
	 * 
	 * @param (none)
	 * @return A time point representing the current time in milliseconds.
	 */
	static uint64_t now()
	{
		return m_time;
	}

	/**
	 * @brief Returns clock step.
	 * 
	 * @param (none)
	 * @return Clock step in milliseconds.
	 */
	static uint32_t step()
	{
		return TIME_STEP;
	}

	/**
	 * @brief Resets clock.
	 * 
	 * @param (none)
	 * @return (none)
	 */
	static void reset()
	{
		m_time = 0;
		m_taskTimestamps.fill(0);
	}

	/**
	 * @brief Checks and runs periodic and delayed tasks.
	 * 
	 */
	static void runTasks(); 

protected:
	/**
	 * @brief Main system clock ISR. Should be called every 1ms.
	 * 
	 * @param (none)
	 * @return (none)
	 */
	static void onInterrupt()
	{
		m_time += TIME_STEP;
	}
};








} // namespace mcu


