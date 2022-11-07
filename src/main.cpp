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
#include "mcu_stm32h7/dma/mcu_dma.h"
#include "mcu_stm32h7/adc/mcu_adc.h"
#include "mcu_stm32h7/crc/mcu_crc.h"

#include "bsp_h743_nucleo/bsp_h743_nucleo_def.h"
#include "bsp_h743_nucleo/leds/leds.h"
#include "bsp_h743_nucleo/button/button.h"

#include "sys/sysconfig/sysconfig.h"
#include "sys/sysinfo/sysinfo.h"
#include "settings/settings.h"
#include "cli/cli_server.h"
#include "cli/shell/cli_shell.h"

#include "build/generated/git_version.h"
#include "tests/tests.h"



mcu::dma::Buffer<uint16_t, 16> adc3DmaBuffer;
mcu::SystemClock::TaskStatus taskLedHeartbeat();
mcu::SystemClock::TaskStatus taskAcqMcuSysInfo();




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


	/* === SETTINGS === */
	Settings::init();
	Settings settings;	// is not used yet


	/* === UART === */
	mcu::uart::Module<mcu::uart::Peripheral::USART_2> uart2(
			sysconfig::uart2::rxPinConfig,
			sysconfig::uart2::txPinConfig,
			sysconfig::uart2::config);


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

	cli::print_blocking("done");


	/* === TESTS RUNNING === */
#ifdef ON_TARGET_TEST_BUILD
	cli::nextline_blocking();
	cli::print_blocking(CLI_COLOR_YELLOW "on-target testing build configuration is selected, run tests..." CLI_COLOR_OFF);
	cli::nextline_blocking();
	emb::run_tests();
#endif


	/* === CAN1 === */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN1 module... ");
	mcu::can::Module<mcu::can::Peripheral::FDCAN_1> can1(
			sysconfig::can1::rxPinConfig,
			sysconfig::can1::txPinConfig,
			sysconfig::can1::config,
			sysconfig::can1::rxFilters);
	
	
	/* === CAN2 === */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN2 module... ");

	std::vector<FDCAN_FilterTypeDef> can2RxFilters;
	mcu::can::Module<mcu::can::Peripheral::FDCAN_2> can2(
			sysconfig::can2::rxPinConfig,
			sysconfig::can2::txPinConfig,
			sysconfig::can2::config,
			sysconfig::can2::rxFilters);

	cli::print_blocking("done");


	/* === CAN1 to CAN2 LOOPBACK === */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN1 to CAN2 loopback... ");

	auto canLoop = [&can1, &can2](can_frame frame)
	{
		can1.send({.id = 0x701, .len = 1, .data = {0x42}});
		can2.send(frame);
		bsp::ledBlue.set();
		mcu::SystemClock::registerDelayedTask([](){ bsp::ledBlue.reset(); }, 100);
	};
	can1.onFifo0FrameReceived = canLoop;
	//can1.onFifo1WatermarkReached = ;
	can1.initInterrupts(FDCAN_IT_RX_FIFO0_NEW_MESSAGE, FDCAN_INTERRUPT_LINE0);
	can1.initInterrupts(FDCAN_IT_RX_FIFO1_WATERMARK, FDCAN_INTERRUPT_LINE1);
	can1.setFifoWatermark(FDCAN_CFG_RX_FIFO1, 4);
	can1.setInterruptPriority(mcu::InterruptPriority(2), mcu::InterruptPriority(15));
	
	cli::print_blocking("done");


	/* === DMA === */
	cli::nextline_blocking();
	cli::print_blocking("enable DMA... ");

	// TODO
	
	cli::print_blocking("done");


	/* === ADC === */
	cli::nextline_blocking();
	cli::print_blocking("configure ADC modules and channels... ");

	mcu::adc::Module<mcu::adc::Peripheral::ADC_3> adc3(sysconfig::adc3::config);
	adc3.addInternalChannel(sysconfig::adc3::channels::internalTemp);
	//adc3.addInternalChannel(settings.adcChannels.internalVrefChannelConfig);
	adc3.calibrate();
	//adc3.startRegularConversion();
	adc3.startRegularConversionDma(adc3DmaBuffer);

	cli::print_blocking("done");


	/* === CRC === */
	cli::nextline_blocking();
	cli::print_blocking("configure CRC module... ");

	//mcu::crc::Module::instance().init({});

	cli::print_blocking("done");


	/* === CLOCK TASKS === */
	cli::nextline_blocking();
	cli::print_blocking("configure system clock periodic tasks... ");

	mcu::SystemClock::registerTask(0, taskLedHeartbeat);
	mcu::SystemClock::setTaskPeriod(0, 2000);

	mcu::SystemClock::registerTask(1, taskAcqMcuSysInfo);
	mcu::SystemClock::setTaskPeriod(1, 1000);

	cli::print_blocking("done");


	/* === START CAN1 and CAN2 === */
	cli::nextline_blocking();
	cli::print_blocking("starting CAN1 and CAN2... ");

	can1.start();
	can2.start();
	can1.enableInterrupts();

	cli::print_blocking("success");


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



mcu::SystemClock::TaskStatus taskLedHeartbeat()
{
	const uint64_t periods[4] = {100, 100, 100, 700};
	static size_t index = 0;

	mcu::SystemClock::setTaskPeriod(0, periods[index]);
	if ((index % 2) == 0) { bsp::ledGreen.set(); }
	else { bsp::ledGreen.reset(); }
	index = (index + 1) % 4;
	return mcu::SystemClock::TaskStatus::SUCCESS;
};


mcu::SystemClock::TaskStatus taskAcqMcuSysInfo()
{
	using namespace mcu::adc; 

	if (Module<Peripheral::ADC_3>::instance().pollForConversion() == mcu::HalStatus::HAL_OK)
	{
		Sysinfo::saveMcuTemperature(mcu::calculateMcuTemperature<ADC_RESOLUTION_16B>(Module<Peripheral::ADC_3>::instance().readRegularConversion()));
		Module<Peripheral::ADC_3>::instance().startRegularConversion();
	}
	return mcu::SystemClock::TaskStatus::SUCCESS;
}





