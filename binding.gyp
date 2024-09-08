{
    "targets": [
        {
            "target_name": "winpeek",
            "sources": ["src/win32/win32.cpp"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
            ],
            "conditions": [
                [
                    "OS=='win'",
                    {
                        'libraries': [
                            'version.lib',
                            'Dwmapi.lib',
                        ],
                    },
                ],
            ],

            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
        }
    ]
}
