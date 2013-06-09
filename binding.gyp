{
  "targets": [
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
