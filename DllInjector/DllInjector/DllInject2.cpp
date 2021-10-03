#include "Windows.h"
#include "stdio.h"

BOOL StartHook(HANDLE hProcess, HANDLE hThread);

BOOL EnableDebugPriv();
int main()
{
    EnableDebugPriv();
    STARTUPINFO sti;
    PROCESS_INFORMATION proci;
    memset(&sti, 0, sizeof(STARTUPINFO));
    memset(&proci, 0, sizeof(PROCESS_INFORMATION));
    sti.cb = sizeof(STARTUPINFO);

    wchar_t ExeName[] = L"notepad.exe";
    DWORD valc = CreateProcess(ExeName, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL\
        , &sti, &proci);
    if (valc == NULL)
    {
        printf("Creaet Process Failed ERROR=%d\n", GetLastError());
        getchar();
        return 0;
    }
    if (!StartHook(proci.hProcess, proci.hThread))
    {
        TerminateProcess(proci.hProcess, 0);
        printf("失败\n");
        getchar();
        return 0;
    }
    ResumeThread(proci.hThread);
    CloseHandle(proci.hProcess);
    CloseHandle(proci.hThread);
    return 0;
}
BYTE ShellCode[64] =
{
0x60,
0x9c,
0x68,
0xaa,0xbb,0xcc,0xdd,//dll path
0xff,0x15,
0xdd,0xcc,0xbb,0xaa,//call [xxxx]
0x9d,
0x61,
0xff,0x25,
0xaa,0xbb,0xcc,0xdd,// jmp [xxxxx]
0xaa,0xaa,0xaa,0xaa,// loadaddr
0xaa,0xaa,0xaa,0xaa//  jmpaddr
};
/*
{
00973689 >    60                PUSHAD
0097368A      9C                PUSHFD
0097368B      68 50369700       PUSH notepad.00973650
00973690      FF15 70369700     CALL DWORD PTR DS:[973670]
00973696      9D                POPFD
00973697      61                POPAD
00973698    - FF25 30369700     JMP DWORD PTR DS:[973630]


}
*/
BYTE* DllPath;
BOOL StartHook(HANDLE hProcess, HANDLE hThread)
{


    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_ALL;
    if (!GetThreadContext(hThread, &ctx))
    {
        printf("GetThreadContext Error\n");
        return FALSE;
    }
    LPVOID LpAddr = VirtualAllocEx(hProcess, NULL, 64, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (LpAddr == NULL)
    {
        printf("VirtualAlloc Error\n");
        return FALSE;
    }
    DWORD LoadDllAAddr = (DWORD)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    if (LoadDllAAddr == NULL)
    {
        printf("LoadDllAddr error\n");
        return FALSE;
    }
    //printf("eip=0x%08x\n",ctx.Eip);
    //printf("lpAddr=0x%08x\n",LpAddr);
    //printf("LoadLibraryAAddr=0x%08x\n",LoadDllAAddr);

    /////////////
    _asm mov esp, esp
    DllPath = ShellCode + 29;
    strcpy((char*)DllPath, "SomeHook.dll");//这里是要注入的DLL名字
    *(DWORD*)(ShellCode + 3) = (DWORD)LpAddr + 29;
    ////////////////
    *(DWORD*)(ShellCode + 21) = LoadDllAAddr;
    *(DWORD*)(ShellCode + 9) = (DWORD)LpAddr + 21;
    //////////////////////////////////
    *(DWORD*)(ShellCode + 25) = ctx.Eip;
    *(DWORD*)(ShellCode + 17) = (DWORD)LpAddr + 25;
    ////////////////////////////////////
    if (!WriteProcessMemory(hProcess, LpAddr, ShellCode, 64, NULL))
    {
        printf("write Process Error\n");
        return FALSE;
    }
    ctx.Eip = (DWORD)LpAddr;
    if (!SetThreadContext(hThread, &ctx))
    {
        printf("set thread context error\n");
        return FALSE;
    }
    return TRUE;
};


BOOL EnableDebugPriv()
{
    HANDLE   hToken;
    LUID   sedebugnameValue;
    TOKEN_PRIVILEGES   tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return   FALSE;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
    {
        CloseHandle(hToken);
        return   FALSE;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        return   FALSE;
    }
    CloseHandle(hToken);
    return TRUE;

}