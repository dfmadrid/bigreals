
/*
 * Doesn't follow the Heavy JS - Thin C/C++ Layer pattern to
 * improve performance.
 * TODO: instantiate a gmp big integer, a mpfr big fraction or
 * a mpfr big floating point depending on user supplied input
*/

var bigInteger = require("./build/Release/bigIntegers");
var bigReal = require("./build/Release/bigReals");

module.exports = bigReal;
module.exports.integer = bigInteger;
module.exports.defaults = bigReal().defaults;

