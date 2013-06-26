
// Hash table creation and lookup using special loop-optimized functions:

    var bigInteger = require('bigreals').integers;
    
    var op1 = bigInteger(20);
    var op2 = bigInteger(30);
    var op3 = bigInteger("500");
    var hashTable = {};
    var cont = 0;

// Base16 Hash table creation

    while(cont < 100){
     hashTable[op1.accMul(3).toString(16)] = op2.sub(op3).toString();    // op1.accMul(3) -> op1 = op1 * 3;
     cont++;
     }

    cont = 0;
    var value = "21d20f70b1835f3560511394";

// Hash value lookup in table

    while(cont < 100){
     if(hashTable[value]){
      console.log(hashTable[value]);                // Value of hash 21d20f70b1835f3560511394 is -470
      cont = 100;
  }
      cont++;
    }
