{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cc", "timer.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
