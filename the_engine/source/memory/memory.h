#ifndef __MEMORY_H__
#define __MEMORY_H__
#pragma once

#define zero_object(obj) memory_zero(&obj, sizeof(obj))

void memory_zero(void* obj, size_t size);

void memory_set(void* dest, int value, size_t size);

// non-overlapping
void memory_copy(void* dest, void const* src, size_t size);

// overlapping allowed
void memory_move(void* dest, void const* src, size_t size);

#endif//__MEMORY_H__