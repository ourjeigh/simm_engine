#include "input_system.h"
#include <debug/logging.h>
#include "input_map.h"
#include <asserts.h>
#include <time/time.h>
#include <structures/array.h>
#include "memory/memory.h"

// definitions

struct s_input_queued_event
{
	t_message_id message_id;
	t_param param;
	t_timestamp timestamp;
};

// remove, this could be turned directly from the queued event into the state
struct s_input_key_event_internal
{
	t_timestamp timestamp;
	bool is_down;
	e_input_event_key_type key_type;
	union
	{
		uint8 keycode_num;
		char keycode_char;
		e_input_event_key_special keycode_special;
	};
};

struct s_input_state
{
	c_array<c_key_state, 10> num_key_states;
	c_array<c_key_state, 26> char_key_states;
	c_array<c_key_state, 10> special_key_states;
	
	void clear()
	{
		zero_object(*this);
	}
};

// prototypes
void process_input_event_queue_internal();
s_input_key_event_internal process_input_event_internal(const s_input_queued_event& event);
s_input_queued_event create_event(t_message_id message_id, t_param param);

// internal data

// called and written from the message pump, processed in the input system update
c_stack<s_input_queued_event, 256> g_input_event_queue;
s_input_state g_input_state;


// public methods
void c_input_system::init()
{
	log(verbose, "Input System Initialized");
}

void c_input_system::term()
{
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
	if (k_input_key_first_num <= key && key <= k_input_key_last_num)
	{
		int32 index = key - static_cast<int32>(k_input_key_first_num);
		ASSERT(0 <= index && index < g_input_state.num_key_states.capacity());
		return &g_input_state.num_key_states[index];
	}
	else if (k_input_key_first_char <= key && key <= k_input_key_last_char)
	{
		int32 index = key - static_cast<int32>(k_input_key_first_char);
		ASSERT(0 <= index && index < g_input_state.char_key_states.capacity());
		return &g_input_state.char_key_states[index];
	}
	else if (k_input_key_first_special <= key && key <= k_input_key_last_special)
	{
		int32 index = key - static_cast<int32>(k_input_key_first_special);
		ASSERT(0 <= index && index < g_input_state.special_key_states.capacity());
		return &g_input_state.special_key_states[index];
	}
	else
	{
		log(warning, "input_system_get_key_state: unrecognized keycode %llu", key);
	}

	return nullptr;
}



void input_system_handle_event(s_event& event)
{
	ASSERT(event.get_category() == window_event_type_input);

	// just push it onto our queue
}

// remove
bool input_system_queue_message(t_message_id message_id, t_param param)
{
	bool handled = false;
	switch (message_id)
	{
	case k_input_id_key_down:
	case k_input_id_key_up:
		s_input_queued_event new_event = create_event(message_id, param);
		g_input_event_queue.push(new_event);
		handled = true;
		break;
	}
	return handled;
}

// private methods
//remove
s_input_queued_event create_event(t_message_id message_id, t_param param)
{
	s_input_queued_event event;
	event.message_id = message_id;
	event.param = param;
	event.timestamp = get_high_precision_timestamp();
	return event;
}

void process_input_event_queue_internal()
{
	// rework, remove internal events and queued events, just go from the external s_event to the states
	c_stack<s_input_key_event_internal, 256> processed_events;

	// process all queued input events
	for (s_input_queued_event& event : g_input_event_queue)
	{
		s_input_key_event_internal new_event= process_input_event_internal(event);

		processed_events.push(new_event);
	}

	g_input_event_queue.clear();

	// update state
	for (s_input_key_event_internal& event : processed_events)
	{
		switch (event.key_type)
		{
		case input_event_key_type_num:
		{
			int32 index = event.keycode_num;
			ASSERT(0 <= index && index < g_input_state.num_key_states.capacity());
			g_input_state.num_key_states[index].set_key_state(
				event.is_down,
				event.timestamp);
			break;
		}
		case input_event_key_type_char:
		{
			int32 index = event.keycode_char - 'A';
			ASSERT(0 <= index && index < g_input_state.char_key_states.capacity());
			g_input_state.char_key_states[index].set_key_state(
				event.is_down,
				event.timestamp);
			break;
		}
		case input_event_key_type_special:
		{
			int32 index = static_cast<int32>(event.keycode_special);
			ASSERT(0 <= index && index < g_input_state.special_key_states.capacity());
			g_input_state.special_key_states[index].set_key_state(
				event.is_down,
				event.timestamp);
			break;
		}
		default:
			break;
		}
	}

	// broadcast changed states
}

s_input_key_event_internal process_input_event_internal(const s_input_queued_event& event)
{ 
	c_time_span span(event.timestamp, get_high_precision_timestamp());
	
	s_input_key_event_internal processed_event;
	zero_object(processed_event);
	
	processed_event.timestamp = event.timestamp;

	if (event.message_id == k_input_id_key_down)
	{
		processed_event.is_down = true;
	}
	else if (event.message_id == k_input_id_key_up)
	{
		processed_event.is_down = false;
	}

	if (k_input_id_kbd_0 <= event.param && event.param <= k_input_id_kbd_9)
	{
		processed_event.key_type = input_event_key_type_num;
		processed_event.keycode_num = static_cast<uint8>(event.param - k_input_id_kbd_0);
		log(verbose, "Processed Keyboard %s Event: %i , delay=%.4fs", 
			processed_event.is_down ? "Down" : "Up",
			processed_event.keycode_num, event.param, span.get_duration_seconds());
	}
	else if (k_input_id_kbd_a <= event.param && event.param <= k_input_id_kbd_z)
	{
		processed_event.key_type = input_event_key_type_char;
		processed_event.keycode_char = static_cast<char>(event.param);
		log(verbose, "Processed Keyboard %s Event: %c , delay=%.4fs",
			processed_event.is_down ? "Down" : "Up",
			processed_event.keycode_char, span.get_duration_seconds());
	}
	else if (event.param == k_input_id_kbd_shift)
	{
		processed_event.key_type = input_event_key_type_special;
		processed_event.keycode_special = input_event_key_special_shift;
		log(verbose, "Processed Keyboard %s Event: SHIFT , delay=%.4fs", 
			processed_event.is_down ? "Down" : "Up",
			span.get_duration_seconds());
	}
	else if (event.param == k_input_id_kbd_control)
	{
		processed_event.key_type = input_event_key_type_special;
		processed_event.keycode_special = input_event_key_special_control;
		log(verbose, "Processed Keyboard %s Event: CONTROL , delay=%.4fs", 
			processed_event.is_down ? "Down" : "Up",
			span.get_duration_seconds());
	}
	else if (event.param == k_input_id_kbd_menu)
	{
		processed_event.key_type = input_event_key_type_special;
		processed_event.keycode_special = input_event_key_special_alt;
		log(verbose, "Processed Keyboard %s Event: ALT , delay=%.4fs", 
			processed_event.is_down ? "Down" : "Up",
			span.get_duration_seconds());
	}
	else if (event.param == k_input_id_kbd_esc)
	{
		processed_event.key_type = input_event_key_type_special;
		processed_event.keycode_special = input_event_key_special_esc;
		log(verbose, "Processed Keyboard %s Event: ESCAPE, delay=%.4fs",
			processed_event.is_down ? "Down" : "Up",
			span.get_duration_seconds());
	}
	else
	{
		log(warning, "Unrecognized key event: message_id=%llu, param=%llu", event.message_id, event.param);
	}

	return processed_event;
}
