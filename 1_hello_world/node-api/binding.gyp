{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "hello.cc" ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "dependencies": [
        "<!(node -p \"require('node-api').gyp\")"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-api').include\")"
      ]
    }
  ]
}
