
/*
 * Doesn't follow the Heavy JS - Thin C/C++ Layer pattern to
 * improve performance.
 * TODO: instantiate a gmp big integer, a mpfr big fraction or
 * a mpfr big floating point depending on user supplied input
*/

var bigIntegers = require("./build/Release/bigIntegers");
var bigReals = require("./build/Release/bigReals");

module.exports = bigReals;
module.exports.integers = bigIntegers;
module.exports.defaults = bigReals().defaults;

