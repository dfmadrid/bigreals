# BigReals Irrationals

BigReals uses [GNU's MPFR](http://www.mpft.org "MPFR") for multi-precision fast calculations over irrationals numbers.
This chapter discusses the special interface of BigReals to MPFR and how to use it.

## Contents

*   [Precision](#precision)
*   [Rounding Mode](#rounding)
*   [Initialization](#initialization)
*   [Basic Arithmetics](#arithmetics)
*   [Exponentiation, roots and logarithms](#logexp)
*   [Equations](#equations)
*   [Other functions](#other)

---

###<a name="precision">Precision</a>

Precision is the of number bits used by BigReals to store internally an irrational number and could be any number between 2 and the maximum unsigned integer value
allowed by your processor. By default, each bigReal number is created with a precision of 53 bits. This value can be changed by modifying the *precision* property of the object *defaults* of the factory/module:

    var bigReals = require('bigreals');

    /* Settles the default precision for new numbers created to 100 bits.
     * Previously created numbers are not affected by this change.
     */

    bigReals.defaults.precision = 100;

This default precision can be overriden in a specific number by specifying a precision upon initialization:

    var bigReals = require('bigreals');

    // Creates a 16 bits precision irrational number with value 4.0

    var op = bigReals(4.0, 16);

To avoid losing precision when executing calculations, bigReals will calculate the result based on an infinite precision and then will round it to the
higher of the operands' precisions:

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

You can get or set the precision of a bigReal number by using the *precision()* function:

    var bigReals = require('bigreals');
    var op = bigReals(4.0);

    console.log("Precision of op is = " + op.precision();
    op.precision(5000);
    console.log("New precision of op is = " + op.precision());

---

###<a name="rounding">Rounding Mode</a>

Rounding mode speficies the way to round the result of a calculation in case rounding is needed. The four allowed modes in BigReals are the same than MPFR ones:

*   0 (MPFR_RNDN: round to nearest)
*   1 (MPFR_RNDZ: round toward zero)
*   2 (MPFR_RNDU: round toward plus infinity)
*   3 (MPFR_RNDD: round toward minus infinity)
*   4 (MPFR_RNDA: round away from zero)

Each BigReals irrational number has associated a rounding mode with it, the default mode is 0 (round to nearest). This default mode can be settled for all
irrational numbers created with bigReals by changing the *rMode* property of the object defaults of the factory/module:

    var bigReals = require('bigreals');

    /* Settles the default rounding mode to round toward minus infinity for new numbers created.
     * Previously created numbers are not affected by this change.
     */

    bigReals.defaults.rMode = 3;

This default rounding mode can be overriden for a particular bigReals irrational upon initialization:

    var bigReals = require('bigreals');

    // Creates a 16 bits precision irrational number with value 4.0 and rounding mode 3.

    var op = bigReals(4.0, 16, 3);

You can get or set the rounding mode of a bigReals irrational number by using the *rMode()* function:

    var bigReals = require('bigreals');
    var op = bigReals(4.0);

    console.log("Rounding mode of op is = " + op.rMode();
    op.rMode(3);
    console.log("New rounding mode of op is = " + op.rMode());

---

###<a name="initialization">Initialization</a>

A bigReals irrational number can be initialized with a long integer value, a double value or a string and optionally a precision and rounding mode. In the case of double values, you can use
dot notation or floating point notation:

    var bigReals = require('bigreals');
    var op1 = bigReals(4.04324324, 32, 3);
    var op2 = bigReals(655e-2, 24);
    var op3 = bigReals(45050003);

    /* Outputs the internal representation of the three bigReals object as the one MPFR would need to represent the
     * object rounding to the nearest.
     */
     
    console.log(op1);
    console.log(op2);
    console.log(op3);

In the case of strings, any string representing a number could be provided, the only limit is the memory available in your system. Optionally a base of the string representation could be
provided. To specify a precision and/or rounding mode for the BigReal irrational number, the base is a mandatory argument:

    var bigReals = require('bigreals');
    var op1 = bigReals("312321785473984758923758943759843759438753984753984753894754398759834759834759", 10, 5000);
    var op2 = bigReals("3f45665bac.45fbae3d", 16);
    var op3 = bigReals("4.5050003", 10, 32, 3);

    /* Outputs the internal representation of the three bigReals object as the one MPFR would need to represent the 
     * object rounding to the nearest.
     */

    console.log(op1);
    console.log(op2);
    console.log(op3);

Irrationals are composed of decimal numbers plus special numbers like Pi, Euler and Catalan constants, so bigReals
supports initialization to any of those constants with an optional precision and rounding mode:

    var bigReals = require('bigreals');
    var pi = bigReals("Pi", 100);
    var cat = bigReals("Catalan");
    var e = bigREals("Euler");
    
    console.log(pi);
    console.log(cat);
    console.log(e);
    
---

###<a name="arithmetics">Basic Arithmetics</a>

BigReals supports basic arithmetic calculations like addition(function *add*), substracction(function *sub*), multiplication(function *mul*), and division(function *div*).
All the functions follow the same pattern in relation to arguments allowed, they accept either a double, a long int or another 
bigReal object with an optional precision and rounding mode of the calculation. The result of the calculation is another bigReal object, so operations can be concatenated:

    var bigReals = require('bigreals');
    var op1 = bigReals(12.32345);
    var op2 = bigReals(5.4353454);
    var op3 = bigReals(234.34);
    
    console.log(op1.add(100));
    console.log(op3.add(0.66));
    console.log(op2.mul(10));
    console.log(op3.div(op2).sub(2.57));
    
---

###<a name="logexp">Exponentation, roots and logarithms</a>

To calculate the n-th power or the n-th root of a bigReal number, functions *pow* and *root* accept as exponent a double, a long integer (which will be casted to a
double internally) or another bigReal number. Optionally a precision and rounding mode of the calculation could be supplied:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.root(3));
    console.log(op2.pow(op3));
    console.log(op2.root(2, 100));
    console.log(op2.root(2).pow(2));

To quickly raise Euler's constant, 2 or 10 to the power of a bigReal number, you could use the *e* function and the *exp* functions. *e* will calculate the power
base Euler's constant and *exp* base 2 or 10, as supplied as argument. In both cases, an optional precision and rounding mode could be provided:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.e());
    console.log(op2.e(100));
    console.log(op2.exp(2));
    console.log(op2.exp(10, op3));

Similarly to *e* and *exp*, *ln* and *log* will calculate the natural/Naiperian logarithm and base 2 or base 10 logarithm of a bigReal number:

    var bigReals = require('bigreals');
    var op1 = bigReals(9);
    var op2 = bigReals(12);
    var op3 = bigReals(3);
    
    console.log(op1.ln());
    console.log(op2.ln(100));
    console.log(op2.log(2));
    console.log(op2.log(10, op3));

Please note that calculation of logarithms could be computatinally intensive in some domains involving large numbers.

---

###<a namef="equations">Equations</a>

---

###<a name="other">Other functions</a>

---
