{
  # This defines a variable which we can pass along to node-gyp as an
  # argument. For example:  node-gyp build --coverage=true
  "variables": {
    "coverage": "false"
  },
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        [
          "coverage == 'true' and OS == 'mac'",
          {
            "xcode_settings": {
              # We need to pass this flag in both places since the compile and
              # link phases are run separately. See `man gcc` for details on
              # what this flag does during each phase.
              "OTHER_CFLAGS+": [ "--coverage" ],
              "OTHER_LDFLAGS+": [ "--coverage" ]
            }
          }
        ],
        [
          "coverage == 'true' and OS == 'linux'",
          {
            # We need to pass this flag in both places since the compile and
            # link phases are run separately. See `man gcc` for details on
            # what this flag does during each phase.
            "cflags+": [ "--coverage" ],
            "ldflags+": [ "--coverage" ]
          }
        ]
      ]
    }
  ]
}
