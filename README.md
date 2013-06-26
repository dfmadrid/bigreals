bigReals
========

node.js native c/c++ addon using [gmp](http://gmplib.org/) and [mpfr](http://www.mpfr.org/) to perform fast, infinite precision,
loop-optimized calculations and bit manipulations over big integers and big irrationals numbers.

In a nutshell, bigReals offers the following features:

   * Easy and fast modular arithmetics.
   * Comprehensive trigonometric, exponentiation and logarithm functions.
   * Special numbers support(NaN, Infinity, log2, Pi, Euler's and Catalan's constants).
   * Loop optimized functions to speed up calculation from Javascript.
   * Precision of irrationals calculations is chosen based on the highest between the operand's precision if none is provided as argument.

bigReals is the addon used by [cryptatoo](https://github.com/dfmadrid/cryptatoo) to perform cryptographic analysis and attacks over
large numbers like RSA keys of 1024 bits. bigReals is a work in progress, currently integer and irrationals support is fully operational but
new functions and improvements will be added in future versions.

Installation
------------
An *npm* package will be created when bigReals arrives to version 1.0. Meanwhile, you can install bigReals cloning from github:

    $ cd node_modules
    $ git clone git://github.com/dfmadrid/bigreals.git
    $ cd bigreals
    $ npm install

Check the [documentation](doc/manual.md) for more info about compatibility and installation in the different platforms.

Testing
-------
A complete battery of tests is included under directory test. To run all tests, execute npm test:

    $ npm test

To run a specific test, execute it with node:

    $ node test/modular

Documentation
-------------
Under folder *doc* you could find the [bigReals manual](doc/manual.md) with a complete description of bigReals interface and many basic examples.
bigReals source code is verboselly commented as well to favour collaboration in development.

Examples
--------
Under folder *examples* you could find more ellaborated examples of bigReal number usage like hash table creation and lookup.

Contributions
-------------
If you have any idea, performance improvement or function missing particularly related to cryptography or number theory that
would be interesting to have included in bigReals, open a pull request in Github. If you find a bug, please open an issue on
Github with the steps to reproduce it.

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
