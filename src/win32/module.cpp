#include <napi.h>
#include "win32.h" // Include the header file for the WinPeek class

namespace module
{

    // Declare the getActiveWindow function before it is used
    Napi::Value getActiveWindow(const Napi::CallbackInfo &info);

    // Initialize the module, setting up the WinPeek instance
    void Init(Napi::Env env, Napi::Object exports)
    {
        // Create a new instance of WinPeek and store it in the NAPI instance data
        env.SetInstanceData(new WinPeek::WinPeek());

        // Export getActiveWindow function to JavaScript
        exports.Set("getActiveWindow", Napi::Function::New(env, getActiveWindow));
    }

    // Define the getActiveWindow function
    Napi::Value getActiveWindow(const Napi::CallbackInfo &info)
    {
        Napi::Env env = info.Env();

        // Retrieve the WinPeek instance from the NAPI instance data
        WinPeek::WinPeek *winPeek = env.GetInstanceData<WinPeek::WinPeek>();
        if (winPeek == nullptr)
        {
            Napi::Error::New(env, "WinPeek module not initialized").ThrowAsJavaScriptException();
            return env.Null();
        }

        // Get the active window name from the WinPeek instance
        std::wstring windowName = winPeek->getActiveAppName();

        if (windowName == L"NULL")
        {
            return env.Null();
        }

        // Convert the std::wstring to UTF-8 and return the window name as a JavaScript string
        std::string utf8WindowName(windowName.begin(), windowName.end());
        return Napi::String::New(env, utf8WindowName);
    }

} // namespace module

// Module initialization hook
Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    module::Init(env, exports); // Call the module's Init function to set up exports
    return exports;
}

// Define the module
NODE_API_MODULE(active_window, InitAll)
