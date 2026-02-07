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
	EXPECT_EQ(string_compare(file_name.get_const_char(), "test_file_text.h"), 0);
}

TEST(FILE_INFO, GET_FILE_EXT)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "h"), 0);
}

TEST(FILE_INFO, GET_FILE_NAME_NO_EXT)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);

	t_string_256 file_ext;
	file_path.get_file_name_no_ext(file_ext);
	EXPECT_EQ(string_compare(file_ext.get_const_char(), "test_file_text"), 0);
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

// if we maintain an external pointer to the read position, we expect a byte for byte match between
// the buffered and unbuffered reads as we progress thru a file.
TEST(C_FILE_BUFFERED, UNBUFFERED_COMPARISON)
{
	t_string_256 file_path_string(k_test_file_path_real);
	c_file_path file_path(file_path_string);
	t_file_open_mode_flags flags;

	c_file_buffered file;
	c_stack<byte, 200> file_buffer;
	file.set_buffer(file_buffer.make_reference());

	flags.set(file_open_mode_read, true);
	EXPECT_TRUE(file.open(file_path, flags));

	// non-divisible by 200 to make sure we force one of the reads to require a buffer refill mid-way
	c_array<byte, 105> read_buffered;
	c_array<byte, 105> read_unbuffered;
	zero_object(read_buffered);
	zero_object(read_unbuffered);

	EXPECT_EQ(read_buffered, read_unbuffered);

	int32 bytes_read_buffered = 0;
	int32 bytes_read_unbuffered = 0;
	int32 read_unbuffered_position = 0;
	do
	{
		bytes_read_buffered = file.read_bytes(read_buffered.capacity(), read_buffered.make_reference());
		bytes_read_unbuffered = file.read_bytes_unbuffered(read_unbuffered_position, read_unbuffered.capacity(), read_unbuffered.make_reference());
		read_unbuffered_position += bytes_read_unbuffered;

		EXPECT_EQ(bytes_read_buffered, bytes_read_unbuffered);
		EXPECT_EQ(read_buffered, read_unbuffered);

		zero_object(read_buffered);
		zero_object(read_unbuffered);

	} while (bytes_read_buffered == read_buffered.capacity());
}