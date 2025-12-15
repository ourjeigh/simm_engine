#include "pch.h"
#include "types/types.h"
#include <structures/string.h>

TEST(STRING_TEST, STRING_TEST_PRINT)
{
	const char* test_string = "hey this is a test string";
	t_string_128 string;
	string.print(test_string);

	EXPECT_EQ(string_compare(test_string, string.get_const_char()), 0);
}

TEST(STRING_TEST, STRING_TEST_PRINTF_APPEND)
{
	const int32 k_max_size = 64;
	c_string<k_max_size> test_string;
	test_string.printf("Hello %s!", "World");

	// "Hello World!\0" = 13
	int32 used = test_string.used();
	EXPECT_EQ(used, 13);
	EXPECT_TRUE(test_string.top() == '\0');
	EXPECT_STREQ("Hello World!", test_string.get_const_char());
	test_string.append(" The answer is %d.", 42);
	EXPECT_STREQ("Hello World! The answer is 42.", test_string.get_const_char());
}

TEST(STRING_TEST, STRING_TEST_ASSERTS)
{
	const int32 k_max_size = 16;
	c_string<k_max_size> test_string;

	// should fit
	test_string.printf("Hello %s!", "Bob");

	// should not fit
	EXPECT_DEATH(test_string.printf("This string is way too long %d", 12345), ".*");
}

TEST(STRING_TEST, STRING_COMPARE_EQUALS)
{
	const char* test1 = "This is a test string";
	const char* test2 = "This is a test string";

	EXPECT_EQ(string_compare(test1, test2), 0);
}

TEST(STRING_TEST, STRING_COMPARE_UNEQUAL)
{
	const char* test1 = "This is test string 1";
	const char* test2 = "This is test string 2";

	EXPECT_EQ(string_compare(test1, test2), 1);
}

TEST(STRING_TEST, STRING_COMPARE_SHORTER)
{
	const char* test1 = "This is test string 1";
	const char* test2 = "This is test";

	EXPECT_EQ(string_compare(test1, test2), -1);
}

TEST(STRING_TEST, STRING_COMPARE_LONGER)
{
	const char* test1 = "This is test string 1";
	const char* test2 = "This is test string 11";

	EXPECT_EQ(string_compare(test1, test2), 1);
}

TEST(STRING_TEST, STRING_COMPARE_NULL)
{
	const char* test1 = "This is test string 1";
	const char* test2 = "This is test string 11";

	EXPECT_EQ(string_compare(nullptr, nullptr), 0);
	EXPECT_GE(string_compare(nullptr, test1), 1);
}