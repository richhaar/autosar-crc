{
  "targets": [
    {
      "target_name": "crc_addon",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions", "-std=c++17", "-Ofast" ],
      "sources": [ "./src/main.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],

      "xcode_settings": {
          "OTHER_CFLAGS": [ "-std=c++17", "-Ofast" ],
      },
    }
  ]
}
