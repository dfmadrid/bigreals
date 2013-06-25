
/*
 * Doesn't follow the Heavy JS - Thin C/C++ Layer pattern to
 * improve performance.
*/

var bigIntegers = require("./build/Release/bigIntegers");
var bigReals = require("./build/Release/bigReals");

module.exports = bigReals;
module.exports.integers = bigIntegers;
module.exports.defaults = bigReals().defaults;

