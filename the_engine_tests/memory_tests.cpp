#include "pch.h"
#include "memory/memory.h"
#include <types/types.h>

TEST(MEMORY, MEM_SET)
{
	const int32 size = 10;
	uint32 test_array[size];
	memory_set(test_array, k_byte_max, sizeof(uint32) * size);

	for (int32 i = 0; i < size; i++)
	{
		EXPECT_EQ(test_array[i], k_uint32_max);
	}
}

TEST(MEMORY, MEMZERO)
{
	int32 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	memory_zero(data, sizeof(int32) * 10);

	for (int32 i = 0; i < 10; i++)
	{
		EXPECT_EQ(data[i], 0);
	}
}

TEST(MEMORY, CLEAR_STRUCT)
{
	struct s_test_struct
	{
		int32 a;
		real32 b;
	};

	s_test_struct test_struct;
	test_struct.a = 42;
	test_struct.b = 3.14f;
	
	zero_object(test_struct);
	
	EXPECT_EQ(test_struct.a, 0);
	EXPECT_EQ(test_struct.b, 0.0f);
}

TEST(MEMORY, MEMCOPY)
{
	int32 source[] = { 1,3,5,7,9 };
	int32 dest[] = { 0,0,0,0,0 };
	memory_copy(dest, source, sizeof(int32) * 5);

	for (int32 i = 0; i < 5; i++)
	{
		EXPECT_EQ(dest[i], source[i]);
	}
}

TEST(MEMORY, MEMCOPY_OVERLAPPING)
{
	int32 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	EXPECT_DEATH(memory_copy(&data[2], &data[0], sizeof(int32) * 8), ".*");

	EXPECT_DEATH(memory_copy(&data[0], &data[4], sizeof(int32) * 5), ".*");
}

TEST(MEMORY, MEMCOPY_NONOVERLAPPING)
{
	int32 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int32 expected[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };

	memory_copy(&data[5], &data[0], sizeof(int32) * 5);

	for (int32 i = 0; i < 10; i++)
	{
		EXPECT_EQ(data[i], expected[i]);
	}
}

TEST(MEMORY, MEMMOVE)
{
	int32 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int32 expected[] = { 1, 2, 1, 2, 3, 4, 5, 6, 7, 8 };

	memory_move(&data[2], &data[0], sizeof(int32) * 8);

	for (int32 i = 0; i < 10; i++)
	{
		EXPECT_EQ(data[i], expected[i]);
	}
}