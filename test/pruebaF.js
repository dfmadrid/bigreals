var bigreals = require('../index');
console.log(bigreals);
var a = bigreals(4.0, 32);
console.log(a);
console.log(a.defaults);
console.log("New precision a = " + a.defaults.precision);
bigreals. defaults.rMode = 0;
console.log("New rMode of a = " + a.defaults.rMode);

var b = bigreals(655e-2, 24);
console.log(b);
console.log(a.add(b));
console.log(b.defaults);
b.defaults.precision = 100;
b.defaults.rMode = 0;
console.log("New Precision of b is = "+ b.defaults.precision);
console.log("New Rounding mode of b = " + b.defaults.rMode);
console.log(b.defaults);

console.log(a);
console.log(b);
//a.precision = 20;
console.log("a = " + a.toString());
console.log("b = " + b.toString());
console.log("Precision of a is = " + a.precision());
console.log("Rounding mode of a is = " + a.rMode());
a.precision(200);
console.log("New precision of a is = " + a.precision());
var c = a.add(b);
console.log("c = a + b = " + c.toString());
var d = a.add(4,1024);
console.log("d = a + 4 = " + d.toString());
console.log("Precision of d = " + d.precision());
var e = d.sub(a);
console.log("e = d - a = " + e.toString());
console.log("e * b = " + e.mul(b).toString());
console.log("b / a = " + b.div(a, 23).toString());
console.log("e ^ 2 = " + e.pow(2).toString());
var f = e.root(3, 1000);
console.log("e ^ 1/3 = " + f.toString());
console.log("Precision of f is = " + f.precision());
console.log("a cmp b = " + a.cmp(b));
console.log("c cmp b = " + c.cmp(b));
console.log("a cmp a = " + a.cmp(a));
a.defaults.precision = 300;
a.defaults.rMode = 0;
var h = bigreals("1234567890abcdefghijklmn", 24);
console.log(h);
var x = bigreals("60509751690538858612029415201127", 10, 112);
console.log(x);
var g = bigreals("123456.123456789012345678901234567890");
console.log(x.mul(g).toString(10, 72));
console.log(g);
g.print();
var n = bigreals(-0.93, 24);
console.log(n);
console.log("abs(n) = " + n.abs().toString(10, 2, 0));
var nan = bigreals(0.3);
console.log(nan + "-" + nan.rMode() + "-" + nan.precision());
console.log("cos n = " + n.root(3, 5000).cos(5000));

var aa = bigreals(1.45, 24);
var bb = bigreals(3.456, 32);
//aa.precision(3);
//bb.precision(4);
console.log("aa = " + aa.toString());
console.log("bb = " + bb.toString());
console.log("aa + bb = " + aa.add(bb));
var pi = bigreals("Pi", 200);
console.log(pi);
console.log(pi.mul(2).cos());
console.log(aa.is("Number"));
console.log(aa.is("Integer"));
