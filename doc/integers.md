# BigReals Integers

BigReals uses [GNU's GMP](http://www.gmp.org "GMP") for infinite-precision fast calculations over integers numbers.
This chapter discusses the special interface of BigReals to GMP and how to use it.

## <a name="contents">Contents</a>

*   [Initialization](#initialization)
*   [Modular arithmetics](#modular)
*   [Essential arithmetics](#arithmetics)
*   [Logical and bit-wise functions](#logical)
*   [Comparison functions](#comparison)
*   [Combo functions](#combos)
*   [Other functions](#other)

***
Next: [Initialization](#initialization)&nbsp;&nbsp;&nbsp;Previous: [Contents](#top)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)

###<a name="initialization">Initialization</a>

To create a bigReals integer number, require the bigReals integers factory and provide as argument a long integer or a string in an optional base
representing the bigReals integer number. 

    var bigInteger = require('bigreals').integers;

    var op1 = bigInteger(23454543534);
    var op2 = bigInteger("323423432423435345345453455369565860948598649586094856908459608459864095864095864905594864985");
    var op3 = bigInteger("3f4d532a4cba56455b64ff", 16);

    console.log(op1);
    console.log(op2);
    console.log(op3);

In the case of strings supplied as argument, a default base 10 is assumed unless otherwise provided. If factory is called with no
arguments, the bigReals integer number will initialized with the value 0.

***
Next: [Modular arithmetics](#modular)&nbsp;&nbsp;&nbsp;Previous: [Initialization](#initialization)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="modular">Modular arithmetics</a>

For fast and easy modular arithmetics, all the functions involving calculations accept an optional argument to be treated as the
modulus of the calculation. The modulus can be a long integer or another bigReals integer:

    var bigInteger = require('bigreals').integers;

    var op = bigInteger(234);
    var modulus = bigInteger(5);

    console.log(op.add(50, 5));
    console.log(op.add(50, modulus));

All the calculations return another bigReals integer as a result, so operations can be chained:


    var bigInteger = require('bigreals').integers;

    var op1 = bigInteger(234);
    var op2 = bigInteger(45);
    var modulus = bigInteger(5);

    console.log(op1.add(50, 5).mul(op2, modulus).pow(5));

***
Next: [Essential arithmetics](#arithmetics)&nbsp;&nbsp;&nbsp;Previous: [Modular arithmetics](#modular)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="arithmetics">Essential arithmetics</a>

BigReals integers allows for basic arithmetics such as addition(**add** function), substraction(**sub** function), multiplication(**mul** function) and
exponentiation(**pow**). All the functions follow the same pattern, a long integer or another bigReal integer can be supplied as arguments together
with an optional modulus to convert the calculation into a modular one. Modulus can be a long integer or another bigReals integer:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(240560);
    var op2 = bigInteger(50000343433);
    var mod = bigInteger(50);
     
    console.log(op1.add(20));
    console.log(op1.mul(op2, mod));
    console.log(op3.pow(100);
    console.log(op3.pow(op1, 5));

Besides that, bigReals integers support calculations like n-th roots and divisions that can raise a non-integer value through **root** and **div** functions.
If the first argument passed is a long integer, a n-th root or division will be calculated rounding result to nearest or equal integer(ceil). In case is
a bigReals integer, it will be used to store the remainder of the calculation. In both cases, an optional long integer or bigReals integer last argument
taken as modulus could be supplyind to execute a modular calculation.
    
    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
    var mod = bigInteger(5);
    var remainder = bigInteger();
 
    console.log(op1.root(6));
    console.log(op1.root(6, mod));
    console.log(op2.div(remainder, 3));
    console.log(op2.div(op1, 5));

    
***
Next: [Logical and bit-wise functions](#logical)&nbsp;&nbsp;&nbsp;Previous: [Essential arithmetics](#arithmetics)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="logical">Logical and bit-wise functions</a>

BigReals integers aims at providing a powerful interface for dealing with bit/byte calculations. Basic logical calculations like xor, AND, OR are supported
through **exor**, **and** and **or** functions. All those functions could be passed a long integer or another bigReals integer as operand:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
    var op3 = bigInteger(5);
 
    console.log(op1.exor(6));
    console.log(op1.and(op2));
    console.log(op1.or(8));

Besides that, logical left shift and right shift calculations can be executed by calling **lshift** and **rshift** functions with a long integer or bigReals
integer operand representing the number of positions/powers of 2 to shift. Optionally, a long integer or bigReals integer could be passed as modulus as
in fact, those operations are a multiplication and a division by a power of 2 respectively:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
    var op3 = bigInteger(5);
 
    console.log(op1.lshift(6));
    console.log(op1.lshift(op3));
    console.log(op2.rshift(3));

To calculate the Hamming distance between a bigReal integer and a long integer or another bigReal integer, you can use the **hammingDist** function, which
will return the number of bit positions with different values of the two operands:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
 
    console.log(op1.hammingDist(6));
    console.log(op1.hammingDist(op2));

To obtain the number of bits sets to 1 in a bigReal integer, you can use the **population** function called with no arguments:


    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
 
    console.log(op1.population());

In the next version of bigReals, logical functions will be extended to allow the user to optionally operate only on a specific bit or byte.
    
***
Next: [Comparison functions](#comparison)&nbsp;&nbsp;&nbsp;Previous:[Logical and bit-wise functions](#logical)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="comparison">Comparison functions</a>

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


***
Previous: [Other functions](#other)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)&nbsp;&nbsp;&nbsp;Up: [BigReals manual](manual.md)
