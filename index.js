
/*
 * Doesn't follow the Heavy JS - Thin C/C++ Layer pattern to
 * improve performance.
 * TODO: instantiate a gmp big integer, a mpfr big fraction or
 * a mpfr big floating point depending on user supplied input
*/

var bigInteger = require("./build/Release/bigInteger");

module.exports = function(value, value2, value3){

	return bigInteger(value, value2, value3);

};
