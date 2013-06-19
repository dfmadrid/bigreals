#BigReals Irrationals

BigReals uses [GNU's MPFR](http://www.mpft.org "MPFR") for infinite precision fast calculations over irrationals numbers.
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

Precision is the of number bits used by BigReals to store internally an irrational number and could be any number between 2 and the maximum unsigned long integer value
allowed by your processor. By default, each bigReal number is created with a precision of 53 bits. This value can be changed by modifying the defaults object of the factory/module:

    var bigReals = require('bigreals');

    /* Settles the default precision for new numbers created to 100 bits.
     * Previously created numbers are not affected by this change.
     */

    bigReals.defaults.precision = 100;

This default configuration can be overriden in a specific number by specifying a precision upon initialization:

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

---

###<a name="rounding">Rounding Mode</a>

---

###<a name="initialization">Initialization</a>

---

###<a name="arithmetics">Basic Arithmetics</a>

---

###<a name="logexp">Exponentation, roots and logarithms</a>

---

###<a namef="equations">Equations</a>

---

###<a name="other">Other functions</a>

---

