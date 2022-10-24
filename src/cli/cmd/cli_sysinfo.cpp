/**
 * @file
 * @ingroup cli
 */


#include "cli/shell/cli_shell.h"

// TODO #include "sys/sysinfo/sysinfo.h"


int cli_sysinfo(int argc, const char** argv)
{
	cli::nextline();
	cli::print("device name: ");
	// TODO cli::print(sys::DEVICE_NAME);
	cli::nextline();
	cli::print("firmware version: ");
	// TODO cli::print(sys::FIRMWARE_VERSION);
	cli::nextline();
	cli::print("build configuration: ");
	// TODO cli::print(sys::BUILD_CONFIGURATION);
	return 0;
}


