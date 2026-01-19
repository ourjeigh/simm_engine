#include "pch.h"
#include <callbacks.h>

class c_caller_no_payload
{
public:
	void set_callback(c_callback cb)
	{
		m_cb = cb;
	}

	void trigger()
	{
		if (m_cb)
		{
			m_cb();
		}
	}

private:
	c_callback m_cb;
};

bool g_callback_result_no_data_called;

class c_handler_no_payload
{
public:
	void handle_callback()
	{
		g_callback_result_no_data_called = true;
	}
};

struct s_payload
{
	int value;
};

class c_caller_with_payload
{
public:
	void set_callback(c_callback cb)
	{
		m_cb = cb;
	}

	void trigger(void* arg)
	{
		if (m_cb)
		{
			m_cb(arg);
		}
	}

private:
	c_callback m_cb;
};

int g_callback_result_data;

class c_handler_with_payload
{
public:
	void handle_callback(void* arg)
	{
		auto* data = static_cast<s_payload*>(arg);
		g_callback_result_data = data->value;
	}
};

bool g_static_callback_result_no_payload_called;

static void static_callback_handler_no_payload()
{
	g_static_callback_result_no_payload_called = true;
}

int g_static_callback_result_with_payload_data;

static void static_callback_handler_with_payload(void* arg)
{
	auto* data = static_cast<s_payload*>(arg);
	g_static_callback_result_with_payload_data = data->value;
}

TEST(CALLBACKS, CALLBACK_NO_PAYLOAD)
{
	g_callback_result_no_data_called = false;

	c_handler_no_payload foo;
	c_caller_no_payload bar;

	bar.set_callback(MAKE_CALLBACK(&foo, c_handler_no_payload::handle_callback));

	bar.trigger();

	EXPECT_TRUE(g_callback_result_no_data_called);
}

TEST(CALLBACKS, CALLBACK_WITH_PAYLOAD)
{
	g_callback_result_data = 0;

	c_handler_with_payload foo;
	c_caller_with_payload bar;

	bar.set_callback(MAKE_CALLBACK(&foo, c_handler_with_payload::handle_callback));

	s_payload payload{ 6 };
	bar.trigger(&payload);

	EXPECT_EQ(g_callback_result_data, 6);
}

TEST(CALLBACKS, STATIC_CALLBACK_NO_PAYLOAD)
{
	g_static_callback_result_no_payload_called = false;

	c_caller_no_payload bar;

	bar.set_callback(MAKE_STATIC_CALLBACK(static_callback_handler_no_payload));
	bar.trigger();

	EXPECT_TRUE(g_static_callback_result_no_payload_called);
}

TEST(CALLBACKS, STATIC_CALLBACK_WITH_PAYLOAD)
{
	g_static_callback_result_with_payload_data = 0;

	c_caller_with_payload bar;
	bar.set_callback(MAKE_STATIC_CALLBACK(static_callback_handler_with_payload));

	s_payload payload{ 6 };
	bar.trigger(&payload);

	EXPECT_EQ(g_static_callback_result_with_payload_data, 6);
}