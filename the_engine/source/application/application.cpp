#include "application.h"
#include <engine/engine.h>
#include "window.h"
#include <events/delegates.h>


void c_application::run()
{
	c_window window;
	//c_application* this_application = this;
	//window.set_event_callback(c_callback::from(this, &c_application::window_event_callback));
	c_application this_app = *this;

	window.set_event_callback(
		MAKE_DELEGATE(
			t_windows_event_callback,
			this_app,
			window_event_callback));

	window.init();

	engine_init();
}

void c_application::window_event_callback(s_window_event& event)
{
	NOP();
}
