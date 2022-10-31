/**
 * @file tests.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "tests.h"

#include "cli/cli_server.h"

#include "emb/tests/emb_test.h"
#include "mcu_stm32h7_test/mcu_stm32h7_test.h"


void emb::run_tests()
{
	emb::TestRunner::print = cli::print_blocking;
	emb::TestRunner::print_nextline = cli::nextline_blocking;

	EMB_RUN_TEST(EmbTest::MathTest);
	EMB_RUN_TEST(EmbTest::AlgorithmTest);

	EMB_RUN_TEST(McuTest::GpioTest);
	EMB_RUN_TEST(McuTest::ClockTest);

	emb::TestRunner::printResult();
}



