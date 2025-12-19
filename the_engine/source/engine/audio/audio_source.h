#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__
#pragma once

#include <types/types.h>
#include <engine/file_system/file.h>
#include "audio_types.h"

class c_audio_source
{
public:
	c_audio_source() : m_sample_rate(0.0f) {}
	c_audio_source(int32 sample_rate) : m_sample_rate(static_cast<real32>(sample_rate)) {}
	~c_audio_source() {}

	virtual void get_samples(real32* buffer, uint32 num_samples) = 0;
protected:
	real32 m_sample_rate;
};

class c_audio_source_sine : public c_audio_source
{
public:
	c_audio_source_sine() :
		m_sample_position(0),
		m_frequency(0.0f),
		c_audio_source(0)
	{
	}

	c_audio_source_sine(int32 sample_rate, real32 frequency) :
		m_sample_position(0),
		m_frequency(frequency),
		c_audio_source(sample_rate)
	{
	}
	~c_audio_source_sine() {}

	void get_samples(real32* buffer, uint32 num_samples);

private:
	uint32 m_sample_position;
	real32 m_frequency;
};

class c_audio_source_noise : public c_audio_source
{
public:
	c_audio_source_noise() : c_audio_source() {}
	c_audio_source_noise(int32 sample_rate) : c_audio_source(sample_rate) {}

	void get_samples(real32* buffer, uint32 num_samples);

private:

	float g_level = 2.0f / 0xffffffff;
	int g_x1 = 0x67452301;
	int g_x2 = 0xefcdab89;
};

class c_audio_source_file : public c_audio_source
{
public:
	c_audio_source_file() : m_position(k_invalid), c_audio_source() {}
	~c_audio_source_file();

	void set_file(c_file_path file_path);
	void get_samples(real32* buffer, uint32 num_samples);
private:
	c_file m_file;
	s_audio_file_format m_format;
	uint32 m_position;
	bool m_looping = true;
};

#endif // __AUDIO_SOURCE_H__
