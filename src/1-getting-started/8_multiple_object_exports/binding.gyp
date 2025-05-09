{
  "targets": [
    {
      # myModule is the name of your native addon
      "target_name": "addon",
      "sources": [
        "addon.cc",
        "myobject-napi.cc",
        "myobject.cc",
        "otherobject-napi.cc",
        "otherobject.cc"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except_all"
      ]
    }
  ]
}

