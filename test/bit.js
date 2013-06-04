
/*
 * Tests for logical and bit related operations
 */

var assert = require('assert');
var bigReal = require('../index.js');

var op1 = bigReal(15);
var op2 = bigReal(14);

console.log("Logical and bit manipulation tests:\n");

assert.equal('1', op1.xor(op2).toString());
console.log('15 xor 14 = 1 -- Ok');

assert.equal('60', op1.lShift(2).toString());
console.log('15 << 2 = 60 -- Ok');

assert.equal('3', op1.rShift(2).toString());
console.log('15 >> 2 = 3 -- Ok');

assert.equal('14', op1.and(op2).toString());
console.log('15 and 14 = 1 -- Ok');

assert.equal('15', op1.or(op2).toString());
console.log('15 or 14 = 15 -- Ok');

assert.equal(1, op1.distance(op2));
console.log('Hamming distance between 15 and 14 = 1 -- Ok');

assert.equal(4, op1.population());
console.log('Bits set to 1 in 15 = 4 -- Ok');

console.log("\nLogical and bit manipulation tests -- Ok\n");
