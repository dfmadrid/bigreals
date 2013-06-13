{
  "targets": [
    {
      "target_name": "bigInteger",
      "sources": [ "bigInteger.cc", "bigInt.cc" ],
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
      "target_name": "bigIrrational",
      "sources": [ "bigIrrational.cc", "bigFloat.cc" ],
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
