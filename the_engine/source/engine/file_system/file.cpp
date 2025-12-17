#include "file.h"
#include "windows.h"
#include <minwinbase.h>

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

	out_file_name.copy_from(m_data, file_name_index, m_data.used());
}

void c_file_path::get_file_ext(t_string_256& out_file_ext) const
{
	out_file_ext.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);
	
	out_file_ext.copy_from(m_data, ext_index, m_data.used());
}

void c_file_path::get_file_name_no_ext(t_string_256& out_file_name) const
{
	out_file_name.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_file_name.copy_from(m_data, file_name_index, ext_index - 1);
}

void c_file_path::get_directory_path(t_string_256& out_directory_path) const
{
	out_directory_path.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_directory_path.copy_from(m_data, 0, file_name_index - 1);
}

void c_file_path::get_directory_name(t_string_256& out_directory_name) const
{
	out_directory_name.clear();

	uint8 directory_name_index;
	uint8 file_name_index;
	uint8 ext_index;
	split_path(directory_name_index, file_name_index, ext_index);

	out_directory_name.copy_from(m_data, directory_name_index, file_name_index - 1);
}

void c_file_path::split_path(uint8& out_directory_name_index, uint8& out_filename_index, uint8& out_ext_index) const
{
	out_directory_name_index = k_invalid;
	out_filename_index = k_invalid;
	out_ext_index = k_invalid;

	for (int16 i = m_data.used() - 1; i >= 0; i--)
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
		m_file_handle = file_handle;
		m_flags = flags;
	}

	return result;
}

bool c_file::close()
{
	bool result = false;
	if (m_file_handle != nullptr)
	{
		CloseHandle(m_file_handle);
		m_file_handle = nullptr;
		result = true;
	}

	return result;
}

bool c_file::read()
{
	ASSERT(is_open());
	ASSERT(m_flags.test(file_open_mode_read));

	bool result = false;

	byte temp[1024];
	DWORD        nNumberOfBytesToRead = 1024;
	LPDWORD      lpNumberOfBytesRead = 0;
	LPOVERLAPPED lpOverlapped = nullptr;

	result = ReadFile(
		m_file_handle,
		temp,
		nNumberOfBytesToRead,
		lpNumberOfBytesRead,
		lpOverlapped
	);

	return result;
}


char get_path_separator()
{
	return '\\';
}

char get_ext_separator()
{
	return '.';
}

