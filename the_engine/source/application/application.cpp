#include "application.h"
#include <engine/engine.h>
#include "window.h"
#include <callbacks.h>


void c_application::run()
{
	c_window window;
	window.init();
	//c_application* this_application = this;
	//window.set_event_callback(c_callback::from(this, &c_application::window_event_callback));

	engine_init();
}

void c_application::window_event_callback(s_window_event& event)
{
}

static void* window_event_callback(s_window_event& event)
{

}
