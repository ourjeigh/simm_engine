#ifndef __WINDOW_H__
#define __WINDOW_H__
#pragma once

#include <asserts.h>
#include <memory>
#include "callbacks.h"

struct s_window_event
{
};

struct s_window_params
{

};

class c_window
{
public:
	void init();
	void term();
	void set_event_callback(c_callback callback)
	{
		m_event_callback = callback;
	}

private:
	c_callback m_event_callback;
};

//class c_window_windows : public c_window
//{
//};
//
//c_window* create_window(i_allocator* allocator)
//{
//#ifdef PLATFORM_WINDOWS
//	c_window_windows* window = reinterpret_cast<c_window_windows*>(allocator->allocate(sizeof(c_window_windows), alignof(c_window_windows)));
//	return window;
//#endif //PLATFORM_WINDOWS
//}

#endif //__WINDOW_H__
