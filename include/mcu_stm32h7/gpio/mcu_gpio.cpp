/**
 * @file mcu_gpio.cpp
 * @ingroup mcu mcu_gpio
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mcu_gpio.h"


/// 
///
///
void mcu::gpio::enableClocks()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
}


std::array<std::function<void(void)>, 16> mcu::gpio::Input::onInterrupt = {
	emb::invalid_function, emb::invalid_function, emb::invalid_function, emb::invalid_function,
	emb::invalid_function, emb::invalid_function, emb::invalid_function, emb::invalid_function,
	emb::invalid_function, emb::invalid_function, emb::invalid_function, emb::invalid_function,
	emb::invalid_function, emb::invalid_function, emb::invalid_function, emb::invalid_function,
};


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI0_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	mcu::gpio::Input::onInterrupt[0]();
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI1_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	mcu::gpio::Input::onInterrupt[1]();
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI2_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	mcu::gpio::Input::onInterrupt[2]();
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI3_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	mcu::gpio::Input::onInterrupt[3]();
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI4_IRQHandler()
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
	mcu::gpio::Input::onInterrupt[4]();
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI9_5_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);	
}


/**
 * @brief 
 * 
 * @param (none)
 * @return (none)
 */
extern "C" void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


/**
 * @brief 
 * 
 * @param GPIO_Pin 
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	size_t pinNo = POSITION_VAL(GPIO_Pin);
	assert_param(pinNo <= 15);
	mcu::gpio::Input::onInterrupt[pinNo]();
}


