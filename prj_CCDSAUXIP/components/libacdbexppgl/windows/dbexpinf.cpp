/*
 *  dbexpinf.cpp - Informix dbExpress driver Win32 DLL entry point
 *
 *    Copyright (c) 2001-2004, Luxena Software Company. All rights reserved.
 *
 *  Purpose:
 *
 */

#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}
