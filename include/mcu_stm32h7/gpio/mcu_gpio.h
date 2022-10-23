/**
 * @defgroup mcu_gpio GPIO
 * @ingroup mcu
 * 
 * @file mcu_gpio.h
 * @ingroup mcu mcu_gpio
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <array>
#include <functional>
#include "emb/emb_core.h"
#include "emb/emb_interfaces/emb_gpio.h"

#include "../mcu_def.h"


namespace mcu {


namespace gpio {


struct Config
{
	GPIO_TypeDef* port;
	GPIO_InitTypeDef pin;
	emb::PinActiveState activeState;
};


/**
 * @brief Enables all gpio ports clocks.
 * 
 * @param (none)
 * @return (none)
 */
void enableClocks();


/*============================================================================*/
/**
 * @brief GPIO generic base class.
 */
class Gpio
{
protected:
	Config m_cfg;
	bool m_initialized;
	Gpio() : m_initialized(false) {}
public:
	/**
	 * @brief Returns reference to pin config.
	 * @param (none)
	 * @return Reference to pin config.
	 */
	const Config& config() const
	{
		return m_cfg;
	}

	/**
	 * @brief Returns pin number.
	 * 
	 * @return Pin number 
	 */
	uint32_t no() const { return POSITION_VAL(m_cfg.pin.Pin); }
};


/*============================================================================*/
/**
 * @brief GPIO input pin class.
 */
class Input : public emb::IGpioInput, public Gpio
{
public:
	/**
	 * @brief GPIO input pin default constructor.
	 * @param (none)
	 */
	Input() {}

	/**
	 * @brief Constructs GPIO input pin.
	 * @param cfg pin config
	 */
	Input(const Config& cfg)
	{
		init(cfg);
	}

	/**
	 * @brief Initializes GPIO input pin.
	 * @param cfg pin config
	 * @return (none)
	 */
	void init(const Config& cfg)
	{
		m_cfg = cfg;
		HAL_GPIO_Init(m_cfg.port, &m_cfg.pin);
		m_initialized = true;
	}

	/**
	 * @brief Reads pin state.
	 * @param (none)
	 * @return Pin state.
	 */
	virtual emb::PinState read() const override
	{
		assert_param(m_initialized);
		return static_cast<emb::PinState>(1
				- (HAL_GPIO_ReadPin(m_cfg.port, static_cast<uint16_t>(m_cfg.pin.Pin))
				^ static_cast<uint32_t>(m_cfg.activeState)));
	}

private:
	IRQn_Type m_irqn {NonMaskableInt_IRQn};	// use NonMaskableInt_IRQn as value for not initialized interrupt
	static std::array<std::function<void(void)>, 16> s_interruptHandlers;
public:
	/**
	 * @brief Initializes interrupt.
	 * 
	 * @param handler interrupt handler
	 * @param priority interrupt priority [0..15]
	 */
	void initInterrupt(std::function<void(void)> handler, InterruptPriority priority)
	{
		switch (m_cfg.pin.Pin)
		{
		case GPIO_PIN_0:
			m_irqn = EXTI0_IRQn;
			break;
		case GPIO_PIN_1:
			m_irqn = EXTI1_IRQn;
			break;
		case GPIO_PIN_2:
			m_irqn = EXTI2_IRQn;
			break;
		case GPIO_PIN_3:
			m_irqn = EXTI3_IRQn;
			break;
		case GPIO_PIN_4:
			m_irqn = EXTI4_IRQn;
			break;
		case GPIO_PIN_5: case GPIO_PIN_6: case GPIO_PIN_7: case GPIO_PIN_8: case GPIO_PIN_9:
			m_irqn = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_10: case GPIO_PIN_11: case GPIO_PIN_12: case GPIO_PIN_13: case GPIO_PIN_14: case GPIO_PIN_15:
			m_irqn = EXTI15_10_IRQn;
			break;
		default:
			m_irqn = NonMaskableInt_IRQn;
			return;
		}
		HAL_NVIC_SetPriority(m_irqn, priority.value(), 0);
		s_interruptHandlers[this->no()] = handler;
	}

	/**
	 * @brief Enables interrupts.
	 * 
	 * @param (none)
	 * @return (none)
	 */
	void enableInterrupts() const
	{
		if (m_irqn != NonMaskableInt_IRQn)
		{
			HAL_NVIC_EnableIRQ(m_irqn);
		}
	}

	/**
	 * @brief Disables interrupts.
	 * 
	 * @param (none)
	 * @return (none)
	 */
	void disableInterrupts() const
	{
		if (m_irqn != NonMaskableInt_IRQn)
		{
			HAL_NVIC_EnableIRQ(m_irqn);
		}
	}

	/**
	 * @brief Executes interrupt handler for specified pin.
	 * 
	 * @param pinNo Pin number
	 */
	static void onInterrupt(size_t pinNo)
	{
		assert_param(pinNo <= 15);
		s_interruptHandlers[pinNo]();
	}
};


/*============================================================================*/
/**
 * @brief GPIO output pin class.
 */
class Output : public emb::IGpioOutput, public Gpio
{
public:
	/**
	 * @brief GPIO output pin default constructor.
	 * @param (none)
	 */
	Output() {}

	/**
	 * @brief Constructs GPIO output pin.
	 * @param cfg pin config
	 */
	Output(const Config& cfg)
	{
		init(cfg);
	}

	/**
	 * @brief Initializes GPIO output pin.
	 * @param cfg pin config
	 * @return (none)
	 */
	void init(const Config& cfg)
	{
		m_cfg = cfg;
		HAL_GPIO_Init(m_cfg.port, &m_cfg.pin);
		m_initialized = true;
	}

	/**
	 * @brief Reads pin state.
	 * @param (none)
	 * @return Pin state.
	 */
	virtual emb::PinState read() const override
	{
		assert_param(m_initialized);
		return static_cast<emb::PinState>(1
				- (HAL_GPIO_ReadPin(m_cfg.port, static_cast<uint16_t>(m_cfg.pin.Pin))
				^ static_cast<uint32_t>(m_cfg.activeState)));
	}

	/**
	 * @brief Sets pin state.
	 * @param state pin state
	 * @return (none)
	 */
	virtual void set(emb::PinState state = emb::PinState::ACTIVE) const override
	{
		assert_param(m_initialized);
		HAL_GPIO_WritePin(m_cfg.port, static_cast<uint16_t>(m_cfg.pin.Pin),
				static_cast<GPIO_PinState>(1 - (static_cast<uint32_t>(state) ^ static_cast<uint32_t>(m_cfg.activeState))));
	}

	/**
	 * @brief Sets pin state to INACTIVE.
	 * @param (none)
	 * @return (none)
	 */
	virtual void reset() const override
	{
		assert_param(m_initialized);
		set(emb::PinState::INACTIVE);
	}

	/**
	 * @brief Toggles pin state.
	 * @param (none)
	 * @return (none)
	 */
	virtual void toggle() const override
	{
		assert_param(m_initialized);
		HAL_GPIO_TogglePin(m_cfg.port, static_cast<uint16_t>(m_cfg.pin.Pin));
	}
};


} // namespace gpio


} // namespace mcu


