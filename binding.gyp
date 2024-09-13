{
    "targets": [
        {
            "target_name": "winpeek",
            "conditions": [
                ["OS=='win'", {
                    "sources": [
                        "src/win32/win32.cpp",
                        "src/win32/module.cpp"
                    ],
                    "libraries": [
                        "User32.lib",
                        "Shell32.lib",
                        "Version.lib",
                        "Shlwapi.lib",
                        "Gdi32.lib",
                        "Gdiplus.lib",
                        "Windowsapp.lib"
                    ],
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": [
                                "/std:c++17"
                            ]
                        }
                    }
                }],
                ["OS=='mac'", {
                    "sources": [
                        "src/macos/macos.mm",
                        "src/macos/module.mm"
                    ],
                    "libraries": [
                        "-lc++",
                        "-framework Foundation",
                        "-framework AppKit",
                        "-framework ApplicationServices"
                    ],
                    "xcode_settings": {
                        "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                    }
                }]
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "defines": [
                "NAPI_DISABLE_CPP_EXCEPTIONS",
                "NAPI_VERSION=<(napi_build_version)"
            ]
        }
    ]
}
