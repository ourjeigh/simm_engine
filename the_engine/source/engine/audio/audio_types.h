#ifndef __AUDIO_TYPES_H__
#define __AUDIO_TYPES_H__
#pragma once

#include <types/types.h>

enum e_audio_sample_type
{
	sample_type_unknown = 0,
	sample_type_int16,
	sample_type_float32,
};

struct s_audio_format
{
	e_audio_sample_type sample_type;
	uint16 num_channels;
	uint32 sample_rate;
	uint16 buffer_size;
};

#endif // __AUDIO_TYPES_H__