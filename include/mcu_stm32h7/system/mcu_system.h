/**
 * @defgroup mcu_system System
 * @ingroup mcu
 * 
 * @file mcu_system.h
 * @ingroup mcu mcu_system
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "emb/emb_core.h"
#include <cstdio>

#include "../mcu_def.h"


void MX_GPIO_Init();


namespace mcu {
/// @addtogroup mcu_system
/// @{


/**
 * @brief Initializes device clock.
 * 
 * @param (none)
 * @return (none) 
 */
void initDeviceClock();


/**
 * @brief Resets device.
 * 
 */
void resetDevice();


/**
 * @brief On fatal error callback.
 * 
 * @param (none)
 * @return (none) 
 */
void fatal_error(const char* hint, int code = 0);


/**
 * @brief Delays for a number of milliseconds.
 * 
 * @param delay delay in milliseconds
 * @return (none)
 */
inline void delay_ms(uint32_t delay)
{
	HAL_Delay(delay);
}


/**
 * @brief Enables maskable interrupts.
 * 
 * @param (none)
 * @return (none)
 */
inline void enableInterrupts() { __enable_irq(); }


/**
 * @brief Disables maskable interrupts.
 * 
 * @param (none)
 * @return (none)
 */
inline void disableInterrupts() { __disable_irq(); }


/**
 * @brief Critical Section class.
 * 
 */
class CriticalSection
{
public:
	CriticalSection() { __disable_irq(); }	// disable interrupts
	~CriticalSection() { __enable_irq(); }	// enable interrupts
	static void enter() { __disable_irq(); }
	static void leave() { __enable_irq(); }
};
#define critical_section CriticalSection EMB_UNIQ_ID(__LINE__)


/// @}
} // namespace mcu


