#include "file.h"
#include "windows.h"
#include <minwinbase.h>

c_file_path::c_file_path(const t_string_256 path)
{
	m_data = path;
	parse();
}

c_file_path::c_file_path(const c_file_path& other)
{
	m_data = other.m_data;
	m_file_name_index = other.m_file_name_index;
	m_file_ext_index = other.m_file_ext_index;
}

bool c_file_path::exists()
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

void c_file_path::get_file_name(t_string_256& out_file_name)
{
	out_file_name.clear();
	out_file_name.copy_from(m_data, m_file_name_index, m_data.used());
}

void c_file_path::get_file_ext(t_string_256& out_file_ext)
{
	out_file_ext.clear();
	out_file_ext.copy_from(m_data, m_file_ext_index, m_data.used());
}

void c_file_path::get_file_name_no_ext(t_string_256& out_file_name)
{
	out_file_name.clear();
	out_file_name.copy_from(m_data, m_file_name_index, m_file_ext_index - 1);
}

void c_file_path::get_directory_path(t_string_256& out_directory_path)
{
	out_directory_path.clear();
	out_directory_path.copy_from(m_data, 0, m_file_name_index - 1);
}

void c_file_path::get_directory_name(t_string_256& out_directory_name)
{
	out_directory_name.clear();

	for (int32 i = m_file_name_index - 2; i >= 0; i--)
	{
		if (m_data[i] == get_path_separator())
		{
			out_directory_name.copy_from(m_data, i + 1, m_file_name_index - 1);
			return;
		}
	}

	HALT("didn't find directory name");
}

void c_file_path::parse()
{
	for (uint8 i = 0; i < m_data.used(); i++)
	{
		char ch = m_data[i];

		if (ch == get_path_separator())
		{
			// the last one will be the direct parent index
			m_file_name_index = i + 1;
		}
		else if (ch == get_ext_separator())
		{
			m_file_ext_index = i + 1;
		}
	}
}

bool c_file::open(c_file_path file_path, t_file_open_mode_flags flags)
{
	ASSERT(m_os_file_handle == nullptr);

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

	// TODO handle read and write sharing separately (maybe delete??)
	DWORD share_mode = flags.test(file_open_mode_exclusive) ? 0 : FILE_SHARE_READ | FILE_SHARE_WRITE;

	LPSECURITY_ATTRIBUTES security = nullptr;

	DWORD creation_disposition = OPEN_ALWAYS;

	file_handle = CreateFileA(
		file_path.get_full_path(), // Filename
		access,    // Desired access
		share_mode, // Share flags
		security,             // Security Attributes
		OPEN_ALWAYS,      // Creation Disposition
		0,                // Flags and Attributes
		NULL);           // OVERLAPPED pointer
	
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		result = true;
		m_os_file_handle = file_handle;
	}

	return false;
}

bool c_file::close()
{
	bool result = false;
	if (m_os_file_handle != nullptr)
	{
		CloseHandle(reinterpret_cast<HANDLE>(m_os_file_handle));
		result = true;
	}

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

