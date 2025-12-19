#ifndef __FILE_H__
#define __FILE_H__
#pragma once

#include "structures/string.h"
#include "structures/array.h"

const uint32 k_file_path_max = 256;

class c_file_path
{
public:
	c_file_path() { m_data.clear(); }
	c_file_path(const t_string_256 path);
	c_file_path(const c_file_path& other);
	~c_file_path() {}

	bool exists() const;
	const char* get_full_path() const { return m_data.get_const_char(); }
	void get_file_name(t_string_256& out_file_name) const;
	void get_file_ext(t_string_256& out_file_ext) const;
	void get_file_name_no_ext(t_string_256& out_file_name) const;
	void get_directory_path(t_string_256& out_directory_path) const;
	void get_directory_name(t_string_256& out_directory_name) const;
private:
	void split_path(uint8& out_parent_directory_index, uint8& out_filename_index, uint8& out_ext_index) const;

	t_string_256 m_data;
};

enum e_file_open_mode
{
	file_open_mode_read,
	file_open_mode_write,
	file_open_mode_append,
	file_open_mode_replace,
	file_open_mode_exclusive,

	k_file_open_mode_count
};
typedef c_flags<k_file_open_mode_count> t_file_open_mode_flags;

class c_file
{
public:
	c_file() : m_file_handle(nullptr) { }

	bool open(const c_file_path& file_path, t_file_open_mode_flags flags);
	bool close();

	bool is_open() const { return m_file_handle != nullptr; }

	//uint32 read_string(int32 start, int32 length, c_array_reference<char>& out_buffer);
	uint32 read_bytes(int32 start, int32 length, c_array_reference<byte>& out_buffer);

protected:
	c_file_path m_path;
	t_file_open_mode_flags m_flags;
	void* m_file_handle;
};

char get_path_separator();
char get_ext_separator();

#endif // __FILE_H__
