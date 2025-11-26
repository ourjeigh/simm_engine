#ifndef __ARRAY_H__
#define __ARRAY_H__
#pragma once

#include "types/types.h"
#include "asserts.h"
#include "memory/memory.h"

template<class t_type, int32 k_max_size>
class c_array
{
public:
	struct iterator
	{
		iterator(t_type* ptr) : m_ptr(ptr) {}
		t_type& operator*() const { return *m_ptr; }
		t_type* operator->() { return m_ptr; }
		iterator& operator++() { m_ptr++; return *this; }
		iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
		//friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
		//friend bool operator!= (const iterator& a, const iterator& b) { return !(a == b); };
		bool operator== (const iterator& other) const { return m_ptr == other.m_ptr; }
		bool operator!= (const iterator& other) const { return !(*this == other); }
	private:
		t_type* m_ptr;
	};

	c_array<t_type, k_max_size>() { zero_object(m_data); }
	~c_array<t_type, k_max_size>() {}
	c_array<t_type, k_max_size>(const c_array<t_type, k_max_size>& other)
	{
		for (int32 i = 0; i < k_max_size; ++i)
		{
			m_data[i] = other.m_data[i];
		}
	}

	c_array<t_type, k_max_size>& operator=(const c_array<t_type, k_max_size>& other)
	{
		if (this != &other)
		{
			for (int32 i = 0; i < k_max_size; ++i)
			{
				m_data[i] = other.m_data[i];
			}
		}

		return *this;
	}

	int32 size() { return k_max_size; }

	t_type& operator[](int32 index)
	{
		assert_valid_index(index);
		return m_data[index];
	}

	const t_type& operator[](int32 index) const
	{
		assert_valid_index(index);
		return m_data[index];
	}

	iterator begin() { return iterator(&m_data[0]); }
	iterator end() { return iterator(&m_data[k_max_size]); }

protected:
	void assert_valid_index(int32 index) const
	{
		ASSERT(index >= 0);
		ASSERT(index < k_max_size);
	}

	t_type m_data[k_max_size];
	// how can we track usage in debug builds?
};


template<class t_type, int32 k_max_size>
class c_stack : public c_array<t_type, k_max_size>
{
public:

	using typename c_array<t_type, k_max_size>::iterator;

	c_stack<t_type, k_max_size>() : m_top(-1) {}

	void push(t_type item)
	{
		ASSERT(!full());
		this->m_data[++m_top] = item;
	}

	void pop()
	{
		ASSERT(!empty());
		m_top--;
	}

	t_type top()
	{
		ASSERT(!empty());
		return this->m_data[m_top];
	}

	t_type* get_item(int32 index)
	{
		ASSERT(0 <= index);
		ASSERT(index <= m_top);
		return &this->m_data[index];
	}

	int32 used() { return m_top + 1; }
	bool empty() { return m_top == -1; }
	bool full() { return m_top == k_max_size - 1; }
	void clear() { m_top = -1; }

	iterator begin() { return c_array<t_type, k_max_size>::begin(); }
	iterator end() { return iterator(&this->m_data[m_top + 1]); }

protected:
	int32 m_top;
};

#endif //__ARRAY_H__