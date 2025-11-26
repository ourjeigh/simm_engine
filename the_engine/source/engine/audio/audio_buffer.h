#ifndef __AUDIO_BUFFER_H__
#define __AUDIO_BUFFER_H__
#pragma once

#include <types/types.h>
#include "structures/array.h"
#include "memory.h"

template<typename t_type, int32 k_channel_count, int32 k_size>
class c_audio_buffer
{
public:
	c_audio_buffer() { zero_object(m_data); }
	~c_audio_buffer() {}
	c_audio_buffer(const c_audio_buffer& other) { m_data = other.m_data; }
	
	c_audio_buffer& operator=(const c_audio_buffer& other)
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}

	int32 channel_count() const { return k_channel_count; }

	c_array<t_type, k_size>* get_channel(int32 channel_index)
	{
		ASSERT(0 <= channel_index && channel_index < k_channel_count);
		return &m_data[channel_index];
	}

	const c_array<t_type, k_size>* get_channel_const(int32 channel_index) const
	{
		ASSERT(0 <= channel_index && channel_index < k_channel_count);
		return &m_data[channel_index];
	}

	t_type* get_channel_raw(int32 channel_index)
	{
		ASSERT(0 <= channel_index && channel_index < k_channel_count);
		return &m_data[channel_index][0];
	}
private:
	c_array<c_array<t_type, k_size>, k_channel_count> m_data;
};

template<typename t_type, int32 k_channel_count, int32 k_size>
class c_audio_ring_buffer
{
public:

	typedef c_array<c_array<t_type, k_size>, k_channel_count> t_multichannel_buffer;

	c_audio_ring_buffer() : m_write_position(0), m_read_position(0) { zero_object(m_buffer); }
	~c_audio_ring_buffer() {}

	// returns actual samples written
	int32 write(const c_audio_buffer<t_type, k_channel_count, k_size>* in_buffer, int32 sample_count);
	
	// returns actual samples read
	int32 read(c_audio_buffer<t_type, k_channel_count, k_size>* out_buffer, int32 sample_count);

private:
	int32 m_write_position;
	int32 m_read_position;
	c_audio_buffer<t_type,k_channel_count, k_size> m_buffer;
};


template<int32 k_size>
class c_audio_real32_mono_buffer : public c_audio_ring_buffer<real32, 1, k_size>
{
};

#include "audio_buffer.inl"
#endif // __AUDIO_BUFFER_H__
