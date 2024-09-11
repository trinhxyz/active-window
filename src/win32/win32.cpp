#include "win32.h"

namespace WinPeek
{
    WinPeek::WinPeek() = default;
    WinPeek::~WinPeek() = default;

    // Get the name of the active application
    std::wstring WinPeek::getActiveAppName()
    {
        HWND hwnd = GetForegroundWindow();

        if (hwnd == NULL)
        {
            return L"NULL"; // No active window
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
}
