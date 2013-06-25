
/*
 * Tests for initialization of bigInteger objects with integers, 
 * strings and strings with a base
 */

var assert = require('assert');
var bigReals = require('../index.js');
var bigInteger = bigReals.integers;

var largeNumber = "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171";

console.log("Initialization tests:\n");

assert.equal('0', bigInteger().toString());
console.log('0 = ' + bigInteger(0) + " -- Ok");
  
assert.equal('3', bigInteger(3).toString());
console.log('3 = ' + bigInteger(3) + " -- Ok");


assert.equal('3', bigInteger("3").toString());
console.log('3 = ' + bigInteger("3") + " -- Ok");

assert.equal('3', bigInteger("3", 16).toString());
console.log('3 = ' + bigInteger("3") + " -- Ok");

assert.equal(largeNumber, bigInteger(largeNumber).toString());
console.log(largeNumber + " = " + bigInteger(largeNumber) + " -- Ok");

console.log("\nInitialization tests -- Ok\n");
