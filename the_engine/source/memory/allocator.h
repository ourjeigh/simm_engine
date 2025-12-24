#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
#pragma once

#include <types/types.h>

typedef uint64 t_stack_allocator_marker;
class c_stack_allocator
{
public:
	c_stack_allocator(void* memory, uint64 size);

	void* allocate(uint64 size, uint64 align);
	void* allocate(uint64 size, uint64 align, t_stack_allocator_marker& out_marker);
	bool is_marker_valid(t_stack_allocator_marker marker) const;
	void free_to_marker(t_stack_allocator_marker marker);
	void clear();
private:
	uint64 m_top;
	uint64 m_base;
	uint64 m_end;
};

template<uint64 k_size>
class c_static_stack_allocator : public c_stack_allocator
{
public:
	c_static_stack_allocator() : c_stack_allocator(m_data, k_size) {}

private:
	byte m_data[k_size];
};
#endif //__ALLOCATOR_H__
