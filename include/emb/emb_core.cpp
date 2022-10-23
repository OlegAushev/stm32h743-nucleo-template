/**
 * @file emb_core.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "emb_core.h"


namespace emb {


__attribute__((weak)) void fatal_error_cb(const char* hint, int code)
{
	/* DO NOTHING */
}


} // namespace emb


