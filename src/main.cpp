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
#include "mcu_stm32h7/can/mcu_can.h"

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
	mcu::uart::RxPinConfig uart2RxPinConfig = {.port = CN9_USART_B_RX_PORT, .pin = CN9_USART_B_RX_PIN, .afSelection = CN9_USART_B_RX_AF};
	mcu::uart::TxPinConfig uart2TxPinConfig = {.port = CN9_USART_B_TX_PORT, .pin = CN9_USART_B_TX_PIN, .afSelection = CN9_USART_B_TX_AF};
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
	cli::print_blocking(CLI_COLOR_GREEN);
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

	/* CAN1 */
	cli::nextline_blocking();
	cli::print_blocking("configure CAN1 module... ");

	mcu::can::RxPinConfig can1RxPinConfig = {.port = CN9_CAN_RX_PORT, .pin = CN9_CAN_RX_PIN, .afSelection = CN9_CAN_RX_AF};
	mcu::can::TxPinConfig can1TxPinConfig = {.port = CN9_CAN_TX_PORT, .pin = CN9_CAN_TX_PIN, .afSelection = CN9_CAN_TX_AF};
	mcu::can::Config can1Config{.init = {
		.FrameFormat = FDCAN_FRAME_CLASSIC,
		.Mode = FDCAN_MODE_EXTERNAL_LOOPBACK,//FDCAN_MODE_NORMAL,
		.AutoRetransmission = ENABLE,
		.TransmitPause = DISABLE,
		.ProtocolException = ENABLE,
		.NominalPrescaler = 25,
		.NominalSyncJumpWidth = 1,
		.NominalTimeSeg1 = 13, /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
		.NominalTimeSeg2 = 2,
		.MessageRAMOffset = 0,
		.StdFiltersNbr = 0,
		.ExtFiltersNbr = 0,
		.RxFifo0ElmtsNbr = 1,
		.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8,
		.RxFifo1ElmtsNbr = 0,
		.RxBuffersNbr = 0,
		.TxEventsNbr = 0,
		.TxBuffersNbr = 0,
		.TxFifoQueueElmtsNbr = 1,
		.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION,
		.TxElmtSize = FDCAN_DATA_BYTES_8,				
	}};
	std::vector<FDCAN_FilterTypeDef> can1RxFilters;
	mcu::can::Can<1> can1(can1RxPinConfig, can1TxPinConfig, can1Config, can1RxFilters);

	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];
	/* Prepare Tx Header */
	TxHeader.Identifier = 0x321;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_2;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	TxData[0] = 'H';
	TxData[1] = 'i';
	
	cli::print_blocking("done");
	

	/* INFINITE LOOP */
	while (true)
	{
		mcu::SystemClock::runTasks();
		cliServer.run();


		/* Start the Transmission process */
		if (HAL_FDCAN_AddMessageToTxFifoQ(&can1.handle(), &TxHeader, TxData) != HAL_OK)
		{
			emb::fatal_error("CAN tx error");
		}
		HAL_Delay(10);


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




