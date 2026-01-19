#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "window.h"

class c_application
{
public:
	void run();
	void window_event_callback(s_window_event& event);
private:
};

#endif //__APPLICATION_H__
