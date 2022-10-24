/**
 * @file
 * @ingroup cli
 */


#include "cli/shell/cli_shell.h"

#include "mcu_stm32h7/system/mcu_system.h"
#include "mcu_stm32h7/clock/mcu_clock.h"


int cli_reboot(int argc, const char** argv)
{
	cli::nextline();

	if (argc > 1)
	{
		cli::print("Invalid options.");
		return 0;
	}

	if (argc == 0)
	{
		mcu::SystemClock::registerDelayedTask(mcu::resetDevice, 2000);
		cli::print("Device will reboot in 2 seconds...");
		return 0;
	}

	uint32_t msec = 1000 * atol(argv[0]);
	mcu::SystemClock::registerDelayedTask(mcu::resetDevice, msec);
	snprintf(CLI_CMD_OUTPUT, CLI_CMD_OUTPUT_LENGTH, "Device will reboot in %lu seconds...", msec / 1000);
	cli::print(CLI_CMD_OUTPUT);
	return 0;
}


