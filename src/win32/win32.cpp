#include <windows.h>
#include <napi.h>
#include <string>
#include <psapi.h>
#include <dwmapi.h>

// Define constants for different window states
const std::string NO_ACTIVE_WINDOW = "NO_ACTIVE_WINDOW";
const std::string CLOAKED = "CLOAKED";
const std::string UNKNOWN = "UNKNOWN";

// Helper function to get the file name from a full path
std::string getFileName(const std::string &value)
{
    char separator = '/';
#ifdef _WIN32
    separator = '\\';
#endif
    size_t index = value.rfind(separator, value.length());

    if (index != std::string::npos)
    {
        std::string filename = value.substr(index + 1, value.length() - index);
        if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".exe")
        {
            filename = filename.substr(0, filename.size() - 4);
        }
        return filename;
    }

    return value;
}

// Convert wstring to UTF-8 string
std::string toUtf8(const std::wstring &str)
{
    std::string ret;
    int len = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0, NULL, NULL);
    if (len > 0)
    {
        ret.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), &ret[0], len, NULL, NULL);
    }

    return ret;
}

// Get the process name from the handle
std::string getProcessName(const HANDLE &processHandle)
{
    wchar_t exeName[MAX_PATH]{};
    if (GetModuleFileNameExW(processHandle, NULL, exeName, MAX_PATH))
    {
        return getFileName(toUtf8(exeName));
    }
    else
    {
        return UNKNOWN;
    }
}

// Check if window is cloaked
bool isWindowCloaked(HWND hwnd)
{
    int cloaked = 0;
    HRESULT hr = DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &cloaked, sizeof(cloaked));
    return (SUCCEEDED(hr) && cloaked != 0);
}

// Get the active window's name
std::string getActiveWindowName()
{
    HWND hwnd = GetForegroundWindow();

    // If no window is in focus or the desktop is in focus
    if (hwnd == NULL || hwnd == GetShellWindow())
    {
        return NO_ACTIVE_WINDOW;
    }

    // Check if the window is cloaked (hidden)
    if (isWindowCloaked(hwnd))
    {
        return NO_ACTIVE_WINDOW;
    }

    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    if (processHandle == NULL)
    {
        return UNKNOWN;
    }

    std::string name = getProcessName(processHandle);
    CloseHandle(processHandle);

    return name;
}

// N-API wrapper for getActiveWindowName
Napi::Value getActiveWindow(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::string activeWindowName = getActiveWindowName();

    // Return null if no active window is found
    if (activeWindowName == NO_ACTIVE_WINDOW || activeWindowName == UNKNOWN)
    {
        return env.Null();
    }

    // Return the window name as a JavaScript string
    return Napi::String::New(env, activeWindowName);
}

// Init function to export the method to JavaScript
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getActiveWindow", Napi::Function::New(env, getActiveWindow));
    return exports;
}

NODE_API_MODULE(addon, Init)
