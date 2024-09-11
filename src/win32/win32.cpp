#include "win32.h"
#include <dwmapi.h> // Required for DwmGetWindowAttribute
#include <iostream>
#pragma comment(lib, "dwmapi.lib") // Link with Dwmapi.lib

namespace WinPeek
{
    WinPeek::WinPeek() = default;
    WinPeek::~WinPeek() = default;

    // Get the name of the active application
    std::wstring WinPeek::getActiveAppName()
    {
        HWND hwnd = GetForegroundWindow();

        // If no window is in focus, the desktop is in focus, or the window is cloaked/minimized
        if (hwnd == NULL || hwnd == GetShellWindow() || IsIconic(hwnd) || isWindowCloaked(hwnd))
        {
            return L"NULL"; // No active window or minimized/cloaked window
        }

        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

        if (hProc == NULL)
        {
            return L"NULL"; // Unable to open process
        }

        std::wstring processName = getProcessName(hProc);
        CloseHandle(hProc);

        return getFileName(processName); // Extract and return the executable name
    }

    // Helper function to get the process name from the process handle
    std::wstring WinPeek::getProcessName(HANDLE hProc)
    {
        wchar_t exeName[MAX_PATH];
        if (GetModuleFileNameExW(hProc, NULL, exeName, MAX_PATH))
        {
            return std::wstring(exeName);
        }

        return L"UNKNOWN";
    }

    // Helper function to get the file name from a full path
    std::wstring WinPeek::getFileName(const std::wstring &path)
    {
        size_t pos = path.find_last_of(L"\\/");
        if (pos != std::wstring::npos)
        {
            std::wstring fileName = path.substr(pos + 1);
            size_t extPos = fileName.find(L".exe");
            if (extPos != std::wstring::npos)
            {
                return fileName.substr(0, extPos); // Return only the file name without the extension
            }
            return fileName;
        }

        return path; // Return full path if separator not found
    }

    // Helper function to check if the window is cloaked (hidden or minimized)
    bool WinPeek::isWindowCloaked(HWND hwnd)
    {
        int isCloaked = 0;
        HRESULT hr = DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked));
        return SUCCEEDED(hr) && isCloaked != 0;
    }
}
