#include <napi.h>
#include "macos.h"  // Include the header file for the WinPeek class

namespace module {

    // Declare the getActiveWindow function before it is used
    Napi::Value getActiveWindow(const Napi::CallbackInfo& info);
    
    // Declare the runLoop function
    void runLoop(const Napi::CallbackInfo& info);

    // Initialize the module, setting up the WinPeek instance
    void Init(Napi::Env env, Napi::Object exports) {
        // Create a new instance of WinPeek and store it in the NAPI instance data
        env.SetInstanceData(new WinPeek::WinPeek());

        // Export getActiveWindow and runLoop functions to JavaScript
        exports.Set("getActiveWindow", Napi::Function::New(env, getActiveWindow));
        exports.Set("runLoop", Napi::Function::New(env, runLoop));
    }

    // Define the getActiveWindow function
    Napi::Value getActiveWindow(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        // Retrieve the WinPeek instance from the NAPI instance data
        WinPeek::WinPeek* winPeek = env.GetInstanceData<WinPeek::WinPeek>();
        if (winPeek == nullptr) {
            Napi::Error::New(env, "ActiveWindow module not initialized").ThrowAsJavaScriptException();
            return env.Null();
        }

        // Get the active window name from the WinPeek instance
        const char* name = winPeek->getActiveWindow();
        if (name == nullptr) {
            Napi::Error::New(env, "Failed to get active window name").ThrowAsJavaScriptException();
            return env.Null();
        }

        // Return the active window name as a JavaScript string
        return Napi::String::New(env, name);
    }

    // Define the runLoop function
    void runLoop(const Napi::CallbackInfo& info) {  // Removed 'module::' extra qualification
        Napi::Env env = info.Env();

        // Retrieve the ActiveWindow instance
        WinPeek::WinPeek* winPeek = env.GetInstanceData<WinPeek::WinPeek>();
        
        // Check if the ActiveWindow instance was initialized
        if (winPeek == nullptr) {
            Napi::Error::New(env, "WinPeek module not initialized").ThrowAsJavaScriptException();
            return;
        }

        // Call the runLoop method
        winPeek->runLoop();
    }

    // Thread-safe function callback for main thread execution
    void tsfnMainThreadCallback(Napi::Env env, Napi::Function jsCallback, void* ctx, char* data) {
        if (data == nullptr) {
            // If no data, pass null to the JavaScript callback
            jsCallback.Call({ env.Null() });
        } else {
            // Pass the active window name as a string to the JavaScript callback
            jsCallback.Call({ Napi::String::New(env, data) });
        }
    }

}  // namespace module

// Module initialization hook
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    module::Init(env, exports);  // Call the module's Init function to set up exports
    return exports;
}

// Define the module
NODE_API_MODULE(active_window, InitAll)
