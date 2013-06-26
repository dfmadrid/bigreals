# BigReals Irrationals

BigReals uses [GNU's MPFR](http://www.mpft.org "MPFR") for multi-precision fast calculations over irrationals numbers.
This chapter discusses the special interface of BigReals to MPFR and how to use it.

## <a name="contents">Contents</a>

*   [Precision](#precision)
*   [Rounding modes](#rounding)
*   [Initialization](#initialization)
*   [Basic arithmetics](#arithmetics)
*   [Exponentiation, roots and logarithms](#logexp)
*   [Trigonometry](#trigonometry)
*   [Integer and remainder functions](#integer)
*   [Comparison functions](#comparison)
*   [Number theory](#numbertheory)
*   [Other functions](#other)

***
Next: [Precision](#precision)&nbsp;&nbsp;&nbsp;Previous: [Contents](#top)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)

###<a name="precision">Precision</a>

Precision is the of number bits used by BigReals to store internally an irrational number and could be any number between 2 and
the maximum unsigned integer value allowed by your processor. By default, each bigReal number is created with a precision of 53 bits.
This value can be changed by modifying the **precision** property of the object **defaults** of the factory/module:

    var bigReals = require('bigreals');

    /** Settles the default precision for new numbers created to 100 bits.
     ** Previously created numbers are not affected by this change.
     **/

    bigReals.defaults.precision = 100;

This default precision can be overriden in a specific number by specifying a precision upon initialization:

    var bigReals = require('bigreals');

    // Creates a 16 bits precision irrational number with value 4.0

    var op = bigReals(4.0, 16);

To avoid losing precision when executing calculations, bigReals will calculate the result based on an infinite precision and then
will round it to the higher between the operands' precisions:

    var bigReals = require('bigreals');

    // Creates a 24 bits and a 32 bits precision irrational numbers.

    var op1 = bigReals(4.0, 16);
    var op2 = bigReals(6.55, 32);

    // Outputs a 32 bits precision number with value 10.5500

    console.log(op1.add(op2).toString());

This behaviour can be overriden by specifying a precision as argument of the calculation:
   
    var bigReals = require('bigreals');
    var op1 = bigReals(4.0, 16);
    var op2 = bigReals(6.55, 32);

    // Outputs a 24 bits precision number rounded to the nearest with value 10.5500

    console.log(op1.add(op2, 24);

You can get or set the precision of a bigReal number by using the **precision** function:

    var bigReals = require('bigreals');
    var op = bigReals(4.0);

    console.log("Precision of op is = " + op.precision();
    op.precision(5000);
    console.log("New precision of op is = " + op.precision());

***
Next: [Rounding modes](#rounding)&nbsp;&nbsp;&nbsp;Previous: [Precision](#precision)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="rounding">Rounding modes</a>

Rounding mode speficies the way to round the result of a calculation in case rounding is needed. The four allowed modes in BigReals
are the same than MPFR ones:

*   0 (MPFR_RNDN: round to nearest)
*   1 (MPFR_RNDZ: round toward zero)
*   2 (MPFR_RNDU: round toward plus infinity)
*   3 (MPFR_RNDD: round toward minus infinity)
*   4 (MPFR_RNDA: round away from zero)

Each BigReals irrational number has associated a rounding mode with it, the default mode is 0 (round to nearest). This default
mode can be settled for all irrational numbers created with bigReals by changing the **rMode** property of the object defaults
of the factory/module:

    var bigReals = require('bigreals');

    /** Settles the default rounding mode to round toward minus infinity for new numbers created.
     ** Previously created numbers are not affected by this change.
     **/

    bigReals.defaults.rMode = 3;

This default rounding mode can be overriden for a particular bigReals irrational upon initialization:

    var bigReals = require('bigreals');

    // Creates a 16 bits precision irrational number with value 4.0 and rounding mode 3.

    var op = bigReals(4.0, 16, 3);

You can get or set the rounding mode of a bigReals irrational number by using the **rMode()** function:

    var bigReals = require('bigreals');
    var op = bigReals(4.0);

    console.log("Rounding mode of op is = " + op.rMode();
    op.rMode(3);
    console.log("New rounding mode of op is = " + op.rMode());

***
Next: [Initialization](#initialization)&nbsp;&nbsp;&nbsp;Previous: [Rounding modes](#rounding)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="initialization">Initialization</a>

A bigReals irrational number can be initialized with a long integer value, a double value or a string and optionally a precision and
rounding mode. In the case of double values, you can use dot notation or floating point notation:

    var bigReals = require('bigreals');
    var op1 = bigReals(4.04324324, 32, 3);
    var op2 = bigReals(655e-2, 24);
    var op3 = bigReals(45050003);
     
    console.log(op1);
    console.log(op2);
    console.log(op3);

In the case of strings, any string representing a number could be provided, the only limit is the memory available in your system. Optionally
a base of the string representation could be provided. To specify a precision and/or rounding mode for the BigReal irrational number, the
base is a mandatory argument:

    var bigReals = require('bigreals');
    var op1 = bigReals("312321785473984758923758943759843759438753984753984753894754398759834759834759", 10, 5000);
    var op2 = bigReals("3f45665bac.45fbae3d", 16);
    var op3 = bigReals("4.5050003", 10, 32, 3);

    console.log(op1);
    console.log(op2);
    console.log(op3);

Irrationals are composed of decimal numbers plus special numbers like *log2*, *Pi*, *Euler*'s and *Catalan*'s constants, so bigReals
supports initialization to any of those constants with an optional precision and rounding mode:

    var bigReals = require('bigreals');
    var pi = bigReals("Pi", 100);
    var cat = bigReals("Catalan");
    var euler = bigReals("Euler");
    var log2 = bigReals("log2);
    
    console.log(pi);
    console.log(cat);
    console.log(euler);
    console.log(log2);
    
***
Next: [Essential arithmetics](#arithmetics)&nbsp;&nbsp;&nbsp;Previous: [Exponentiation, roots and logarithms](#logexp)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="arithmetics">Essential arithmetics</a>

BigReals supports basic arithmetic calculations like addition(function **add**), substracction(function **sub**), multiplication(function **mul**),
and division(function **div**).
All the functions follow the same pattern in relation to arguments allowed, they accept either a double, a long int or another 
bigReal object with an optional precision and rounding mode of the calculation. The result of the calculation is another bigReal
object, so operations can be concatenated:

    var bigReals = require('bigreals');
    var op1 = bigReals(12.32345);
    var op2 = bigReals(5.4353454);
    var op3 = bigReals(234.34);
    
    console.log(op1.add(100));
    console.log(op3.add(0.66));
    console.log(op2.mul(10));
    console.log(op3.div(op2).sub(2.57));
    
***
Next: [Exponentiation, roots and logarithms](#logexp)&nbsp;&nbsp;&nbsp;Previous:[Essential arithmetics](#arithmetics)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="logexp">Exponentation, roots and logarithms</a>

To calculate the n-th power or the n-th root of a bigReal number, functions **pow** and **root** accept as exponent a double, a long integer
(which will be casted to a double internally) or another bigReal number. Optionally a precision and rounding mode of the calculation could be supplied:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.root(3));
    console.log(op2.pow(op3));
    console.log(op2.root(2, 100));
    console.log(op2.root(2).pow(2));

To quickly raise Euler's number, 2 or 10 to the power of a bigReal number, you could use the **e** function and the **exp** functions. **e** will
calculate the power base Euler's constant and **exp** base 2 or 10, as supplied as argument. In both cases, an optional precision and rounding mode
could be provided:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.e());
    console.log(op2.e(100));
    console.log(op2.exp(2));
    console.log(op2.exp(10, op3));

Similarly to **e** and **exp**, **ln** and **log** will calculate the natural/Naiperian logarithm and base 2 or base 10 logarithm of a bigReal number:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.ln());
    console.log(op2.ln(100));
    console.log(op2.log(2));
    console.log(op2.log(10, op3));

Please note that calculation of logarithms could be computationally intensive in some domains involving large numbers.

***
Next: [Trigonometry](#trigonometry)&nbsp;&nbsp;&nbsp;Previous: [Exponentiation, roots and logarithms](#logexp)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="trigonometry">Trigonometry</a>

**cos**, **sin**, **tan**, **sec**, **cosec** and **cotan** functions allow to do trigonometric calculations over a bigReal number. All follow the same
pattern in relation to arguments. If no string is provided as argument a normal trigonometric calculation will be executed. If you want to
calculate the inverse("arc"), the hyperbolic("hyp") or the inverse hyperbolic("invhyp") version of the function, just supply the string as
argument. In all the cases, an optional precision and rounding mode could be provided:
 
    var bigReals = require('bigreals');
    var op1 = bigReals("Pi", 100);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.cos());
    console.log(op2.sec("hyp"));
    console.log(op3.tan(100));
    console.log(op2.("invhyp", 100));

To calculate the arc-tangent2 function, you could use **atan2** specifying as argument a number or a bigReal number and optionally a
precision and rounding mode of the result:

    var bigReals = require('bigreals');
    var op1 = bigReals("Pi", 100);
    
    console.log(op1.atan2());
    console.log(op2.atan2(50));

***
Next: [Integer and remainder functions](#integer)&nbsp;&nbsp;&nbsp;Previous: [Trigonometry](#trigonometry)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="integer">Integer and remainder functions</a>

To round a bigReals irrational number to an integer, **toInt** function with an optional string specifying the type of rounding to be
executed could be used. By default, **toInt** will round to the nearest integer in the direction given by a user supplied rounding
mode. This rounding mode will be the one passed as argument to the function or the one associated with the bigReal number upon
creation or by calls to **precision** function:
 
    var bigReals = require('bigreals');
    var op = bigReals("Pi", 100);
    
    console.log(op.toInt());
    console.log(op.toInt(32, 3));

Integers can be rounded to the next or equal integer("ceil"), to the next lower or equal("floor"), to the nearest integer rounding halfway cases
away from zero("round") or to the next integer towards zero("trunc") by specifying the type of rounding as a string with an optional
precision argument:

    var bigReals = require('bigreals');
    var op1 = bigReals("Pi", 100);
    var op2 = bigreals(23456.535455);

    console.log(op1.toInt("ceil", 32));
    console.log(op1.toInt("floor", 32));
    console.log(op2.toInt("round"));
    console.log(op2.toInt("trunc"));

To calculate the remainder of the division of a bigReal number by a quotient, you could use the **mod** function. By default, **mod**
will calculate the remainder of the division by the number or bigReal number passed as argument rounding towards zero with an optional precision.
If you want to round to the nearest integer, pass the string "remainder" and optionally a precision as argument:

    var bigReals = require('bigreals');
    var op1 = bigReals("Pi", 100);
    var op2 = bigreals(20);

    console.log(op2.mod(3, 32));
    console.log(op2.mod(op1));
    console.log(op2.mod("remainder", 3, 32));
    console.log(op2.mod("remainder", op1));

***
Next: [Number theory](#numbertheory)&nbsp;&nbsp;&nbsp;Previous: [Integer and remainder functions](#integer)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="numbertheory">Number theory</a>

bigReals supports some basic calculation and functions related to number theory like Riemann's Zeta and Gamma functions. In both
functions, an optionally precision and rounding mode could be provided as argument:

    var bigReals = require('bigreals');
    var op1 = bigReals("Pi", 100);
    var op2 = bigreals(20);

    console.log(op1.riemannZ(32));
    console.log(op2.gamma(64, 3));

These functions will be complemented in future versions of bigReals with other related to prime numbers.

***
Next: [Comparison Functions](#comparison)&nbsp;&nbsp;&nbsp;Previous: [Number theory](#numbertheory)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="comparison">Comparison functions</a>

To compare a bigReals number with a normal number or another bigReals number, you could use the function **cmp**, which will return 0 if
both numbers are equal, 1 if the bigReals number is bigger than the operand or -1 otherwise:

    var bigReals = require('bigreals');
    var op1 = bigReals(12.57);
    var op2 = bigReals(12.57);
    var op3 = bigReals(-23456.45678);
    
    console.log(op1.cmp(op2));
    console.log(op2.cmp(op3));
    console.log(op3.cmp(-0.45877));

To check if a bigReal number is not a number("NaN"), infinity("Infinity"), zero("Zero") a regular number("Regular") and integer
("Integer") or an ordinary number("Ordinary"), you can use the **is** function supplying as argument the type to compare with:

    var bigReals = require('bigreals');
    var op1 = bigReals();
    var op2 = bigReals(12.57);
    var op3 = bigReals(0);

    console.log(op1.is("Nan"));
    console.log(op2.is("Number"));
    console.log(op3.is("Ordinary"));
    console.log(op2.toInt().is("Integer"));

***
Next: [Other functions](#other)&nbsp;&nbsp;&nbsp;Previous: [Comparison functions](#comparison)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="other">Other functions</a>

To calculate the factorial or absolute value of a bigReals number, you could use **fac** and **abs** functions. In the case of **fac** function,
an optional precision and rounding mode could be provided and in the case of **abs** and optional rounding mode of the calculation:

    var bigReals = require('bigreals');
    var op1 = bigReals(20);
    var op2 = bigReals(12.57);
    var op3 = bigReals(-23456.45678);
    
    console.log(op1.fac());
    console.log(op2.fac(100));
    console.log(op3.abs());

To print a bigReals number, you could use **toString** and **print** functions. If called with no arguments or with an optional string base, **toString** will
return a string in the specified base(or base 10 if none is provided) representing the bigReals number with the number of digits and precision needed by
biReals to convert back the string exactly to the same bigReal number. If a number of digits is provided as second argument and an optional rounding mode is provided
as third, it will return a string in the specified base with the specified number of most significant digits rounded in the specified roundin mode:

    var bigReals = require('bigreals');

    var op = bigReal(23423423.432432432423423);

    console.log(op.toString());
    console.log(op.toString(16));
    console.log(op.toString(10, 32));

To print to a stream, the **print** function could be used with a similar arguments patterns than **toString** except for a stream to print to as first argument.
If no stream is provided as argument, it will print to standard output:

    var bigReals = require('bigreals');

    var op = bigReal("43dfa.45cba34522", 16));

    op.print();
    op.print(16, 8);

***
Previous: [Other functions](#other)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)
