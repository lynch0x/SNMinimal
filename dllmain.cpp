// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>
#include <iostream>
#include <MinHook.h>
FILE* f;
const uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");
void OnShowPasswordPrompt(LPVOID cutomGameScreen) {
   
    typedef void(*__fastcall JoinRoom)(LPVOID instanceOfLobbyController, LPVOID roomData, UINT type1, UINT type2);
    ((JoinRoom)(GameAssembly + 0x169E190))(nullptr, *(LPVOID*)((uintptr_t)cutomGameScreen + 0x70), 2, 2);
    std::cout << "[INFO] Joined without a passcode!\n";
}
void OnKick() {
    std::cout << "[INFO] Someone tried to kick you!\n";
}

void AddAllSkins() {
    DWORD currentProtection;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, PAGE_EXECUTE_READWRITE, &currentProtection);
    *(BYTE*)(GameAssembly + 0x014FA306) = 0x48;
    *(BYTE*)(GameAssembly + 0x014FA307) = 1;
    DWORD shit;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, currentProtection, &shit);
}
void RemoveAllSkins() {
    DWORD currentProtection;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, PAGE_EXECUTE_READWRITE, &currentProtection);
    *(BYTE*)(GameAssembly + 0x014FA306) = 0x40;
    *(BYTE*)(GameAssembly + 0x014FA307) = 0;
    DWORD shit;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, currentProtection, &shit);
}
DWORD WINAPI Main(HMODULE m) {
    AllocConsole();
    SetConsoleTitleA("SNMagicDll (by xcar)");
    freopen_s(&f, "CONOUT$", "w", stdout);
   
    std::cout << "[DEBUG] Initializing hooking library\n";
    MH_Initialize();
    std::cout << "[DEBUG] Initializing hooks\n";
    MH_CreateHook((LPVOID*)(GameAssembly + 0x12ADFA0), &OnShowPasswordPrompt, NULL);
    MH_CreateHook((LPVOID*)(GameAssembly + 0x1456E00), &OnKick, NULL);
    MH_EnableHook(MH_ALL_HOOKS);
    std::cout << "[DEBUG] Initialized!\n";
    std::cout << "[DEBUG] Adding skins\n";
    AddAllSkins();
    std::cout << "\n\n";
    std::cout << "[INFO] Mini cheat developed by xcar :D\n";
    std::cout << "[INFO] To unload cheat press DELETE on keyboard\n";
    while (true) {
        if (GetAsyncKeyState(VK_DELETE)) {
            break;
        }
      
        Sleep(100);
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    RemoveAllSkins();
    if(f)
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(m, 0);
    return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == 1) {
        DisableThreadLibraryCalls(hModule);

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr);

    }
    return TRUE;
}

