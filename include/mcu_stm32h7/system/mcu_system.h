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


/**
 * @brief 
 * 
 */
inline void enableICache() { SCB_EnableICache(); }

 
/**
 * @brief 
 * 
 */
inline void disableICache() { SCB_DisableICache(); }


/**
 * @brief 
 * 
 */
inline void enableDCache() { SCB_EnableDCache(); }


/**
 * @brief 
 * 
 */
inline void disableDCache() { SCB_DisableDCache(); }


/**
 * @brief 
 * 
 * @tparam AdcResolution 
 * @param adcData left-aligned 16-bit ADC result
 * @return float 
 */
inline float calculateMcuTemperature(uint32_t adcData)
{
	return float(int32_t(adcData) - int32_t(*TEMPSENSOR_CAL1_ADDR)) * (float(TEMPSENSOR_CAL2_TEMP) - float(TEMPSENSOR_CAL1_TEMP))
			/ (int32_t(*TEMPSENSOR_CAL2_ADDR) - int32_t(*TEMPSENSOR_CAL1_ADDR)) + float(TEMPSENSOR_CAL1_TEMP); 
}


/**
 * @brief 
 * 
 * @tparam AdcResolution 
 * @param adcData left-aligned 16-bit ADC result
 * @return float 
 */
inline float calculateMcuVref(uint32_t adcData)
{
	return 3.3f * float(*VREFINT_CAL_ADDR) / float(adcData);
}





/// @}
} // namespace mcu


