{
  "targets": [
    {
      "target_name": "tsfn_object_wrap",
      "sources": [
        "tsfn_object_wrap.cc",
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NODE_API_SWALLOW_UNTHROWABLE_EXCEPTIONS"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
    }
  ]
}
