{
  "targets": [
    {
      "target_name": "bigIntegers",
      "sources": [ "src/bigIntegers/intFactory.cc", "src/bigIntegers/bigInt.cc" ],
      'conditions': [
        ['OS=="linux"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp'
              ]
            }
          }
        ],
        ['OS=="mac"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp'
              ]
            }
          }
        ],
        ['OS=="win"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp.lib'
              ],
            }
          }
        ]
      ]
    },
    {
      "target_name": "bigReals",
      "sources": [ "src/bigReals/realsFactory.cc", "src/bigReals/bigReal.cc" ],
      'conditions': [
        ['OS=="linux"',
          {
            'link_settings': {
              'libraries': [
                '-lmpfr -lgmp'
              ]
            }
          }
        ],
        ['OS=="mac"',
          {
            'link_settings': {
              'libraries': [
                '-lmpfr -lgmp'
              ]
            }
          }
        ],
        ['OS=="win"',
          {
            'link_settings': {
              'libraries': [
                '-lmpfr.lib -lgmp.lib'
              ],
            }
          }
        ]
      ]
    }
  ]
}
