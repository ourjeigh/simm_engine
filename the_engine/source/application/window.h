#ifndef __WINDOW_H__
#define __WINDOW_H__
#pragma once

#include <asserts.h>
#include <memory>
#include "events/delegates.h"
#include "types/types.h"	

#include "windows.h"

struct s_window_event
{
	int32 test;
};

struct s_window_params
{

};

using t_windows_event_callback = void(s_window_event&);

class c_window
{
public:
	void init();
	void term();
	void set_event_callback(c_delegate<t_windows_event_callback> callback)
	{
		m_event_callback = callback;
	}

	void send_window_event(s_window_event& event)
	{
		if (m_event_callback.is_valid())
		{
			m_event_callback(event);
		}
	}

	// hack
	HINSTANCE m_instance;
private:
	c_delegate< t_windows_event_callback> m_event_callback;
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
