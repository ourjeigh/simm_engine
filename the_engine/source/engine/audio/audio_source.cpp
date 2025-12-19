#include "audio_source.h"
#include "mmath.h"
#include "audio_types.h"
#include "audio_buffer.h"

void c_audio_source_sine::get_samples(real32* buffer, uint32 num_samples)
{
	ASSERT(buffer != nullptr);

	while (num_samples)
	{
		*buffer++ = math_sin((k_math_real32_two_pi * m_frequency) * (m_sample_position++ / this->m_sample_rate));
		num_samples--;
	}
}

void c_audio_source_noise::get_samples(real32* buffer, uint32 num_samples)
{
	ASSERT(buffer != nullptr);

	while (num_samples--)
	{
		g_x1 ^= g_x2;
		*buffer++ = g_x2 * g_level;
		g_x2 += g_x1;
	}
}

c_audio_source_file::~c_audio_source_file()
{
	m_file.close();
}

e_audio_sample_type get_sample_type_from_bits_per_sample(uint16 bits_per_sample)
{
	e_audio_sample_type out_type = audio_sample_type_unknown;
	switch (bits_per_sample)
	{
	case 16:
		out_type = audio_sample_type_int16;
		break;
	case 24:
		out_type = audio_sample_type_int24;
		break;
	default:
		break;
	}

	return out_type;
}

e_audio_compression_format get_compression_format_from_wave_format(uint16 format)
{
	e_audio_compression_format out_format = audio_compression_format_unknown;

	switch (format)
	{
	case 1:
		out_format = audio_compression_format_pcm;
		break;
	default:
		break;
	}

	return out_format;
}

void c_audio_source_file::set_file(c_file_path file_path)
{
	t_file_open_mode_flags flags;
	flags.set(file_open_mode_read, true);
	m_file.open(file_path, flags);

	c_array<byte, 2048> header_buffer;
	auto ref = header_buffer.make_reference();
	m_file.read_bytes(0, header_buffer.capacity(), ref);

	uint32 index = 0;
	s_audio_wav_header_riff riff_chunk;
	memory_copy(&riff_chunk, &ref.data()[index], sizeof(s_audio_wav_header_riff));

	index += sizeof(s_audio_wav_header_riff);

	s_audio_wav_header_format format_chunk;
	memory_copy(&format_chunk, &ref.data()[index], sizeof(s_audio_wav_header_format));

	m_format.sample_type = get_sample_type_from_bits_per_sample(format_chunk.bits_per_sample);
	m_format.channel_count = format_chunk.channel_count;
	m_format.sample_rate = format_chunk.sample_rate;
	m_format.bits_per_sample = format_chunk.bits_per_sample;
	m_format.block_align = format_chunk.block_align;

	index += sizeof(s_audio_wav_header_format);

	// now we're in uncharted territory. look for the "data" chunk
	s_audio_wav_header_chunk chunk;
	bool data_chunk_found = false;
	do {
		// TODO handle running off the end of header_buffer and needing to keep reading the file.
		memory_copy(&chunk, &ref.data()[index], sizeof(s_audio_wav_header_chunk));

		// TODO make this use a string, string needs to be able to take in a non-terminated char array
		data_chunk_found = chunk.name[0] != 'd' && chunk.name[0] != 'a' && chunk.name[0] != 't' && chunk.name[0] != 'a';

		if (!data_chunk_found)
		{
			// the next chunk will start chunk_size bytes away
			index += sizeof(s_audio_wav_header_chunk) + chunk.chunk_size;
		}

	} while (!data_chunk_found);

	m_format.sample_count = chunk.chunk_size / format_chunk.channel_count / format_chunk.bits_per_sample;
	m_format.data_position = index + sizeof(s_audio_wav_header_chunk);// -chunk.chunk_size;;
	m_position = 0;

	NOP();
}

inline real32 convert_sample_to_real32(byte* in, e_audio_sample_type sample_type)
{
	real32 out = 0.0f;

	switch (sample_type)
	{
	case audio_sample_type_int8:
	{
		const uint8 k_audio_int8_zero = 128;
		const real32 divizor = 1 / k_audio_int8_zero;
		out = (static_cast<uint8>(*in) - k_audio_int8_zero) * divizor;
		HALT_UNIMPLEMENTED(); // needs testing
		break;
	}
	case audio_sample_type_int16:
	{
		const real32 divizor = 1 / (k_int16_max + 1);
		out = (*reinterpret_cast<int16*>(in)) * divizor;
		HALT_UNIMPLEMENTED(); // needs testing
		break;
	}
	case audio_sample_type_int24:
	{
		constexpr uint32 int24_max = math_pow<uint32>(2, 31);
		const real32 divisor = 1.0f / int24_max;
		out = ((in[2] << 24) | (in[1] << 16) | (in[0] << 8)) * divisor;
		break;
	}
	case audio_sample_type_int32:
	{
		const real32 divizor = 1 / (k_int32_max + 1);
		return (*reinterpret_cast<int32*>(in)) * divizor;
		HALT_UNIMPLEMENTED(); // needs testing
		break;
	}
	case audio_sample_type_float32:
	{
		out = *reinterpret_cast<real32*>(in);
		break;
	}
	default:
		HALT("Unknown sample type");
	}
}

void c_audio_source_file::get_samples(real32* out_buffer, uint32 num_samples)
{
	ASSERT(out_buffer != nullptr);
	c_array<byte, 4096> temp_buffer;

	int32 start_position = m_format.data_position + (m_format.block_align * m_position);

	int32 bytes_to_read = num_samples * m_format.block_align;
	ASSERT(bytes_to_read < temp_buffer.capacity());

	auto ref = temp_buffer.make_reference();
	int32 bytes_read = m_file.read_bytes(start_position, bytes_to_read, ref);

	c_static_audio_buffer<real32, 2, 1024> audio_buffer;

	int32 sample_index = 0;
	const int32 bytes_per_sample = m_format.bits_per_sample / 8;
	for (int32 byte_index = 0; byte_index < bytes_read; byte_index += m_format.block_align)
	{
		for (int32 channenl_index = 0; channenl_index < m_format.channel_count; channenl_index++)
		{
			real32 sample = convert_sample_to_real32(&temp_buffer.data()[byte_index + (bytes_per_sample * channenl_index)], m_format.sample_type);
			audio_buffer.get_channel(channenl_index)[sample_index] = sample;
		}
		sample_index++;
	}

	memory_copy(out_buffer, audio_buffer.get_channel(0), num_samples * sizeof(real32));

	m_position += num_samples;

	if (bytes_read < bytes_to_read)
	{
		if (m_looping)
		{
			m_position = 0;
			int32 second_pass_bytes_to_read = bytes_to_read - bytes_read;
			int32 second_pass = m_file.read_bytes(m_format.data_position, second_pass_bytes_to_read, ref);
			ASSERT(second_pass == bytes_to_read - bytes_read);

			sample_index = 0;
			for (int32 byte_index = 0; byte_index < second_pass; byte_index += m_format.block_align)
			{
				for (int32 channenl_index = 0; channenl_index < m_format.channel_count; channenl_index++)
				{
					real32 sample = convert_sample_to_real32(&temp_buffer.data()[byte_index + (bytes_per_sample * channenl_index)], m_format.sample_type);
					audio_buffer.get_channel(channenl_index)[sample_index] = sample;
				}
				sample_index++;
			}

			memory_copy(out_buffer, audio_buffer.get_channel(0), num_samples * sizeof(real32));

		}
		else
		{
			HALT_UNIMPLEMENTED();
		}
	}
}
