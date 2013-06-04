
/*
 * Tests for miscellaneous operations
 */

var assert = require('assert');
var bigReal = require('../index.js');

var op1 = bigReal(15);
var op2 = bigReal(14);
var op3 = bigReal(15);
var op4 = bigReal(16);

console.log("Miscellaneous tests:\n");

assert.equal(false, (op1.isEven() != 0));
console.log('15 is even = false -- Ok');

assert.equal(true, op1.isOdd());
console.log('15 is odd = true -- Ok');

assert.equal(true, (op1.cmp(op2) > 0));
console.log('15 > 14 = true -- Ok');

assert.equal(true, (op1.cmp(op3) == 0));
console.log('15 == 15 = true -- Ok');

assert.equal(true, (op1.cmp(op4) < 0));
console.log('15 < 16 = true -- Ok');

console.log("\nMiscellaneous tests -- Ok\n");
