#include "logging.h"

#include "windows.h"
#include <cstdarg>
#include <iostream>
#include <debugapi.h>

#include <types/types.h>
#include <structures/string.h>
#include "asserts.h"
#include <time/time.h>

const int32 k_max_log_length = 256;

s_log_config g_log_config;

const char* get_log_level_string(e_log_level level);

void log_system_init(s_log_config config)
{
	g_log_config = config;
}

void log_system_term()
{

}

void log(e_log_level level, const char* format, ...)
{
	t_timestamp current_time = get_high_precision_timestamp();
	c_time_span time_since_start = get_session_time()->time_since_start(current_time);

	c_string<k_max_log_length> output;

	output.print("%llu %2.3f %s: ", current_time, time_since_start.get_duration_seconds(), get_log_level_string(level));

	va_list args;
	va_start(args, format);

	output.append_va(format, args);
	va_end(args);

	if (g_log_config.log_to_console)
	{
		puts(output.get_const_char());
		OutputDebugString(output.get_const_char());
		OutputDebugString("\n");
	}

	if (g_log_config.log_to_file)
	{
		// todo
	}
}

const char* get_log_level_string(e_log_level level)
{
	switch (level)
	{	
	case verbose:
		return "verbose";
		break;
	case warning:
		return "warning";
		break;
	case error:
		return "error";
		break;
	case critical:
		return "critical";
		break;
	default:
		HALT("Unknown log level");
		break;
	}
	return "unknown";
}
