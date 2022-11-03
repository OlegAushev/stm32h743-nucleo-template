/**
 * @file emb_gpio.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


namespace emb {


/// Pin active states
enum class PinActiveState
{
	LOW = 0,
	HIGH = 1
};


/// Pin states
enum class PinState
{
	INACTIVE = 0,
	ACTIVE = 1
};


/**
 * @brief GPIO input interface class.
 */
class IGpioInput
{
public:
	IGpioInput() {}
	virtual ~IGpioInput() {}

	virtual emb::PinState read() = 0;
	virtual int readValue() = 0;
};


/**
 * @brief GPIO output interface class.
 */
class IGpioOutput
{
public:
	IGpioOutput() {}
	virtual ~IGpioOutput() {}

	virtual emb::PinState read() = 0;
	virtual int readValue() = 0;
	virtual void set(emb::PinState state = emb::PinState::ACTIVE) = 0;
	virtual void reset() = 0;
	virtual void toggle() = 0;
};


} // namespace emb



