
/*
 * Tests for modular arithmetic operations with integers 
 * and bigReal objects as operands and an integer or BigReal
 * object as a modulus
 */

var assert = require('assert');
var bigReal = require('../index.js');
var op1 = bigReal(10);
var op2 = bigReal(2);
var op3 = bigReal(5);
var op4 = bigReal(25);
var mod = bigReal(3);
var remainder = bigReal(1);

console.log("Modular arithmetic tests:\n");

assert.equal('2', op1.mul(2, 3).toString());
console.log('10 * 2 mod 3 = 2 -- Ok');

assert.equal('2', op1.mul(2, mod).toString());
console.log('10 * 2 mod 3 = 2 -- Ok');

assert.equal('2', op1.div(remainder, op2, mod).toString());
console.log('10 / 2 mod 3 = 2 -- Ok');

assert.equal('0', op1.add(2, 3).toString());
console.log('10 + 2 mod 3 = 0 -- Ok');

assert.equal('0', op1.add(op2, mod).toString());
console.log('10 + 2 mod 3 = 0 -- Ok');

assert.equal('2', op1.sub(2, 3).toString());
console.log('10 - 2 mod 3 = 2 -- Ok');

assert.equal('2', op1.sub(op2, mod).toString());
console.log('10 - 2 mod 3 = 2 -- Ok');

assert.equal('1', op1.pow(2, 3).toString());
console.log('10 ^ 2 mod 3 = 1 -- Ok');

assert.equal('1', op1.pow(op2, mod).toString());
console.log('10 ^ 2 mod 3 = 1 -- Ok');

assert.equal('2', op4.root(2, mod).toString());
console.log('25 ^ 1/2 mod 3 = 2 -- Ok');

assert.equal('2', op4.root(remainder, 2, mod).toString());
console.log('25 ^ 1/2 mod 3 = 2 -- Ok');

assert.equal('1', op1.lShift(2, mod).toString());
console.log('10 << 2 mod 3 = 1 -- Ok');

assert.equal('1', op3.invert(3).mul(op3, 3).toString());
console.log('5 ^ -1 * 5 mod 3 = 1 -- Ok');

assert.equal('1', op3.invert(mod).mul(op3, mod).toString());
console.log('5 ^ -1 * 5 mod 3 = 1 -- Ok');

assert.equal('1', op1.addMul(op2, 3, 3).toString());
console.log('10 + 2 * 3 mod 3 = 1 -- Ok');

assert.equal('1', op1.addMul(op2, 3, mod).toString());
console.log('1 + 2 * 3 mod 3 = 1 -- Ok');

assert.equal('1', op1.subMul(op2, 3, 3).toString());
console.log('1 - 2 * 3 mod 3 = 1 -- Ok');

assert.equal('1', op1.subMul(op2, 3, mod).toString());
console.log('1 - 2 * 3 mod 3 = 1 -- Ok');

assert.equal('2', op1.accMul(op2, 3).toString());
console.log('1 * 2 mod 3 = 2 -- Ok');

assert.equal('1', op1.accMul(op3, mod).toString());
console.log('2 * 5 mod 3 = 1 -- Ok');

console.log("\nModular arithmetic tests -- Ok\n");
