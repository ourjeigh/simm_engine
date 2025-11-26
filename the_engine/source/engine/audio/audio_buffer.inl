#include "mmath.h"

// returns actual samples written
template<typename t_type, int32 k_channel_count, int32 k_size>
int32 c_audio_ring_buffer<t_type, k_channel_count, k_size>::write(const c_audio_buffer<t_type, k_channel_count, k_size>* in_buffer, int32 sample_count)
{
	int32 free_samples = (m_read_position - m_write_position + k_size) % k_size;
	int32 samples_to_write = math_min(sample_count, free_samples);
	int32 first_block = k_size - samples_to_write;

	for (int32 channel_index = 0; channel_index < in_buffer->channel_count(); channel_index++)
	{
		memory_copy(&m_buffer.get_channel(channel_index)[m_write_position], &(in_buffer->get_channel_const(channel_index))[0], sizeof(t_type) * first_block);
	}

	if (first_block < samples_to_write)
	{
		int32 second_block = samples_to_write - first_block;

		for (int32 channel_index = 0; channel_index < in_buffer->channel_count(); channel_index++)
		{
			memory_copy(&m_buffer.get_channel(channel_index)[m_write_position + first_block], &in_buffer->get_channel_const(channel_index)[first_block], sizeof(t_type) * second_block);
		}
	}

	m_write_position = (m_write_position + samples_to_write) % k_size;

	return samples_to_write;
}

// returns actual samples read
template<typename t_type, int32 k_channel_count, int32 k_size>
int32 c_audio_ring_buffer<t_type, k_channel_count, k_size>::read(c_audio_buffer<t_type, k_channel_count, k_size>* out_buffer, int32 sample_count)
{

}