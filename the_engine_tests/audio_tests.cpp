#include "pch.h"
#include "engine/audio/audio_buffer.h"

TEST(AUDIO, RING_BUFFER)
{
	c_audio_real32_mono_buffer<10> ring_buffer;

	real32 data[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	c_audio_buffer<real32, 1, 10> write_buffer;
	memory_copy(write_buffer.get_channel_raw(0), &data[0], 10 * sizeof(real32));

	ring_buffer.write(&write_buffer, 5);
}