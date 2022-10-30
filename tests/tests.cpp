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


void emb::run_tests()
{
	emb::TestRunner::print = cli::print_blocking;
	emb::TestRunner::print_nextline = cli::nextline_blocking;

	emb::TestRunner::printResult();
}



