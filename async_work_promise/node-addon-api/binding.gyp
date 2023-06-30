{
  "targets": [{
    "target_name": "addon",
    "sources": [
      "addon.cc"
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")"
    ],
    'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    'conditions': [
      [ 'OS=="win"', {
        'defines': [ '_HAS_EXCEPTIONS=1' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'ExceptionHandling': 1,
          },
        },
      }],
      [ 'OS=="linux"', {
        "cflags": [ "-fexceptions" ],
        "cflags_cc": [ "-fexceptions" ],
      }],
      [ 'OS=="mac"', {
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'CLANG_CXX_LIBRARY': 'libc++',
        }
      }]
    ],
  }]
}
