
/*
 * Tests for normal arithmetic operations with integers 
 * and bigReal objects as operands
 */

var assert = require('assert');
var bigReal = require('../index.js');

var op1 = bigReal(6);
var op2 = bigReal(2);
var op3 = bigReal(9);
var remainder = bigReal(1);

var getRootRem = function(base, root){
	base.root(remainder, root);
	return remainder;
	}

var getDivRem = function(dividend, remainder, divisor){
	dividend.div(remainder, divisor);
	return remainder;
	}
	
console.log("Arithmetic tests:\n");

assert.equal('12', op1.mul(2).toString());
console.log('6 * 2 = 12 -- Ok');

assert.equal('12', op1.mul(op2).toString());
console.log('6 * 2 = 12 -- Ok');

assert.equal('3', op1.div(2).toString());
console.log('6 / 2 = 3 -- Ok');

assert.equal('3', op1.div(op2).toString());
console.log('6 / 2 = 3 -- Ok');

assert.equal('9', op2.mul(op3.div(op2)).add(getDivRem(op3, remainder, op2)).toString());
console.log('1 + 2 * 4 = 9 -- Ok');

assert.equal('9', op2.mul(op3.div(2)).add(getDivRem(op3, remainder, 2)).toString());
console.log('1 + 2 * 4 = 9 -- Ok');

assert.equal('8', op1.add(2).toString());
console.log('6 + 2 = 8 -- Ok');

assert.equal('8', op1.add(op2).toString());
console.log('6 + 2 = 8 -- Ok');

assert.equal('4', op1.sub(2).toString());
console.log('6 - 2 = 4 -- Ok');

assert.equal('4', op1.sub(op2).toString());
console.log('6 - 2 = 4 -- Ok');

assert.equal('36', op1.pow(2).toString());
console.log('6 ^ 2 = 36 -- Ok');

assert.equal('3', op3.root(2).toString());
console.log('9 ^ 1/2 = 3 -- Ok');

assert.equal('6', op1.root(2).pow(2).add(getRootRem(op1, 2)).toString());
console.log('2 + (6 ^ 1/2) ^ 2 = 6 -- Ok');

assert.equal('24', op1.lShift(2).toString());
console.log('6 << 2 = 24 -- Ok');

assert.equal('1', op1.rShift(2).toString());
console.log('6 >> 2 = 1 -- Ok');

console.log("\nArithmetic tests -- Ok\n");

