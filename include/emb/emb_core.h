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
class noncopyable
{
public:
	noncopyable() = default;
	noncopyable(const noncopyable& other) = delete;
	noncopyable& operator=(const noncopyable& other) = delete;
	virtual ~noncopyable() = default;
};


/**
 * @brief 
 * 
 * @tparam T 
 */
template <class T>
class monostate
{
private:
	static inline bool s_initialized {false};
protected:
	monostate()
	{
		assert(s_initialized);
		if (!s_initialized) { fatal_error("uninitialized monostate class"); }
	}

	static void set_initialized()
	{
		assert(!s_initialized);
		if (s_initialized) { fatal_error("repeated initialization of monostate class"); }
		s_initialized = true;
	}
public:
	static bool initialized() { return s_initialized; }
};


/**
 * @brief 
 * 
 * @tparam T 
 */
template <class T>
class irq_singleton
{
private:
	static T* s_instance;
	static bool s_initialized;
protected:
	irq_singleton(T* self)
	{
		assert(!s_initialized);
		if (s_initialized) while (1) {}
		s_instance = self;
		s_initialized = true;
	}
public:
	static T* instance()
	{
		assert(s_initialized);
		if (!s_initialized) while (1) {}
		return s_instance;
	}

	static bool initialized() { return s_initialized; }

	virtual ~irq_singleton()
	{
		s_initialized = false;
		s_instance = static_cast<T*>(NULL);
	}
};

template <class T>
T* irq_singleton<T>::s_instance = static_cast<T*>(NULL);
template <class T>
bool irq_singleton<T>::s_initialized = false;


} // namespace emb


