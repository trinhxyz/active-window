{
    "targets": [
        {
            "target_name": "win32",
            "sources": ["src/win32/win32.cpp", "src/win32/module.cpp"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "libraries": [
                "version.lib",
                "Dwmapi.lib"
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "conditions": [
                ["OS=='win'", {}]
            ]
        },
        {
            "target_name": "macos",
            "sources": ["src/macos/macos.mm", "src/macos/module.mm"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "libraries": [
                "-framework AppKit",
                "-framework CoreFoundation",
                "-framework ApplicationServices"
            ],
            "cflags!": ["-fno-exceptions", "-std=c++17"],
            "cflags_cc!": ["-fno-exceptions", "-std=c++17", "-fobjc-arc"],
            "xcode_settings": {
                "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            },
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "conditions": [
                ["OS=='mac'", {}]
            ]
        }
    ]
}
