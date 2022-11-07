/**
 * @file sysconfig_uart.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "sysconfig.h"


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"


/* UART2 */
const mcu::uart::RxPinConfig sysconfig::uart2::rxPinConfig = {	
	.port = CN9_USART_B_RX_PORT,
	.pin = CN9_USART_B_RX_PIN,
	.afSelection = CN9_USART_B_RX_AF
};
const mcu::uart::TxPinConfig sysconfig::uart2::txPinConfig = {
	.port = CN9_USART_B_TX_PORT,
	.pin = CN9_USART_B_TX_PIN,
	.afSelection = CN9_USART_B_TX_AF
};
const mcu::uart::Config sysconfig::uart2::config = {
	.init = {	
		.BaudRate = 9600,
		.WordLength = UART_WORDLENGTH_8B,
		.StopBits = UART_STOPBITS_1,
		.Parity = UART_PARITY_NONE,
		.Mode = UART_MODE_TX_RX,
		.HwFlowCtl = UART_HWCONTROL_NONE
	},
	.advanced = {
		.AdvFeatureInit = UART_ADVFEATURE_NO_INIT
	}
};