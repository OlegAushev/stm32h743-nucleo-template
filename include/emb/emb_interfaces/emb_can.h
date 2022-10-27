/**
 * @file emb_can.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "../emb_def.h"
#include <array>


struct can_frame
{
	unsigned int id;
	unsigned int len;
	std::array<uint8_t, 8> data;
};


