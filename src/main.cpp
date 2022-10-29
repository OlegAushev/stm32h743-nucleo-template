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


#include "emb/emb_math.h"
#include "stm32h7xx.h"

#include "mcu_stm32h7/system/mcu_system.h"
#include "mcu_stm32h7/gpio/mcu_gpio.h"
#include "mcu_stm32h7/uart/mcu_uart.h"
#include "mcu_stm32h7/clock/mcu_clock.h"
#include "mcu_stm32h7/can/mcu_can.h"

#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"
#include "bsp_h743_nucleo/leds/leds.h"
#include "bsp_h743_nucleo/button/button.h"

#include "settings/settings.h"
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
	/* === HAL, CLOCKS === */
	HAL_Init();
	mcu::enableICache();
	mcu::enableDCache();
	mcu::initDeviceClock();
	mcu::delay_ms(500);
	mcu::gpio::enableClocks();


	/* === SETTINGS === */
	Settings::init();
	Settings settings;


	/* === UART === */
	mcu::uart::Uart<2> uart2(
			settings.mcu.UART2_RX_PIN_CONFIG,
			settings.mcu.UART2_TX_PIN_CONFIG,
			settings.mcu.UART2_CONFIG);


	/* === CLI === */
	cli::Server cliServer("stm32-nucleo", &uart2, nullptr, nullptr);
	cli::Shell::init();
	cliServer.registerExecCallback(cli::Shell::exec);
	cli::nextline_blocking();
	cli::nextline_blocking();
	cli::nextline_blocking();
	cli::print_blocking(CLI_WELCOME_STRING);
	cli::nextline_blocking();
	cli::print_blocking(CLI_COLOR_GREEN);
	cli::print_blocking("stm32h743-nucleo-template | ");
	cli::print_blocking(GIT_DESCRIBE);
	cli::print_blocking(CLI_COLOR_OFF);


	/* === BSP === */
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
	bsp::buttonUser.initInterrupt(bsp::onButtonUserInterrupt, mcu::InterruptPriority(15));
	bsp::buttonUser.enableInterrupts();

	cli::print_blocking("done");


	/* === GPIO PROFILER === */
	cli::nextline_blocking();
	cli::print_blocking("initialize gpio profiler pins... ");

	auto profilerPin = mcu::gpio::DurationLogger<>::init(GPIOC, GPIO_PIN_10);
	mcu::gpio::DurationLogger<>::init(GPIOC, GPIO_PIN_11);
	mcu::gpio::DurationLogger<>::init(GPIOC, GPIO_PIN_12);

	cli::print_blocking("done");


	/* === CLOCK === */
	cli::nextline_blocking();
	cli::print_blocking("configure system clock... ");

	mcu::SystemClock::init();
	auto taskLedHeartbeat = [](){
		const uint64_t periods[4] = {100, 100, 100, 700};
		static size_t index = 0;

		mcu::SystemClock::setTaskPeriod(0, periods[index]);
		if ((index % 2) == 0) { bsp::ledGreen.set(); }
		else { bsp::ledGreen.reset(); }
		index = (index + 1) % 4;
		return mcu::SystemClock::TaskStatus::SUCCESS;
	};
	mcu::SystemClock::registerTask(0, taskLedHeartbeat);
	mcu::SystemClock::setTaskPeriod(0, 2000);

	cli::print_blocking("done");


	/* === CAN1 === */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN1 module... ");

	std::vector<FDCAN_FilterTypeDef> can1RxFilters;
	mcu::can::Can<1> can1(
			settings.mcu.CAN1_RX_PIN_CONFIG,
			settings.mcu.CAN1_TX_PIN_CONFIG,
			settings.mcu.CAN1_CONFIG,
			can1RxFilters);
	
	auto can1Loop = [&can1](can_frame frame)
	{
		can1.send(frame);
		bsp::ledGreen.toggle();
	};
	can1.initRxInterrupt(can1Loop, mcu::InterruptPriority(10));
	can1.enableInterrupts();

	cli::print_blocking("done");
	

	/* === CAN2 === */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN2 module... ");

	

	cli::print_blocking("done");


	/* === INFINITE LOOP === */
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
		bsp::ledBlue.reset();
	}
}


/**
 * @brief Fatal error callback.
 * 
 * @param hint 
 * @param code 
 */
void emb::fatal_error_cb(const char* hint, int code)
{
	cli::nextline_blocking();
	cli::print_blocking(CLI_COLOR_RED "Fatal error occurred: " CLI_COLOR_OFF);
	cli::print_blocking(hint);
	cli::nextline_blocking();
	cli::print_blocking("Terminate...");
	cli::nextline_blocking();
	bsp::ledRed.set();
}




