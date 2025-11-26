#include "memory/memory.h"
//#include <string.h>
#include <logging/logging.h>

void memory_zero(void* obj, size_t size)
{
	memory_set(obj, 0, size);
	/*for (int i = 0; i < size; i++)
	{
		static_cast<char*>(obj)[i] = 0;
	}*/
}

void memory_set(void* dest, int value, size_t size)
{
	char* dst_ptr = static_cast<char*>(dest);

	for (size_t i = 0; i < size; i++)
	{
		dst_ptr[i] = static_cast<char>(value);
	}
}

void memory_copy(void* dest, void const* src, size_t size)
{
	char* dst_ptr = static_cast<char*>(dest);
	const char* src_ptr = static_cast<const char*>(src);

	for (size_t i = 0; i < size; ++i)
	{
		dst_ptr[i] = src_ptr[i];
	}
}

void memory_move(void* dest, void const* src, size_t size)
{
	//memmove(dest, src, size);
}