//#include <engine/engine.h>
//#include <asserts.h>
//#include <engine/engine_system.h>
//#include <engine/input/input_system.h>
//#include <engine/input/input_map.h>
//#include "memory/memory.h"
//#include <threads/threads.h>
//#include <logging/logging.h>
//
#include "application/application.h"
#include "platform/platform.h"


#ifdef PLATFORM_WINDOWS

IGNORE_WINDOWS_WARNINGS_PUSH
#include <windows.h>
IGNORE_WINDOWS_WARNINGS_POP

//const char* k_application_name = "SiMM Engine";

c_application g_application;

int main(int argc, char** argv)
{
	//engine_init();
	

	return 0;
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	
	g_application.run();
	/*c_application* application = new c_application();
	application->run();
	delete application;*/
	
	return 0;
}


#endif // PLATFORM_WINDOWS