#include "pch.h"
#include "engine/file_system/file.h"

const char* k_test_file_path_real = __FILE__;
const char* k_test_file_path_fake = "c:\\this_path\\definitely\\shouldnt.exist";
TEST(FILE_TEST, FILE_EXISTS_TRUE)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	EXPECT_TRUE(file_path.exists());
}

TEST(FILE_TEST, FILE_EXISTS_FALSE)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	EXPECT_FALSE(file_path.exists());
}

TEST(FILE_TEST, FILE_INFO_GET_FILE_NAME)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_name;
	file_path.get_file_name(file_name);
	EXPECT_EQ(string_compare(file_name.get_const_char(), "file_tests.cpp"), 0);
}

TEST(FILE_TEST, FILE_INFO_GET_FILE_EXT)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "cpp"), 0);
}

TEST(FILE_TEST, FILE_INFO_GET_FILE_NAME_NO_EXT)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_name_no_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "file_tests"), 0);
}

TEST(FILE_TEST, FILE_INFO_GET_DIRECTORY_PATH)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	t_string_256 directory_path;
	file_path.get_directory_path(directory_path);

	EXPECT_EQ(string_compare(directory_path.get_const_char(), "c:\\this_path\\definitely"), 0);
}

TEST(FILE_TEST, FILE_INFO_GET_DIRECTORY_NAME)
{
	t_string_256 file_path_string;
	file_path_string.printf(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	t_string_256 directory_name;
	file_path.get_directory_name(directory_name);
	EXPECT_EQ(string_compare(directory_name.get_const_char(), "definitely"), 0);
}