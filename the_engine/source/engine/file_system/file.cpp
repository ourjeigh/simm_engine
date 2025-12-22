#include "file.h"
#include "platform/platform.h"

IGNORE_WINDOWS_WARNINGS_PUSH
#include "windows.h"
#include <minwinbase.h>
#include <platform/platform_handle_windows.h>
IGNORE_WINDOWS_WARNINGS_POP

template<class t_type>
int32 read_file_internal(
	c_platform_handle& file_handle,
	t_file_open_mode_flags flags,
	int32 start,
	int32 length,
	c_array_reference<t_type> out_buffer);

c_file_path::c_file_path(const t_string_256 path)
{
	m_data = path;
}

c_file_path::c_file_path(const c_file_path& other)
{
	m_data = other.m_data;
}

bool c_file_path::exists() const
{
	bool result = false;

	WIN32_FIND_DATA file_data;
	HANDLE find_handle;

	find_handle = FindFirstFileA(m_data.get_const_char(), &file_data);
	if (find_handle != INVALID_HANDLE_VALUE)
	{
		result = true;
		//ASSERT(string_compare(file_data.cFileName, m_data.get_const_char()) == 0);
		FindClose(find_handle);
	}

	return result;
}

void c_file_path::get_file_name(t_string_256& out_file_name) const
{
	out_file_name.clear();
	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_file_name.copy_from_range(m_data, file_name_index, m_data.used());
}

void c_file_path::get_file_ext(t_string_256& out_file_ext) const
{
	out_file_ext.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);
	
	out_file_ext.copy_from_range(m_data, ext_index, m_data.used());
}

void c_file_path::get_file_name_no_ext(t_string_256& out_file_name) const
{
	out_file_name.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_file_name.copy_from_range(m_data, file_name_index, ext_index - 1);
}

void c_file_path::get_directory_path(t_string_256& out_directory_path) const
{
	out_directory_path.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_directory_path.copy_from_range(m_data, 0, file_name_index - 1);
}

void c_file_path::get_directory_name(t_string_256& out_directory_name) const
{
	out_directory_name.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_directory_name.copy_from_range(m_data, directory_name_index, file_name_index - 1);
}

void c_file_path::split_path(uint8& out_directory_name_index, uint8& out_filename_index, uint8& out_ext_index) const
{
	out_directory_name_index = k_invalid;
	out_filename_index = k_invalid;
	out_ext_index = k_invalid;

	for (int8 i = int32_to_uint8(m_data.used()) - 1; i >= 0; i--)
	{
		char ch = m_data[i];
		if (out_ext_index == k_invalid && ch == get_ext_separator())
		{
			out_ext_index = i + 1;
		}
		else if (out_filename_index == k_invalid && ch == get_path_separator())
		{
			out_filename_index = i + 1;
		}
		else if (out_directory_name_index == k_invalid &&
			out_filename_index != k_invalid &&
			ch == get_path_separator())
		{
			out_directory_name_index = i + 1;
			break;
		}
	}

	ASSERT(out_directory_name_index != k_invalid);
	ASSERT(out_filename_index != k_invalid);
	ASSERT(out_ext_index != k_invalid);
}

bool c_file::open(const c_file_path& file_path, t_file_open_mode_flags flags)
{
	ASSERT(!is_open());

	bool result = false;

	HANDLE file_handle = nullptr;

	DWORD access = 0;
	
	if (flags.test(file_open_mode_read))
	{
		access |= GENERIC_READ;
	}

	if (flags.test(file_open_mode_write))
	{
		access |= GENERIC_WRITE;
	}

	// TODO handle read and write sharing separately (maybe delete also??)
	DWORD share_mode = flags.test(file_open_mode_exclusive) ? 0 : FILE_SHARE_READ | FILE_SHARE_WRITE;
	LPSECURITY_ATTRIBUTES security = nullptr;
	DWORD creation_disposition = OPEN_ALWAYS;
	DWORD attributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE template_file = nullptr;

	file_handle = CreateFileA(
		file_path.get_full_path(),
		access,
		share_mode,
		security,
		creation_disposition,
		attributes,
		template_file);
	
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		result = true;
		m_file_handle = c_platform_handle_factory::get_platform_handle_from_native_handle(file_handle);
		m_flags = flags;
	}

	return result;
}

bool c_file::close()
{
	bool result = false;
	if (m_file_handle.is_valid())
	{
		CloseHandle(c_platform_handle_factory::get_native_handle_from_platform_handle(m_file_handle));
		m_file_handle.invalidate();
		result = true;
	}

	return result;
}

// this doesn't null terminate the string or set the stack's top

//uint32 c_file::read_string(int32 start, int32 length, c_array_reference<char>& out_buffer)
//{
//	ASSERT(is_open());
//	if (length == out_buffer.capacity())
//	{
//		length--;
//	}
//
//
//	int32 read_count = read_file_internal(
//		m_file_handle,
//		m_flags,
//		start,
//		length,
//		out_buffer);
//
//
//
//	return read_count;
//}

uint32 c_file::read_bytes(int32 start, int32 length, c_array_reference<byte> out_buffer)
{
	ASSERT(is_open());
	return read_file_internal(
		m_file_handle,
		m_flags,
		start,
		length,
		out_buffer);
}

char get_path_separator()
{
	return '\\';
}

char get_ext_separator()
{
	return '.';
}

// private

template<class t_type>
int32 read_file_internal(
	c_platform_handle& file_handle,
	t_file_open_mode_flags flags,
	int32 start,
	int32 length,
	c_array_reference<t_type> out_buffer)
{
	if (length == 0)
	{
		length = out_buffer.capacity();
	}

	//ASSERT(start + length <= out_buffer.capacity());
	//ASSERT(flags.test(file_open_mode_read));

	//SetFilePointer(file_handle, start, nullptr, FILE_BEGIN);

	uint32 bytes_read = 0;
	OVERLAPPED overlapped;
	zero_object(overlapped);
	overlapped.Offset = start;

	//LPOVERLAPPED overlapped_ptr = nullptr;
	LPOVERLAPPED overlapped_ptr = &overlapped;

	bool result = ReadFile(
		c_platform_handle_factory::get_native_handle_from_platform_handle(file_handle),
		out_buffer.data(),
		length,
		&bytes_read,
		overlapped_ptr);

	// how could we succeed but read no bytes?
	//ASSERT(!result || bytes_read > 0);

	return bytes_read;
}