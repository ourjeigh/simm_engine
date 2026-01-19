#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__
#pragma once

#define MAKE_STATIC_CALLBACK(function) c_callback::from<function>()

#define MAKE_CALLBACK(obj, function) \
	c_callback::from< \
		typename callback_detail::method_class<decltype(&function)>::type, \
		&function>(obj)


class c_callback
{
public:
	using t_callback_function = void(*)(void* object, void* arg);

	c_callback() = default;

	// instanced, no arg
	template <class t_class, void (t_class::* t_function)()>
	static c_callback from(t_class* instance)
	{
		return c_callback{ instance, &invoke_no_arg<t_class, t_function> };
	}

	// instanced, with arg
	template <class t_class, void (t_class::* t_function)(void*)>
	static c_callback from(t_class* instance)
	{
		return c_callback{ instance, &invoke_with_arg<t_class, t_function> };
	}

	// static, no arg
	template <void (*t_function)()>
	static c_callback from()
	{
		return c_callback{ &invoke_static_no_arg<t_function> };
	}

	// static, with arg
	template <void (*t_function)(void*)>
	static c_callback from()
	{
		return c_callback{ &invoke_static_with_arg<t_function> };
	}

	void operator()(void* arg = nullptr) const
	{
		m_function(m_instance, arg);
	}

	explicit operator bool() const
	{
		return m_function != nullptr;
	}

private:
	c_callback(t_callback_function function) : m_instance(nullptr), m_function(function) {}
	c_callback(void* object, t_callback_function function) : m_instance(object), m_function(function) {}

	template <class t_class, void (t_class::* t_function)()>
	static void invoke_no_arg(void* obj, void*)
	{
		(static_cast<t_class*>(obj)->*t_function)();
	}

	template <class t_class, void (t_class::* t_function)(void*)>
	static void invoke_with_arg(void* obj, void* arg)
	{
		(static_cast<t_class*>(obj)->*t_function)(arg);
	}

	template <void (*t_function)()>
	static void invoke_static_no_arg(void*, void*)
	{
		t_function();
	}

	template <void (*t_function)(void*)>
	static void invoke_static_with_arg(void*, void* arg)
	{
		t_function(arg);
	}

	void* m_instance = nullptr;
	t_callback_function m_function = nullptr;
};

namespace callback_detail
{
	template <typename>
	struct method_class;

	template <typename T>
	struct method_class<void (T::*)()>
	{
		using type = T;
	};

	template <typename T>
	struct method_class<void (T::*)(void*)>
	{
		using type = T;
	};
}


#endif //__CALLBACKS_H__
