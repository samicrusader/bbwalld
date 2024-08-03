#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#define len(array) ((sizeof(array) / sizeof(array[0])))
 
const char* nconvertArgs[] = { "-ratio", "-resize", "longest", "<width>", "-canvas", "<width>", "<height>", "center", "-quiet", "-overwrite", "-out", "bmp", "-o", "<destfn>", "<sourcefn>", "\0"};
const char* bsetrootArgs[] = { "-full", "<destfn>", "\0"};
char nconvertPath[64];
char bsetrootPath[64];
 
DWORD RunSilent(char* BinPath, const char* ParamArray[]) {
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
    ULONG ReturnCode;
    char Args[1024];
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(STARTUPINFO);
    StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
    //StartupInfo.wShowWindow = SW_HIDE;
    strcpy(Args, ParamArray[0]);
    for (int i = 1; ParamArray[i] != "\0"; i++) {
        strcat(Args, " ");
        strcat(Args, ParamArray[i]);
    }
    char launchCmdline[4096];
    char launchLog[4096];
    sprintf(launchCmdline, "\"%s\" %s", BinPath, Args);
    sprintf(launchLog, "Launching process %s with parameters %s...\n", BinPath, Args);
    OutputDebugString(launchLog);
    if (!CreateProcess(NULL, launchCmdline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo)) {
        DWORD err = GetLastError();
        char errorText[64];
        sprintf(errorText, "Process launch failed with error code: %d\n", err);
        OutputDebugString(errorText);
        return err;
    }
    OutputDebugString("Launched process!\n");
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    if (!GetExitCodeProcess(ProcessInfo.hProcess, &ReturnCode))
        ReturnCode = 0;
 
    CloseHandle(ProcessInfo.hThread);
    CloseHandle(ProcessInfo.hProcess);
    OutputDebugString("Process returned!\n");
    return ReturnCode;
}
 
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    char width[10];
    char height[10];
    const char** args = nconvertArgs;
    switch (message) {
        case WM_DISPLAYCHANGE:
            _itoa(GetSystemMetrics(SM_CXSCREEN), width, 10);
            _itoa(GetSystemMetrics(SM_CYSCREEN), height, 10);
            OutputDebugString("Resolution changed!\n");
            args[3] = width;
            args[5] = width;
            args[6] = height;
            //_spawnv(_P_WAIT, nconvertPath, args);
            //_spawnv(_P_NOWAIT, bsetrootPath, bsetrootArgs);
            RunSilent(nconvertPath, nconvertArgs);
            RunSilent(bsetrootPath, bsetrootArgs);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
 
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
    char sourcefn[39];
    char destfn[39];
    sprintf(nconvertPath, "%s\\Program Files\\nconvert\\nconvert.exe", getenv("systemdrive"));
    sprintf(bsetrootPath, "%s\\Program Files\\bbLean\\bsetroot.exe", getenv("systemdrive"));
    sprintf(sourcefn, "%s\\Windows\\winpe.jpg", getenv("systemdrive"));
    sprintf(destfn, "%s\\Windows\\Temp\\bg.bmp", getenv("systemdrive"));
    nconvertArgs[13] = destfn;
    bsetrootArgs[1] = destfn;
    nconvertArgs[14] = sourcefn;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX),0,WindowProcedure,0,0,hThisInstance,NULL,NULL,0,NULL,"bbwalld",NULL };
    if (!RegisterClassEx(&wc)) {
        return 0;
    }
 
    CreateWindowEx(0, "bbwalld", 0, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, HWND_DESKTOP, NULL, hThisInstance, NULL);
 
    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
 
    return messages.wParam;
}