/**
 * @file emb_algorithm_test.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "emb_test.h"
#include <algorithm>
#include <array>
#include <limits>


struct StructTest
{
	int key;
	int value;

	StructTest() {}
	StructTest(int k, int v) : key(k), value(v) {}
};

bool operator==(const StructTest& lhs, const StructTest& rhs)
{
	return (lhs.key == rhs.key) && (lhs.value == rhs.value);
}

bool operator<(const StructTest& lhs, const StructTest& rhs)
{
	return (lhs.key < rhs.key) || ((lhs.key == rhs.key) && (lhs.value < rhs.value));
}


void EmbTest::AlgorithmTest()
{
	// find
	int arr1[10] = {3, 6, 2, 7, 8, 9, 0, 1, 5, 4};
	std::array<int, 5> arr2 = {-5, 1, 4, 4, 7};
	std::array<StructTest, 5> arr3 = {StructTest(1, 2), StructTest(5, 0),
			StructTest(-4, 7), StructTest(8, 1), StructTest(5, -4)};


	// binary_find
	std::sort(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]));
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 0), arr1);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 1), arr1 + 1);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 2), arr1 + 2);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 3), arr1 + 3);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 4), arr1 + 4);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 5), arr1 + 5);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 6), arr1 + 6);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 7), arr1 + 7);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 8), arr1 + 8);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 9), arr1 + 9);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), -1), arr1 + 10);
	EMB_ASSERT_EQUAL(emb::binary_find(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]), 10), arr1 + 10);

	std::sort(arr2.begin(), arr2.begin());
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), -5), arr2.begin());
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), 1), arr2.begin() + 1);
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), 4), arr2.begin() + 2);
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), 7), arr2.begin() + 4);
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), -7), arr2.end());
	EMB_ASSERT_EQUAL(emb::binary_find(arr2.begin(), arr2.end(), 8), arr2.end());

	std::sort(arr3.begin(), arr3.end());
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(-4, 7)), arr3.begin());
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(1, 2)), arr3.begin() + 1);
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(5, -4)), arr3.begin() + 2);
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(5, 0)), arr3.begin() + 3);
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(8, 1)), arr3.begin() + 4);
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(-4, 6)), arr3.end());
	EMB_ASSERT_EQUAL(emb::binary_find(arr3.begin(), arr3.end(), StructTest(8, 0)), arr3.end());

	std::array<int, 9> arr4;
	std::copy(arr1, arr1+9, arr4.data());
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 0), arr4.begin());
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 1), arr4.begin() + 1);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 2), arr4.begin() + 2);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 3), arr4.begin() + 3);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 4), arr4.begin() + 4);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 5), arr4.begin() + 5);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 6), arr4.begin() + 6);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 7), arr4.begin() + 7);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 8), arr4.begin() + 8);
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), 9), arr4.end());
	EMB_ASSERT_EQUAL(emb::binary_find(arr4.begin(), arr4.end(), -1), arr4.end());
}

