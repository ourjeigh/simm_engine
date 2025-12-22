#include "pch.h"
#include <types/types.h>

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
