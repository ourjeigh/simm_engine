#include "memory/memory.h"
//#include <string.h>
#include <logging/logging.h>
#include <asserts.h>

void memory_zero(void* obj, size_t size)
{
	memory_set(obj, 0, size);
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
	ASSERT(dest != nullptr);
	ASSERT(src != nullptr);

	char* dst_ptr = static_cast<char*>(dest);
	const char* src_ptr = static_cast<const char*>(src);
	ASSERT(((dst_ptr + size) <= src_ptr)|| ((src_ptr + size) <= dst_ptr));

	for (size_t i = 0; i < size; ++i)
	{
		dst_ptr[i] = src_ptr[i];
	}
}

void memory_move(void* dest, void const* src, size_t size)
{
	ASSERT(dest != nullptr);
	ASSERT(src != nullptr);

	char* dst_ptr = static_cast<char*>(dest);
	const char* src_ptr = static_cast<const char*>(src);

	if (dst_ptr < src_ptr)
	{
		for (size_t i = 0; i < size; i++)
		{
			dst_ptr[i] = src_ptr[i];
		}
	}
	else if (dst_ptr > src_ptr)
	{
		for (size_t i = size; i > 0; i--)
		{
			dst_ptr[i - 1] = src_ptr[i - 1];
		}
	}
}