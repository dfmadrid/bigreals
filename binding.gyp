{
  "targets": [
    {
      "target_name": "bigIntegers",
      "sources": [ "intFactory.cc", "bigInt.cc" ],
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
      "sources": [ "realsFactory.cc", "bigReal.cc" ],
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
