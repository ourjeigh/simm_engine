#ifndef __MEMORY_H__
#define __MEMORY_H__
#pragma once

#include <types/types.h>

class c_handle
{
public:
	c_handle() { invalidate(); }
	~c_handle() = default;

	c_handle(c_handle&& other) {};
	c_handle& operator=(c_handle&& other) {}

	bool is_valid();
	void invalidate() { m_pointer = k_invalid; };

	void* get_pointer();
	const void* get_pointer_const();

private:
	// may not need this
	friend class i_memory_allocator;

	// no copying
	c_handle(const c_handle& other) = delete;
	c_handle& operator=(const c_handle& other) = delete;

	uint64 m_pointer;
};

class i_memory_allocator
{
public:
	virtual c_handle allocate(uint32 size) = 0;
	virtual void free(c_handle handle) = 0;

private:
	i_memory_allocator() = delete;
	~i_memory_allocator() = delete;
};


#define zero_object(obj) memory_zero(&obj, sizeof(obj))

void memory_zero(void* obj, size_t size);

void memory_set(void* dest, int value, size_t size);

// non-overlapping
void memory_copy(void* dest, void const* src, size_t size);

// overlapping allowed
void memory_move(void* dest, void const* src, size_t size);

int32 memory_compare(const void* left, const void* right, size_t size);

#endif//__MEMORY_H__