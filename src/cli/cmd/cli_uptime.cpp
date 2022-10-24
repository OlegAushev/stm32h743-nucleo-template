/**
 * @file
 * @ingroup cli
 */


#include "cli/shell/cli_shell.h"

#include "mcu_stm32h7/clock/mcu_clock.h"


int cli_uptime(int argc, const char** argv)
{
	if (argc == 0)
	{
		snprintf(CLI_CMD_OUTPUT, CLI_CMD_OUTPUT_LENGTH, "uptime: %lums", static_cast<uint32_t>(mcu::SystemClock::now()));
	}
	else
	{
		if (strcmp(argv[0], "-v") == 0)
		{
			uint32_t sec = static_cast<uint32_t>(mcu::SystemClock::now()) / 1000;
			uint32_t msec = static_cast<uint32_t>(mcu::SystemClock::now()) - 1000 * sec;
			uint32_t min = sec / 60;
			sec -= 60 * min;
			snprintf(CLI_CMD_OUTPUT, CLI_CMD_OUTPUT_LENGTH, "uptime: %lum %lus %lums", min, sec, msec);
		}
		else
		{
			snprintf(CLI_CMD_OUTPUT, CLI_CMD_OUTPUT_LENGTH, "uptime: invalid option - \"%s\"", argv[0]);
		}
	}

	cli::nextline();
	cli::print(CLI_CMD_OUTPUT);
	return 0;
}


