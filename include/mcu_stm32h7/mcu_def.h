/**
 * @defgroup mcu MCU
 *
 * @file mcu_def.h
 * @ingroup mcu
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cassert>

#include "stm32h7xx.h"


namespace mcu {
/// @addtogroup mcu_system
/// @{


using HalStatus = HAL_StatusTypeDef;


/**
 * @brief Interrupt priority helper class.
 * 
 */
class InterruptPriority
{
private:
	uint32_t m_value;
public:
	explicit InterruptPriority(uint32_t value)
		: m_value(value)
	{
		assert_param(value <= 15);
	}

	uint32_t value() const { return m_value; }
};


/// @}
} // namespace mcu


