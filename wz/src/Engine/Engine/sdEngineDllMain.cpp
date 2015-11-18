#include "sdEnginePCH.h"
#include "sdEngineLibType.h"

#ifdef SD_ENGINE_EXPORT
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{

	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{

	}

	return TRUE;
}
#endif