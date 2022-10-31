/**
 * @file emb_algorithm.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <utility>

#include "emb_def.h"


namespace emb {


/**
 * @brief Returns pointer to the element in range [first; last) that is equal to value.
 * Range must be sorted.
 */
template <class It, class T>
constexpr inline It binary_find(It first, It last, const T& value)
{
	It notFound = last;

	while (first < last)
	{
		It mid = first + (last - first) / 2;

		if (value == *mid)
		{
			return mid;
		}

		if (value < *mid)
		{
			last = mid;
		}
		else
		{
			first = mid + 1;
		}
	}

	return notFound;
}


} // namespace emb


