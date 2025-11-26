#include "pch.h"
#include "memory/memory.h"
#include <types/types.h>

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
}