/**
 * @file main.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "stm32h7xx.h"

#include "mcu_stm32h7/system/mcu_system.h"
#include "mcu_stm32h7/gpio/mcu_gpio.h"
#include "mcu_stm32h7/uart/mcu_uart.h"
#include "mcu_stm32h7/clock/mcu_clock.h"

#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"
#include "bsp_h743_nucleo/leds/leds.h"
#include "bsp_h743_nucleo/button/button.h"

#include "cli/cli_server.h"
#include "cli/shell/cli_shell.h"

#include "build/generated/git_version.h"


/**
 * @brief 
 * 
 * @return int 
 */
int main()
{
	/* HAL, CLOCKS */
	HAL_Init();
	mcu::initDeviceClock();
	mcu::delay_ms(500);
	mcu::gpio::enableClocks();

	/* UART */
	mcu::uart::RxPinConfig uart2RxPinConfig = {.port = CN9_USART_B_RX_PORT, .pin = CN9_USART_B_RX_PIN, .afSelection = GPIO_AF7_USART2};
	mcu::uart::TxPinConfig uart2TxPinConfig = {.port = CN9_USART_B_TX_PORT, .pin = CN9_USART_B_TX_PIN, .afSelection = GPIO_AF7_USART2};
	mcu::uart::Config uart2Config = {.init = {	
						.BaudRate = 9600,
						.WordLength = UART_WORDLENGTH_8B,
						.StopBits = UART_STOPBITS_1,
						.Parity = UART_PARITY_NONE,
						.Mode = UART_MODE_TX_RX,
						.HwFlowCtl = UART_HWCONTROL_NONE},
					.advanced = {
						.AdvFeatureInit = UART_ADVFEATURE_NO_INIT}};
	mcu::uart::Uart<2> uart2(uart2RxPinConfig, uart2TxPinConfig, uart2Config);

	/* CLI */
	cli::Server cliServer("stm32-nucleo", &uart2, nullptr, nullptr);
	cli::Shell::init();
	cliServer.registerExecCallback(cli::Shell::exec);
	cli::nextline_blocking();
	cli::nextline_blocking();
	cli::nextline_blocking();
	cli::print_blocking(CLI_WELCOME_STRING);
	cli::nextline_blocking();
	cli::print_blocking(CLI_COLOR_YELLOW);
	cli::print_blocking("stm32h743-nucleo-template | ");
	cli::print_blocking(GIT_DESCRIBE);
	cli::print_blocking(CLI_COLOR_OFF);

	/* BSP */
	cli::nextline_blocking();
	cli::print_blocking("initialize bsp... ");

	bsp::initLedGreen();
	bsp::initLedBlue();
	bsp::initLedRed();
	for (auto i = 0; i < 4; ++i)
	{
		bsp::ledGreen.set();
		mcu::delay_ms(50);
		bsp::ledGreen.reset();
		bsp::ledBlue.set();
		mcu::delay_ms(50);
		bsp::ledBlue.reset();
		bsp::ledRed.set();
		mcu::delay_ms(50);
		bsp::ledRed.reset();		
	}

	bsp::initButtonUser();
	bsp::buttonUser.initInterrupt(bsp::onButtonUserInterrupt, mcu::InterruptPriority(2));
	bsp::buttonUser.enableInterrupts();

	cli::print_blocking("done");

	/* CLOCK */
	cli::nextline_blocking();
	cli::print_blocking("configure system clock... ");

	mcu::SystemClock::init();

	cli::print_blocking("done");

	/* INFINITE LOOP */
	while (true)
	{
		mcu::SystemClock::runTasks();
		cliServer.run();
	}
}




/**
 * @brief 
 * 
 */
void bsp::onButtonUserInterrupt()
{
	if (bsp::buttonUser.read() == emb::PinState::ACTIVE)
	{
		bsp::ledBlue.set();
	}
	else
	{
		mcu::resetDevice();
	}
}







