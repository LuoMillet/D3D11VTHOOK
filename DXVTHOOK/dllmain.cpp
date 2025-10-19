// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include"basic.h"
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        detail::conalloc();
        log_info("DXVTHOOK injected!");
        if (BasicConfig::GameBase)
        {
            log_ok("GameAssembly.dll found at address: 0x%p", (void*)BasicConfig::GameBase);
            std::thread(InitImGui).detach();
        }
        else
        {
			log_error("GameAssembly.dll not found!");
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

