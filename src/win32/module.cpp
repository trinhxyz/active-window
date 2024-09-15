#include <napi.h>
#include "win32.h"

namespace module
{

    Napi::Value getActiveWindow(const Napi::CallbackInfo &info);

    void Init(Napi::Env env, Napi::Object exports)
    {
        env.SetInstanceData(new WinPeek::WinPeek());
        exports.Set("getActiveWindow", Napi::Function::New(env, getActiveWindow));
    }

    Napi::Value getActiveWindow(const Napi::CallbackInfo &info)
    {
        Napi::Env env = info.Env();

        WinPeek::WinPeek *winPeek = env.GetInstanceData<WinPeek::WinPeek>();
        if (winPeek == nullptr)
        {
            Napi::Error::New(env, "WinPeek module not initialized").ThrowAsJavaScriptException();
            return env.Null();
        }

        std::wstring windowName = winPeek->getActiveAppName();

        if (windowName == L"NULL")
        {
            return env.Null();
        }

        std::string utf8WindowName(windowName.begin(), windowName.end());
        return Napi::String::New(env, utf8WindowName);
    }

}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    module::Init(env, exports);
    return exports;
}

NODE_API_MODULE(active_window, InitAll)
