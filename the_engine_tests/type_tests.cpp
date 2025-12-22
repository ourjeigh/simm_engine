#include "pch.h"
#include <types/types.h>
#include <types/atomic.h>

#include <thread>

// enable this to test the std wrapped version of c_atomic
//#define USING_STD_ATOMIC


TEST(TYPES, INVALID_INT32)
{
	const int32 test = k_invalid;
	EXPECT_TRUE(test == k_invalid);
}

TEST(TYPES, INVALID_UINT32)
{
	const uint32 test = k_invalid;
	EXPECT_TRUE(test == k_invalid);
}

TEST(ATOMIC, LOAD_STORE)
{
	c_atomic<int32> atomic_test(0);

	EXPECT_EQ(atomic_test.load(), 0);

	atomic_test.store(5);

	EXPECT_EQ(atomic_test.load(), 5);
}

TEST(ATOMIC, CAS)
{
	c_atomic<int32> atomic_test(10);
	EXPECT_EQ(atomic_test.load(), 10);
	
	// Correct expected, exchange succeeds.
	int32 expected = 10;
	EXPECT_TRUE(atomic_test.compare_exchange(expected, 20));
	EXPECT_EQ(expected, 10);
	EXPECT_EQ(atomic_test.load(), 20);

	// Incorrect expected, exchange fails
	EXPECT_FALSE(atomic_test.compare_exchange(expected, 30));
	EXPECT_EQ(expected, 20);
	EXPECT_EQ(atomic_test.load(), 20);
}

const int32 k_thread_count = 40;
const int32 k_increment_count = 100000;

void concurrent_increment_counter(c_atomic<int32>* counter)
{
	for (int32 i = 0; i < k_increment_count; i++)
	{
		int32 current = counter->load();
		counter->store(current + 1);
	}
}

TEST(ATOMIC, NON_ATONIC_SANITY_CHECK)
{
	volatile int32 counter = 0;

	std::vector<std::thread> threads;

	for (int t = 0; t < k_thread_count; ++t)
	{

		threads.emplace_back([&] {
			for (int i = 0; i < k_increment_count; ++i)
			{
				counter++;
			}
			});
	}

	for (auto it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}

	const int32 expected = k_thread_count * k_increment_count;
	EXPECT_FALSE(counter == (expected));
}

TEST(ATOMIC, CONCURRENT_INCREMENT)
{
	c_atomic<int32> counter(0);
	
	std::vector<std::thread> threads;

	for (int t = 0; t < k_thread_count; ++t)
	{
		threads.emplace_back([&] {
			for (int i = 0; i < k_increment_count; ++i)
			{
				int32 old;
				do {
					old = counter.load();
				} while (!counter.compare_exchange(old, old + 1));
			}
			});
	}

	for (auto it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}

	const int32 expected = k_thread_count * k_increment_count;
	EXPECT_EQ(counter.load(), (expected));
}