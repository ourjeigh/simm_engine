#include "pch.h"
#include "engine/file_system/file.h"
#include "test_file_text.h"

const char* k_test_file_path_real = k_test_file_text_path;
const char* k_test_file_path_fake = "c:\\this_path\\definitely\\shouldnt.exist";
TEST(FILE_INFO, FILE_EXISTS_TRUE)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	EXPECT_TRUE(file_path.exists());
}

TEST(FILE_INFO, FILE_EXISTS_FALSE)
{
	t_string_256 file_path_string(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	EXPECT_FALSE(file_path.exists());
}

TEST(FILE_INFO, GET_FILE_NAME)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_name;
	file_path.get_file_name(file_name);
	EXPECT_EQ(string_compare(file_name.get_const_char(), "file_tests.cpp"), 0);
}

TEST(FILE_INFO, GET_FILE_EXT)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "cpp"), 0);
}

TEST(FILE_INFO, GET_FILE_NAME_NO_EXT)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_name_no_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "file_tests"), 0);
}

TEST(FILE_INFO, GET_DIRECTORY_PATH)
{
	t_string_256 file_path_string(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	t_string_256 directory_path;
	file_path.get_directory_path(directory_path);

	EXPECT_EQ(string_compare(directory_path.get_const_char(), "c:\\this_path\\definitely"), 0);
}

TEST(FILE_INFO, GET_DIRECTORY_NAME)
{
	t_string_256 file_path_string(k_test_file_path_fake);
	c_file_path file_path(file_path_string);

	t_string_256 directory_name;
	file_path.get_directory_name(directory_name);
	EXPECT_EQ(string_compare(directory_name.get_const_char(), "definitely"), 0);
}

TEST(C_FILE, OPEN_SUCESS)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);
	t_file_open_mode_flags flags;

	c_file file;
	flags.set(file_open_mode_read, true);
	bool result = file.open(file_path, flags);

	EXPECT_TRUE(result);
	EXPECT_TRUE(file.is_open());

	file.close();
	EXPECT_FALSE(file.is_open());
}

//TEST(C_FILE, READ_STRING)
//{
//	t_string_256 file_path_string(k_test_file_path_real);
//	c_file_path file_path(file_path_string);
//	t_file_open_mode_flags flags;
//
//	c_file file;
//	flags.set(file_open_mode_read, true);
//	bool result = file.open(file_path, flags);
//
//	t_string_1024 string_buffer;
//
//	file.read_string(0, 0, string_buffer.make_reference());
//
//	file.close();
//
//	string_buffer.assert_valid();
//}

TEST(C_FILE, READ_BYTES)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);
	t_file_open_mode_flags flags;

	c_file file;
	flags.set(file_open_mode_read, true);
	bool result = file.open(file_path, flags);

	c_array<byte, 2048> buffer;
	zero_object(buffer);

	file.read_bytes(0, buffer.capacity(), buffer.make_reference());

	EXPECT_TRUE(array_has_non_zero_data(buffer.make_reference()));

	file.close();
}

// fragile, should be fixed up. we just want to make sure that we can properly read thru a whole file, and that when
// we hit the end of the file, we stop returning bytes
TEST(C_FILE_BUFFERED, READ_BYTES)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);
	t_file_open_mode_flags flags;

	c_file_buffered file;
	c_stack<byte, 200> file_buffer;
	file.set_buffer(file_buffer.make_reference());

	flags.set(file_open_mode_read, true);
	EXPECT_TRUE(file.open(file_path, flags));

	c_array<byte, 100> read_buffer;
	zero_object(read_buffer);

	EXPECT_EQ(read_buffer.capacity(), file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()));
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	EXPECT_EQ(read_buffer.capacity(), file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()));
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	EXPECT_EQ(read_buffer.capacity(), file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()));
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	EXPECT_EQ(read_buffer.capacity(), file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()));
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	EXPECT_EQ(read_buffer.capacity(), file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()));
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	// we're at the EOF, we expect a partial fill, still with valid data
	EXPECT_LE(file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()), read_buffer.capacity());
	EXPECT_TRUE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

	zero_object(read_buffer);

	// we're past EOF, new reads should return 0 bytes and not fill the output
	EXPECT_EQ(file.read_bytes(read_buffer.capacity(), read_buffer.make_reference()), 0);
	EXPECT_FALSE(array_has_non_zero_data(read_buffer.make_reference()));

	{
		const char* file_string = reinterpret_cast<const char*>(read_buffer.data());
		NOP();
	}

}