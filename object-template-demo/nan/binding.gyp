{
  "targets": [
    {
      "target_name": "object-template-demo",
      "sources": [ "object-template-demo.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
