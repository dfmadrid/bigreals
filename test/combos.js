
/*
 * Tests for special operations which are a combination of
 * addition/substraction and multiplication/division.
 * BigReal object op1 accumulates result of previous test.
 */

var assert = require('assert');
var bigReals = require('../index.js');
var bigInteger = bigReals.integers;

var op1 = bigInteger(6);
var op2 = bigInteger(2);
var op3 = bigInteger(3);

console.log("Combined operations tests:\n");

assert.equal('12', op1.addMul(op2, 3).toString());
console.log('6 + 2 * 3 = 12 -- Ok');

assert.equal('18', op1.addMul(op2, op3).toString());
console.log('12 + 2 * 3 = 18 -- Ok');

assert.equal('12', op1.subMul(op2, 3).toString());
console.log('18 - 2 * 3 = 12 -- Ok');

assert.equal('6', op1.subMul(op2, 3).toString());
console.log('12 - 2 * 3 = 6 -- Ok');

assert.equal('12', op1.accMul(op2).toString());
console.log('6 * 2 = 12 -- Ok');

assert.equal('36', op1.accMul(op3).toString());
console.log('12 * 3 = 36 -- Ok');

console.log("\nCombined operations tests -- Ok\n");
