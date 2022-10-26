/**
 * @defgroup cli CLI
 *
 * @file
 * @ingroup cli
 */


#pragma once


#define CLI_SERVER_VERSION 0.1

#define CLI_USE_HISTORY
#define CLI_HISTORY_LENGTH 8

#define CLI_PROMPT_MAX_LENGTH 32

#define CLI_DEVICE_NAME_MAX_LENGTH 16

#define CLI_CMDLINE_MAX_LENGTH 64

#define CLI_ESCSEQ_MAX_LENGTH 16

#define CLI_OUTBUT_BUFFER_LENGTH 256

#define CLI_TOKEN_MAX_COUNT 8
#define CLI_TOKEN_MAX_LENGHT 16


#define CLI_ESC		"\x1B"
#define CLI_ENDL	"\r\n"

#define CLI_COLOR_OFF		"\033[0m"
#define CLI_COLOR_RED		"\033[1;31m"
#define CLI_COLOR_GREEN 	"\033[1;32m"
#define CLI_COLOR_YELLOW	"\033[1;33m"
#define CLI_COLOR_BLUE		"\033[1;34m"
#define CLI_COLOR_MAGENTA	"\033[1;35m"
#define CLI_COLOR_CYAN		"\033[1;36m"
#define CLI_COLOR_WHITE		"\033[1;37m"

#define CLI_PROMPT_BEGIN CLI_COLOR_MAGENTA"[root@"
#define CLI_PROMPT_END "]> " CLI_COLOR_OFF

#define CLI_WELCOME_STRING CLI_COLOR_GREEN "Welcome to uShell!" CLI_COLOR_OFF


