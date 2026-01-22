#ifndef __INPUT_SYSTEM_H__
#define __INPUT_SYSTEM_H__
#pragma once

#include "engine/engine_system.h"
#include <types/types.h>
#include <time/time.h>
#include "input_map.h"
#include "events/events.h"

typedef uint16 t_message_id;
typedef uint64 t_param;

const real64 k_input_keydown_allowance_seconds = 0.01f;

struct s_input_event_mouse_data
{
	// does mouse data need to track button down?
	// it might make more sense to treat buttons as keys
	int32 x;
	int32 y;
};

struct s_input_event_key_data
{
	e_input_keycode key;
	//t_param param;
	uint16 repeat_count;
	bool down;
};

struct s_input_event_controller_data
{
	// literally who knows
};

struct s_input_event : s_event
{
	e_event_category get_category() { return window_event_type_input; }
};

struct s_input_key_event : s_input_event
{
	e_event_type get_type() { return event_type_input_key; }

	bool down;
	uint16 message;
	uint64 param;
	uint16 repeat_count;

	s_input_event_key_data data;
};

struct s_input_mouse_event : s_input_event
{
	e_event_type get_type() { return event_type_input_mouse; }

	s_input_event_mouse_data data;
};

struct s_input_controller_event : s_input_event
{
	e_event_type get_type() { return event_type_input_controller; }

	s_input_event_controller_data data;
};

enum e_input_event_key_type : byte
{
	input_event_key_type_none,
	input_event_key_type_num,
	input_event_key_type_char,
	input_event_key_type_special,
};

enum e_input_event_key_special : byte
{
	input_event_key_special_shift,
	input_event_key_special_control,
	input_event_key_special_alt,
	input_event_key_special_esc,
};



class c_key_state
{
public:
	c_key_state() :
		m_is_down(false),
		m_last_changed_timestamp(0),
		m_key_type(input_event_key_type_none),
		u_keycode_num(0)
	{
	}

	bool is_down() const 
	{ 
		if (m_is_down) return true;

		auto span = get_time_since(m_last_changed_timestamp);
		real64 duration_seconds = span.get_duration_seconds();

		return (duration_seconds < k_input_keydown_allowance_seconds);
	}
	
	t_timestamp get_last_changed_timestamp() const { return m_last_changed_timestamp; }

	void set_key_state(bool is_down, t_timestamp timestamp)
	{
		if (is_down != m_is_down)
		{
			m_is_down = is_down;
			m_last_changed_timestamp = timestamp;
		}
	}

private:
	bool m_is_down;
	t_timestamp m_last_changed_timestamp;

	e_input_event_key_type m_key_type;
	union
	{
		uint8 u_keycode_num;
		char u_keycode_char;
		e_input_event_key_special u_keycode_special;
	};
};

class c_input_system : public c_engine_system
{
public:
	virtual void init() override;
	virtual void term() override;
	virtual void update() override;

};

void input_system_handle_event(s_event& event);

bool input_system_queue_message(t_message_id message_id, t_param param);
const c_key_state* input_system_get_key_state(e_input_keycode key);
#endif//__INPUT_SYSTEM_H__