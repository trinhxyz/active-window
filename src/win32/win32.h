#ifndef WINPEEK_H
#define WINPEEK_H

#include <windows.h>
#include <psapi.h>
#include <string>

namespace WinPeek
{
    class WinPeek
    {
    public:
        WinPeek();
        ~WinPeek();

        // Method to get the name of the active application
        std::wstring getActiveAppName();

    private:
        // Helper method to get the process name
        std::wstring getProcessName(HANDLE hProc);

        // Helper method to get the file name from a full path
        std::wstring getFileName(const std::wstring &path);
    };
}

#endif
