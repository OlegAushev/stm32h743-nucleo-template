/**
 * @file emb_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>
#include <cstddef>
#include <cassert>


/**
 * @brief UNIQ_ID implementation.
 * 
 */
#define EMB_UNIQ_ID_IMPL(arg) a_local_var_##arg
#define EMB_UNIQ_ID(arg) EMB_UNIQ_ID_IMPL(arg)


/**
 * @brief STRINGIZE implementation.
 * 
 */
#define EMB_STRINGIZE_IMPL(x) #x
#define EMB_STRINGIZE(x) EMB_STRINGIZE_IMPL(x)


namespace emb {





} // namespace emb


