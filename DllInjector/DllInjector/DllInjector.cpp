#include<stdio.h>
#include<windows.h>
#include<tchar.h>



BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &hToken))
    {
        _tprintf(L"OpenProcessToken error: %u\n", GetLastError());
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL,           // lookup privilege on local system
        lpszPrivilege,  // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        _tprintf(L"LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.
    if (!AdjustTokenPrivileges(hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        _tprintf(L"AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        _tprintf(L"The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}



BOOL InjectDll(DWORD pid, LPCTSTR dllname)
{
    HANDLE hProcess = NULL;
    LPVOID pRemoteAddress = NULL;
    HMODULE hMod = NULL;
    FARPROC pThreadProc = NULL;
    HANDLE hThread = NULL;
    DWORD dwSize = _tcslen(dllname) * 2 + 2;
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)))
    {
        printf("OpenProcess(%d)filed %u", pid, GetLastError());
        return FALSE;

    }

    pRemoteAddress = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(hProcess, pRemoteAddress, dllname, dwSize, NULL);

    hMod = GetModuleHandle(L"kernel32.dll");
    pThreadProc = GetProcAddress(hMod, "LoadLibraryW");

    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pThreadProc, pRemoteAddress, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return TRUE;

}

void _tmain(int argc, TCHAR* argv[])
{
    if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
    {
        printf("SetPrivilege Error");
    }
    if (InjectDll((DWORD)_tstol(argv[1]), (LPCTSTR)argv[2]))
        _tprintf(L"InjectDll(\"%s\") success!!!\n", argv[2]);
    else
        _tprintf(L"InjectDll(\"%s\") failed!!!\n", argv[2]);

}