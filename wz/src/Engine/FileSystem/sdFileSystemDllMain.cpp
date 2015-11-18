#include "sdFileSystemPCH.h"
#include "sdFileSystemLibType.h"

//#pragma init_seg(compiler)

#ifdef SD_ENGINE_FILESYSTEM__EXPORT
	BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
	{
		if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		{
			//OutputDebugString("aaaaa");
		}
		else if (ul_reason_for_call == DLL_PROCESS_DETACH)
		{

		}

		return TRUE;
	}
#endif