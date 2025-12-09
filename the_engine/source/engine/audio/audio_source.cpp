#include "audio_source.h"
#include "mmath.h"

void c_audio_source_sine::get_samples(real32* buffer, uint32 num_samples)
{
	ASSERT(buffer);

	while (num_samples)
	{
		*buffer++ = math_sin((k_math_real32_two_pi * m_frequency) * (m_sample_position++ / this->m_sample_rate));
		num_samples--;
	}
}

void c_audio_source_noise::get_samples(real32* buffer, uint32 num_samples)
{
	ASSERT(buffer);

	while (num_samples--)
	{
		g_x1 ^= g_x2;
		*buffer++ = g_x2 * g_level;
		g_x2 += g_x1;
	}
}