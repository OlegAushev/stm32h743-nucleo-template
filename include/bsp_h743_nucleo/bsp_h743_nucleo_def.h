/**
 * @file bsp_h743_nucleo_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#define CN9_USART_B_RX_PORT	GPIOD
#define CN9_USART_B_RX_PIN	GPIO_PIN_6
#define CN9_USART_B_RX_AF	GPIO_AF7_USART2

#define CN9_USART_B_TX_PORT	GPIOD
#define CN9_USART_B_TX_PIN	GPIO_PIN_5
#define CN9_USART_B_TX_AF	GPIO_AF7_USART2


#define CN9_CAN_RX_PORT		GPIOD
#define CN9_CAN_RX_PIN		GPIO_PIN_0
#define CN9_CAN_RX_AF		GPIO_AF9_FDCAN1

#define CN9_CAN_TX_PORT		GPIOD
#define CN9_CAN_TX_PIN		GPIO_PIN_1
#define CN9_CAN_TX_AF		GPIO_AF9_FDCAN1


