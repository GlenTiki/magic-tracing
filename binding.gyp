{
  "targets": [
    {
      "target_name": "myaddon",
      "sources": [ "myaddon.cc", "simple-tp.cc" ],
      "include_dirs": ["<!(node -e \"require('nan')\")", "./" ],
      "libraries": [ "-llttng-ust" ]
    }
  ]
}
