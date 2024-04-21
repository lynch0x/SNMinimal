#include <Windows.h>
#include <iostream>
#include <MinHook.h>
FILE* f;
const uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");
void OnShowPasswordPrompt(LPVOID cutomGameScreen) {
   
    typedef void(*__fastcall JoinRoom)(LPVOID instanceOfLobbyController, LPVOID roomData);
    ((JoinRoom)(GameAssembly + 0x169E7D0))(nullptr, *(LPVOID*)((uintptr_t)cutomGameScreen + 0x70));
    std::cout << "[INFO] Joined without a passcode!\n";
}
void OnKick() {
    std::cout << "[INFO] Someone tried to kick you!\n";
}
void ForceHost() {
    typedef bool(*__fastcall SetMasterClient)(LPVOID player);
    typedef LPVOID(*__fastcall get_LocalPhotonPlayer)();
    LPVOID localPlayer = ((get_LocalPhotonPlayer)(GameAssembly + 0x38B6380))();
    ((SetMasterClient)(GameAssembly + 0x389E970))(localPlayer);
    std::cout << "[INFO] Forced Host!\n";
}


void AllSkins(bool unlock) {
    DWORD currentProtection;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, PAGE_EXECUTE_READWRITE, &currentProtection);
    *(BYTE*)(GameAssembly + 0x014FA306) = unlock?0x48:0x40;
    *(BYTE*)(GameAssembly + 0x014FA307) = unlock?1:0;
    DWORD shit;
    VirtualProtect((LPVOID)(GameAssembly + 0x014FA306), 2, currentProtection, &shit);
    std::cout << "[INFO] UnlockSkins: "<<(unlock?"TRUE":"FALSE")<<'\n';
} 

void CreateStartGameLoadingMessage() {
    typedef LPVOID(*__fastcall StartGameLoadingMessageCreate)();
    LPVOID message2 = ((StartGameLoadingMessageCreate)(GameAssembly + 0x16C3BE0))();
    reinterpret_cast<void(*__fastcall)(LPVOID, int32_t)>(GameAssembly + 0xB939C0)(message2, 1);
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
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)std::cout << "Failed to apply hooks!\n";
    std::cout << "[DEBUG] Initialized!\n";
   
    AllSkins(true);
    std::cout << "\n\n";
    std::cout << "[INFO] Mini cheat developed by xcar :D\n";
    std::cout << "[INFO] To unload cheat press DELETE on keyboard\n";
    while (true) {
        if (GetAsyncKeyState(VK_DELETE)) {
            break;
        }
        if (GetAsyncKeyState(VK_INSERT)) {
            ForceHost();
         
      }
        Sleep(100);
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    AllSkins(false);
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

