/*
*/

#include"config.h" 
#include"core/error/error.inl"

#if CROWN_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include<Windows.h>

#include<stdio.h>
#include<stdlib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (AttachConsole(ATTACH_PARENT_PROCESS) != 0)
	{
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "W", stdout);
		freopen("CONOUT$", "W", stderr);
	}

	//TODO: WSAStarup()
	CE_ASSERT(1 > 10, "hello");

	Sleep(5 * 1000);

	return 0;
}


#endif
