#include "window.h"
#include "structures/array.h"
#include <threads/threads.h>
#include "windows.h"
#include <engine/input/input_system.h>

//remove
const char* k_application_name = "SiMM Engine";

struct s_window_thread_params
{
	HINSTANCE instance;
	c_window* window;
};

LRESULT CALLBACK process_message_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void window_thread_entry_point(c_window* window);


void c_window::init()
{
	c_thread window_thread;
	m_instance = GetModuleHandle(nullptr);

	window_thread.create(THREAD_FUNCTION(window_thread_entry_point), THREAD_ARGS(this), WIDE("Window Thread"));
	window_thread.start();
}

void c_window::term()
{
}


const int32 k_window_max_height = 2400;
const int32 k_window_max_width = 3840;
const int32 k_window_buffer_size = k_window_max_height * k_window_max_width;

struct s_backbuffer
{
	int width;
	int height;
	BITMAPINFO bmi;
	//void* memory;
	c_array<uint32, k_window_buffer_size> memory;
};

static s_backbuffer g_backbuffer = {};

void resize_backbuffer(HWND hwnd, int width, int height)
{
	g_backbuffer.width = width;
	g_backbuffer.height = height;

	g_backbuffer.bmi = {};
	g_backbuffer.bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	g_backbuffer.bmi.bmiHeader.biWidth = width;
	g_backbuffer.bmi.bmiHeader.biHeight = -height; // top-down
	g_backbuffer.bmi.bmiHeader.biPlanes = 1;
	g_backbuffer.bmi.bmiHeader.biBitCount = 32;
	g_backbuffer.bmi.bmiHeader.biCompression = BI_RGB;
}

void render()
{
	//uint32* pixels = (uint32*)g_backbuffer.memory;
	//auto pixels = .data();

	// Clear to dark gray
	for (int y = 0; y < g_backbuffer.height; y++)
	{
		for (int x = 0; x < g_backbuffer.width; x++)
		{
			g_backbuffer.memory[y * g_backbuffer.width + x] = 0xFF202020;
		}
	}

	// Simple test rectangle
	for (int y = 50; y < 150; y++)
	{
		for (int x = 50; x < 200; x++)
		{
			g_backbuffer.memory[y * g_backbuffer.width + x] = 0xFFFF0000; // red
		}
	}
}

void present(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	StretchDIBits(
		hdc,
		0, 0, g_backbuffer.width, g_backbuffer.height,
		0, 0, g_backbuffer.width, g_backbuffer.height,
		g_backbuffer.memory.data(),
		&g_backbuffer.bmi,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	ReleaseDC(hwnd, hdc);
}

void window_thread_entry_point(c_window* window)
{
	// Register the window class.
	const char* CLASS_NAME = k_application_name;

	WNDCLASS window_class = {};

	window_class.lpfnWndProc = process_message_callback;
	window_class.hInstance = window->m_instance;
	window_class.lpszClassName = CLASS_NAME;

	RegisterClass(&window_class);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,								// Optional window styles.
		CLASS_NAME,						// Window class
		k_application_name,				// Window text
		WS_OVERLAPPEDWINDOW,			// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// Position (X Y)
		CW_USEDEFAULT, CW_USEDEFAULT,	// Size (Width Height)
		NULL,							// Parent window
		NULL,							// Menu
		window->m_instance,						// Instance handle
		NULL							// Additional application data
	);

	if (hwnd == NULL)
	{
		return;
	}

	SetLastError(0);

	LONG_PTR windowptr = reinterpret_cast<LONG_PTR>(window);
	auto result = SetWindowLongPtr(
		hwnd,
		GWLP_USERDATA,
		windowptr);

	if (result == 0)
	{
		HRESULT result = HRESULT_FROM_WIN32 (GetLastError());
		NOP();
	}


	ShowWindow(hwnd, SW_SHOWDEFAULT);

	MSG msg;
	zero_object(msg);

	bool quit = false;
	while (GetMessage(&msg, hwnd, 0, 0) != 0 && !quit)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		render();
		present(hwnd);

		// this should be moved to engine code and signalled to the message thread
		const c_key_state* escape_key = input_system_get_key_state(_input_key_special_esc);
		if (escape_key != nullptr && escape_key->is_down())
		{
			quit = true;
		}
	}

}

LRESULT CALLBACK process_message_callback(HWND hwnd, UINT msg, WPARAM param, LPARAM lParam)
{
	LONG_PTR user_data = GetWindowLongPtr(
		hwnd,
		GWLP_USERDATA
	);

	c_window* window = reinterpret_cast<c_window*>(user_data);

	if (window == nullptr)
	{
		NOP();
	}

	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (msg == WM_QUIT)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (msg == WM_SIZE)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		resize_backbuffer(hwnd, rect.right - rect.left, rect.bottom - rect.top);
		return 0;
	}

	if (msg == WM_SETFOCUS || msg == WM_KILLFOCUS)
	{
		// handle focus change

		s_window_event event;
		event.test = 24;
		window->send_window_event(event);
		return 0;
	}

	
	if (msg == WM_PAINT)
	{
		ValidateRect(hwnd, nullptr);
		return 0;
	}

	if (input_system_queue_message(msg, param))
	{
		return 0;
	}

	return DefWindowProc(hwnd, msg, param, lParam);
}