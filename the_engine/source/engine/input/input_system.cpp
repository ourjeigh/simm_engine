#include "input_system.h"
#include <debug/logging.h>
#include "input_map.h"
#include <asserts.h>
#include <time/time.h>
#include <structures/array.h>
#include "memory/memory.h"
#include <memory/allocator.h>
#include <platform/platform.h>

// definitions
struct s_input_queued_event
{
	e_event_type type;
	t_timestamp timestamp;

	union 
	{
		s_input_event_mouse_data mouse_data;
		s_input_event_key_data key_data;
		s_input_event_controller_data controller_data;
	};
};

struct s_input_state
{
	c_array<c_key_state, k_input_key_count> key_states;
	
	void clear()
	{
		zero_object(key_states);
	}
};

// prototypes
void process_input_event_queue_internal();

// internal data

// called and written from the message pump, processed in the input system update
// todo this needs to be thread safe
c_stack<s_input_queued_event, 256> g_input_event_queue;
s_input_state g_input_state;

// public methods
void c_input_system::init()
{
	g_input_event_queue.clear();
	g_input_state.clear();
	log(verbose, "Input System Initialized");
}

void c_input_system::term()
{
	g_input_state.clear();
	log(verbose, "Input System Terminated");
}

void c_input_system::update()
{
	//log(verbose, "Input System Update");
	
	// we probably want to move input processing to a different thread that can run as fast as possible to reduce input latency
	process_input_event_queue_internal();
}

const c_key_state* input_system_get_key_state(e_input_keycode key)
{
	return &g_input_state.key_states[key];
}

void input_system_handle_event(s_event& event)
{
	ASSERT(event.get_category() == window_event_type_input);

	s_input_queued_event& new_queue = g_input_event_queue.push();
	new_queue.timestamp = get_high_precision_timestamp();
	new_queue.type = event.get_type();

	switch (new_queue.type)
	{
	case event_type_input_key:
	{
		s_input_key_event& key_event = static_cast<s_input_key_event&>(event);
		new_queue.key_data = key_event.data;
		break;
	}
	case event_type_input_mouse:
	{
		s_input_mouse_event& mouse_event = static_cast<s_input_mouse_event&>(event);
		new_queue.mouse_data = mouse_event.data;
		break;
	}
	case event_type_input_controller:
	{
		s_input_controller_event& controller_event = static_cast<s_input_controller_event&>(event);
		new_queue.controller_data = controller_event.data;
		break;
	}
	default:
		HALT_UNIMPLEMENTED();
	}
}

// private methods

void process_input_event_queue_internal()
{
	while(!g_input_event_queue.empty())
	{
		s_input_queued_event event = g_input_event_queue.top();
		g_input_event_queue.pop();

		switch (event.type)
		{
		case event_type_input_key:
		{
			s_input_event_key_data& data = event.key_data;
			g_input_state.key_states[data.key].set_key_state(data.down, event.timestamp);
			log(verbose, "input system: key:%i %s repeat:%i", 
				data.key, 
				data.down ? "down" : "up", 
				data.repeat_count);
			// todo, handle repeat
			break;
		}
		case event_type_input_mouse:
		case event_type_input_controller:
			HALT_UNIMPLEMENTED();
		}
	}

	g_input_event_queue.clear();
}