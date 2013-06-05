bigReals
========

node.js native c/c++ addon using [gmp](http://gmplib.org/) and [mpfr](http://www.mpfr.org/) to perform fast, infinite precision, loop-optimized calculations and bit manipulations over big integers, big rationals and big floating point numbers.

bigReals is the addon used by [cryptatoo](https://github.com/dfmadrid/cryptatoo) to perform cryptographic analysis and attacks over large numbers like RSA keys of 1024 bits. bigReals is a work in progress, currently only big integer support is fully operational.

Installation
------------
bigReals has been tested on Linux, but should compile and work fine in Windows or Mac provided [gmp](http://gmplib.org/) sources are installed. To install gmp's sources on Ubuntu Linux:

    $ sudo apt-get install libgmp-dev

While a npm package is created, you can install bigReals cloning from github:

    $ cd node_modules
    $ git clone git://github.com/dfmadrid/bigreals.git
    $ cd bigreals
    $ npm install

Testing
-------
A complete battery of tests is included under directory test. To run all tests, execute npm test:

    $ npm test

To run a specific test, execute it with node:

    $ node test/modular

Documentation
-------------
Documentation will follow soon. Meanwhile, check tests for guiding about usage.

Examples
--------
Normal and modular arithmetics:

    var bigReal = require('bigreals');
    
    var op1 = bigReal(20);
    var op2 = bigReal(30);
    var mod = bigReal(3);

    console.log(op1.add(op2).toString());       // 20 + 30 = 50
    console.log(op1.mul(2, mod).toString());    // 20 * 2 mod 3 = 1

Integer roots with or without remainder:

    var bigReal = require('bigreals');
    
    var op = bigReal(30);
    var mod = bigReal(3);
    var remainder = bigReal(1);
    console.log(op.root(3).toString());        // 30 ^ 1/3 = 3 (truncated)
    var result = op.root(remainder, 3);             // 30 ^ 1/3 = 3
    console.log(remainder.add(result.pow(3)).toString());  // 3 + 3 ^ 3 = 30

Hash table creation and lookup using special loop-optimized functions:

    var bigReal = require('bigreals');
    
    var op1 = bigReal(20);
    var op2 = bigReal(30);
    var op3 = bigReal("500");
    var hashTable = {};
    var cont = 0;

    while(cont < 100){
     hashTable[op1.accMul(3).toString(16)] = op2.sub(op3).toString();    // op1.accMul(3) -> op1 = op1 * 3;
     cont++;
     }

    cont = 0;
    var value = "21d20f70b1835f3560511394";

    while(cont < 100){
     if(hashTable[value]){
      console.log(hashTable[value]);                // Value of hash 21d20f70b1835f3560511394 is -470
	    cont = 100;
	}
      cont++;
    }

Contributions
-------------
If you have any idea, performance improvement or function missing that would be interesting to have included in bigReals, send an email to &lt;david.fdmv@gmail.com&gt; or open a pull request in Github. If you find a bug, please open an issue on Github with the steps to reproduce it. Patches are more than welcome, of course.

License
-------
(The MIT License)

Copyright (c) 2013 David Fern&aacute;ndez Madrid &lt;david.fdmv@gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
