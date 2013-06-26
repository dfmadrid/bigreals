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



***
Next: [Essential arithmetics](#arithmetics)&nbsp;&nbsp;&nbsp;Previous: [Modular arithmetics](#modular)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="arithmetics">Essential arithmetics</a>

BigReals integers allows for basic arithmetics such as addition(function **add**), substraction(function **sub**), multiplication(function **mul**) and
exponentiation(function **pow**). All the functions follow the same pattern, a long integer or another bigReal integer can be supplied as arguments together
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
taken as modulus could be supplying to execute a modular calculation.
    
    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
    var mod = bigInteger(5);
    var remainder = bigInteger();
 
    console.log(op1.root(6));
    console.log(op1.root(6, mod));
    console.log(op2.div(remainder, 3));
    console.log(op2.div(op1, 5));

To calculate the inverse of a bigReal number, the **invert** function is used, called with an optional long integer or bigReal integer as modulus:

    var bigInteger = require('bigreals').integers;

    var op = bigInteger(50);
    var mod = bigInteger(3);

    console.log(op.invert());
    console.log(op.invert(mod));

All the calculations return another bigReals integer as a result, so operations can be chained:

    var bigInteger = require('bigreals').integers;

    var op1 = bigInteger(234);
    var op2 = bigInteger(45);
    var modulus = bigInteger(5);

    console.log(op1.add(50, 5).mul(op2, modulus).pow(5));
    
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

To calculate the Hamming distance between a bigReal integer and a long integer or another bigReal integer, you can use the **hamDist** function, which
will return the number of bit positions with different values of the two operands:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
    var op2 = bigInteger(50);
 
    console.log(op1.hamDist(6));
    console.log(op1.hamDist(op2));

To obtain the number of bits sets to 1 in a bigReal integer, you can use the **population** function called with no arguments:


    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(36);
 
    console.log(op1.population());

In the next version of bigReals, logical functions will be extended to allow the user to optionally operate only on a specific bit or byte.
    
***
Next: [Comparison functions](#comparison)&nbsp;&nbsp;&nbsp;Previous:[Logical and bit-wise functions](#logical)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="comparison">Comparison functions</a>

A bigReals integer could be compared with a long integer or another bigReals integer with the **cmp** function, which will return 0 if both numbers
are equal, >0 if the argument is smaller or <0 if bigger:

    var bigReals = require('bigreals').integers;

    var op1 = bigReals(9);
    var op2 = bigReals(12);
    
    console.log(op1.cmp(9));
    console.log(op2.cmp(op1));
    console.log(op1.cmp(op2);

***
Next: [Combo functions](#combos)&nbsp;&nbsp;&nbsp;Previous:[Comparison functions](#comparison)&nbsp;&nbsp;&nbsp;Top: [Contents](#contents)

###<a name="combos">Combo functions</a>

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
