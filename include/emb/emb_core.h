/**
 * @file emb_core.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <functional>

#include "emb_def.h"


namespace emb {


inline void empty_function()
{
	/* DO NOTHING */
}


[[ noreturn ]] inline void invalid_function()
{
	while (true) {}
}


void fatal_error_cb(const char* hint, int code);


[[ noreturn ]] inline void fatal_error(const char* hint, int code = 0)
{	
	fatal_error_cb(hint, code);
	while (true) {}
}


/**
 * @brief 
 * 
 */
class NonCopyable
{
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable& other) = delete;
	NonCopyable& operator=(const NonCopyable& other) = delete;
	virtual ~NonCopyable() = default;
};


/**
 * @brief 
 * 
 * @tparam T 
 */
template <class T>
class Monostate
{
private:
	static inline bool s_initialized {false};
protected:
	Monostate()
	{
		assert(s_initialized);
		if (!s_initialized) { fatal_error("uninitialized monostate class"); }
	}

	static void setInitialized()
	{
		assert(!s_initialized);
		if (s_initialized) { fatal_error("repeated initialization of monostate class"); }
		s_initialized = true;
	}
public:
	static bool initialized() { return s_initialized; }
};


} // namespace emb


