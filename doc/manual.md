# BigReals Manual

This is the manual of bigReals version 0.6.0. An online html version of this manual could be
found at GitHub's [bigReals project repository](https://github.com/dfmadrid/bigreals/doc/manual.md).

## <a name="contents">Contents</a>

*   [About bigReals](#about)
*   [Changelog](#changelog)
*   [License](#license)
*   [Compatibility](#compatibility)
*   [Installation](#installation)
*   [Basic usage](#basic)
*   [Integers manual](integers.md)
*   [Irrationals manual](irrationals.md)

***
Next: [About bigReals](#about)&nbsp;&nbsp;&nbsp;Previous: [Contents](#top)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)

###<a name="about">About bigReals</a>

bigReals is a node.js native addon written in C/C++ using GNU's [gmp](http://www.gmp.org) and [mpfr](http://www.mpfr.org) arithmetic
libraries for infinite/multi precision fast calculations over big integers and irrationals numbers. Both *gmp* and *mpfr* are state-of-the-art
libraries in performance with most of its functions implemented in assembly language. bigReals focuses in improving the usability and speed of
calculations with the libraries from node.js, adding some special functions to improve performance or add new functionalities.

***
Next: [changelog](#changelog)&nbsp;&nbsp;&nbsp;Previous: [About bigReals](#about)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="changelog">Changelog</a>

Below you could find the differences between the last released version of bigReals and the past one:

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
Next: [Compatibility](#compatibility)&nbsp;&nbsp;&nbsp;Previous: [License](#license)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="compatibility">Compatibility</a>

BigReals has been tested with *gmp* version 5.0.5 and *mpfr* version 4.0 in Ubuntu Linux 64bits, but should compile and work in other systems,
either 32 or 64bits in Windows, Linux or Apple platforms.

If you find problems or bugs compiling or using bigReals in other systems or with other versions of *gmp* or *mpfr* installed, please open an
issue in GitHub's [bigReals project repository](https://github.com/dfmadrid/bigreals).
    
***
Next: [Installation](#installation)&nbsp;&nbsp;&nbsp;Previous:[Compatibility](#compatibility)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="installation">Installation</a>

A bigReals integer could be compared with a long integer or another bigReals integer with the **cmp** function, which will return 0 if both numbers
are equal, >0 if the argument is smaller or <0 if bigger:

    var bigReals = require('bigreals').integers;

    var op1 = bigReals(9);
    var op2 = bigReals(12);
    
    console.log(op1.cmp(9));
    console.log(op2.cmp(op1));
    console.log(op1.cmp(op2);

Next: [Basic usage](#basics)&nbsp;&nbsp;&nbsp;Previous:[Installation](#installation)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="basics">Basic usage</a>

BigReals combo functions execute several calculations in C/C++ before returning the result to NodeJS, which results in fast calculations mainly
targeted to be executed inside loops. Besides that, all combo functions will add the result of the calculation to the existing value of the
bigReals integer. Current combo functions in bigReals are **addMul**, **subMul** and **accMul**. All the functions allow an optional long
integer or bigReals integer modulus for modular calculations:

    var bigReals = require('bigreals').integers;

    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var modulus = bigReals(3);

    console.log(op1.addMul(9));			// op1 = op1 + op1 * 9
    console.log(op1.addMul(9, modulus));    	// op1 = op1 + op1 * 9 mod 3 
    console.log(op2.subMul(3));         	// op2 = op2 - op2 * 3
    console.log(op1.accMul(op2, modulus);   	// op1 = op1 * op2 mod 3

In future versions, combo functions will allow to specify a maximum value a bigReal integer could have to avoid reallocation of memory in executions
inside loops, speeding up the calculation even more.

***
Next: [Other functions](#other)&nbsp;&nbsp;&nbsp;Previous: [Combo functions](#combos)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

To obtain a string representing the bigReals integer, function **toString** could be used with an optinal string base between 2 and 62
as argument. If no base is provided, base 10 is assumed:


    var bigReals = require('bigreals').integers;

    var op1 = bigReals("ded4b562a4e56432de5", 16);
 
    console.log(op1.toString());
    console.log("Value of op1 is = " + op1);	// toString() will called explictly by Javascript/NodeJS
    console.log(op1.toString(16));

When you inspect a bigReals integer with console log, an object with a string representing the number in base 10 will be obtained:
 
    var bigReals = require('bigreals').integers;

    var op1 = bigReals("ded4b562a4e56432de5", 16);
 
    console.log(op1);

***
Previous: [Other functions](#other)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)
