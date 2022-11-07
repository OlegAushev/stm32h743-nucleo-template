/**
 * @file settings.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "emb/emb_core.h"


class Settings : public emb::monostate<Settings>
{
public:
	static void init()
	{
		set_initialized();
	}
};


