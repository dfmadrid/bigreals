# BigReals Manual

This is the manual of bigReals version 0.6.0. An online html version of this manual could be
found at GitHub's [bigReals project repository](https://github.com/dfmadrid/bigreals/doc/manual.md).

## <a name="contents">Contents</a>

*   [About bigReals](#about)
*   [Compatibility](#compatibility)
*   [Installation](#installation)
*   [Basic usage](#basics)
*   [Integers manual](integers.md)
*   [Irrationals manual](irrationals.md)
*   [Changelog](#changelog)
*   [License](#license)

***
Next: [About bigReals](#about)&nbsp;&nbsp;&nbsp;Previous: [Contents](#top)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)

###<a name="about">About bigReals</a>

bigReals is a node.js native addon written in C/C++ using GNU's [gmp](http://www.gmp.org) and [mpfr](http://www.mpfr.org) arithmetic
libraries for infinite/multi precision fast calculations over big integers and irrationals numbers. Both *gmp* and *mpfr* are state-of-the-art
libraries in performance with most of its functions implemented in assembly language. bigReals focuses in improving the usability and speed of
calculations with the libraries from node.js, adding some special functions to improve performance or add new functionalities.
    
***
Next: [Compatibility](#compatibility)&nbsp;&nbsp;&nbsp;Previous: [License](#license)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="compatibility">Compatibility</a>

BigReals has been tested under *gmp* version 5.0.5 and *mpfr* version 3.1.1 in Ubuntu Linux amd64-i386, but should compile and work in other systems,
either 32 or 64bits in Windows, Linux or Apple platforms.

If you find problems or bugs compiling using bigReals in other systems or with other versions of *gmp* or *mpfr* installed, please open an
issue in GitHub's [bigReals project repository](https://github.com/dfmadrid/bigreals).
    
***
Next: [Installation](#installation)&nbsp;&nbsp;&nbsp;Previous:[Compatibility](#compatibility)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="installation">Installation</a>

Installation in Linux/OS-X platforms is quite straightforward, just use your favorite package manager to install libgmp and libmpfr sources:

    $ sudo apt-get install libgmp-dev
    $ sudo apt-get install libmpfr-dev

Anyway, is quite feasible you already have installed libmpfr-dev package in your system. 

Once gmp's and mpfr's sources are installed, clone bigReals project from github into a node_modules folder in the path of your nodejs
project:

    $ git clone git://github.com/dfmadrid/bigreals.git

Then cd to the directory and execute npm install to build bigReals modules:

    $ cd bigreals
    $ npm install

Installation in Windows platforms in equally simple and requires the same steps if you have installed all the required components that
**node-gyp** uses to compile and build native nodejs addons:

*   Python 2.7.3
*   MS Visual Studio C++ 2010 for WXP/Vista/7 32bits systems
*   MS Visual Studio C++ 2010 and/or Windows 7 SDK for WXP/Vista/7 64 bits systems
*   MS Visual Studio C++ 2010 for W7/8

Please check **node-gyp** [project's page](https://github.com/TooTallNate/node-gyp) for more details and troubleshooting.

***
Next: [Basic usage](#basics)&nbsp;&nbsp;&nbsp;Previous:[Installation](#installation)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="basics">Basic usage</a>

BigReals exposes to Javascript two factory functions for creating irrational numbers using **mpfr** and integers-only
numbers using **gmp**. To create a bigReals number, just call the appropiate factory:

    var biginteger = require('bigreals').integers;
    var bigreal = require('bigreals');

    var op1 = bigreal("Pi", 100);
    var op2 = biginteger(1223435345465656767678678768988879789789789879879);

    console.log(op1);			
    console.log(op2);

Each factory/interface has its own unique characteristics and functions. Integers factory is suitable for lightway, fast 
calculations like those involved in cryptography and irrationals factory is a general purpose interface for dealing with
integers, floating point number and special numbers and constants with support for trigonometry, logarithms, etc. Please
check the irrationals and integer manuals for more details about both factories.

***
Next: [changelog](#changelog)&nbsp;&nbsp;&nbsp;Previous: [Basic usage](#basics)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="changelog">Changelog</a>

Below you could find the differences between a specific released version of bigReals and the previous one:

#### 0.6.0

*   Added irrationals support using MPFR
*   Rearranged folder structure
*   Added irrationals and integers manual
*   Added test for irrationals functions

***
Next: [License](#license)&nbsp;&nbsp;&nbsp;Previous: [Changelog](#changelog)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="license">License</a>

(The MIT License)

Copyright (c) 2013 David Fernández Madrid <david.fdmv@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
***
Previous: [license](#license)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)
