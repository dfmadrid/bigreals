#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION 
#endif

#ifndef MPFR_USE_INTMAX_T
#define MPFR_USE_INTMAX_T

#include <node.h>
#include <gmp.h>
#include <mpfr.h>
#include "bigFloat.h"

using namespace v8;

bigFloat::bigFloat() {};
bigFloat::~bigFloat(){
  mpfr_clear(*mpFloat_);
  delete(mpFloat_);
};

Persistent<Function> bigFloat::constructor;

void bigFloat::Init() {

  // Constructor template

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("bigFloat"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Javascript Prototype of object

  tpl->PrototypeTemplate()->Set(String::NewSymbol("inspect"),
    FunctionTemplate::New(inspect)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
    FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("add"),
    FunctionTemplate::New(add)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("sub"),
    FunctionTemplate::New(sub)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mul"),
    FunctionTemplate::New(mul)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addMul"),
    FunctionTemplate::New(addMul)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("accMul"),
    FunctionTemplate::New(accMul)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("subMul"),
    FunctionTemplate::New(subMul)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("xor"),
    FunctionTemplate::New(exor)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("and"),
    FunctionTemplate::New(bitAnd)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("or"),
    FunctionTemplate::New(bitOr)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("distance"),
    FunctionTemplate::New(hamDist)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("population"),
    FunctionTemplate::New(population)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("lShift"),
    FunctionTemplate::New(lShift)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("rShift"),
    FunctionTemplate::New(rShift)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("isEven"),
    FunctionTemplate::New(isEven)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("isOdd"),
    FunctionTemplate::New(isOdd)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("cmp"),
    FunctionTemplate::New(cmp)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("pow"),
    FunctionTemplate::New(pow)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("root"),
    FunctionTemplate::New(root)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("invert"),
    FunctionTemplate::New(invert)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("div"),
    FunctionTemplate::New(div)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());

}

/* Constructor.
 * Accepts a uint64 number, a string in a provided base or a GMP integer.
 * If no argument is provided, instantiates the object to a biginteger with value 0.
 */

Handle<Value> bigFloat::New(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = new bigFloat();
  obj->mpFloat_ = new mpfr_t[1];
  mpfr_init(*obj->mpFloat_);
  mpfr_prec_t precision = 53;
  mpfr_rnd_t rMode = MPFR_RNDD;
  int base = 10;

  if(args[0]->IsUndefined()){
    mpfr_init(*obj->mpFloat_);
  }
  else if(args[0]->IsNumber()){
    if(args.Length() > 1 && args[1]->IsUint32()){
      precision = args[1]->ToUint32()->Value();
    }
    mpfr_init2(*obj->mpFloat_, precision);
    mpfr_set_d(*obj->mpFloat_, args[0]->ToNumber()->Value());
  }	
  else if(args[0]->IsString()){
    if(args.Length() > 1 && args[1]->IsNumber()){
      base = args[1]->ToInt32()->Value();
      if(base < 2 || base > 62) {
        ThrowException(Exception::Error(String::New("Base two must be >= 2 and <= 62. If empty, default is 10")));
        return scope.Close(Undefined());
      }
    }
    if(args.Length() > 2 && args[2]->IsUint32()){
      precision = args[2]->ToUint32()->Value();
    }
    if(args.Length() > 3 && args[3]->IsUint32()){
      rMode = args[3]->ToUint32()->Value();
    }
  mpft_init2(*obj->mpFLoat_, precision);
  String::Utf8Value str(args[0]->ToString());
  mpfr_set_str(*obj->mpFloat_, *str, base, rMode);
  }
  else if(args[0]->IsExternal()){
    obj->mpFloat_ = (mpfr_t *) External::Unwrap(args[0]);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a number or a string.")));
    return scope.Close(Undefined());
  }

  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> bigFloat::NewInstance(const Arguments& args) {
  HandleScope scope;
  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

// Returns a string when object is inspected by console.log().

Handle<Value> bigFloat::inspect(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  Local<String> bigFloatValue = String::New(mpfr_get_str(0, 10, *obj->mpFloat_, MPFR_RNDD));
  Local<String> type = String::Concat(String::New("<bigFloat:"), bigFloatValue);

  return scope.Close(String::Concat(type, String::New(">")));
}

/* Returns a string representing the bigFloateger.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::toString(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  int base = 10;
  
  if(args[0]->IsInt32()){
    base = args[0]->ToInt32()->Value();
    if(base < 2 || base > 62) {
      ThrowException(Exception::Error(String::New("Base must be >= 2 and <= 62. If empty, default is 10")));
      return scope.Close(Undefined());
    }
  }
  else if(args.Length() > 0){
    ThrowException(Exception::TypeError(String::New("Base must be a positive integer")));
    return scope.Close(Undefined()); 
  }

  return scope.Close(String::New(mpfr_get_str(0, base, *obj->mpFloat_)));
}


/* Addition, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::add(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_init(*res);
  
  if(args[0]->IsNumber()){
    mpfr_add_ui(*res, *obj->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_add(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be an integer or a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Substraction, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular substraction.
 */

Handle<Value> bigFloat::sub(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_init(*res);
  
  if(args[0]->IsNumber()){
    mpfr_sub_ui(*res, *obj->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_sub(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must an integer or a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Multiplication, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular multiplication.
 */

Handle<Value> bigFloat::mul(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_init(*res);

  if(args[0]->IsNumber()){
    mpfr_mul_ui(*res, *obj->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_mul(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must an integer or a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Combined operation equivalent to op1 = op1 + op2 * op3.
 * Accepts as main argument a biginteger object and a second argument that
 * might be a uint64 or a biginteger object. Optinonally accepts a
 * a uint64 or biginteger object modulus for modular addition.
 */

Handle<Value> bigFloat::addMul(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  if(args.Length() > 1 && args[0]->IsObject()){
    if(args[1]->IsObject()){
      bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
      bigFloat* obj3 = ObjectWrap::Unwrap<bigFloat>(args[1]->ToObject());
      mpfr_addmul(*obj->mpFloat_, *obj2->mpFloat_, *obj3->mpFloat_);
    }
    else if(args[1]->IsNumber()){
      bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
      mpfr_addmul_ui(*obj->mpFloat_, *obj2->mpFloat_, args[1]->ToInteger()->Value());
    }
    else{
      ThrowException(Exception::TypeError(String::New("Argument 2 must be a bigint or an integer")));
      return scope.Close(Undefined());
    }
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 2){
    obj->mpFloat_ = modRes(obj->mpFloat_, args[2]); 
  }

  return scope.Close(args.This());
}

/* Combined operation equivalent to op1 = op1 * op2.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger object modulus for modular multiplication.
 */

Handle<Value> bigFloat::accMul(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_init(*res);

  if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_mul(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else if(args[0]->IsNumber()){
    mpfr_mul_ui(*res, *obj->mpFloat_, args[0]->ToInteger()->Value());
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must an integer or a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    obj->mpFloat_ = modRes(res, args[1]); 
  }
  else{
    obj->mpFloat_ = res;
  }

  return scope.Close(args.This());
}

/* Combined operation equivalent to op1 = op1 - op2 * op3.
 * Accepts as main argument a biginteger object and a second
 * argument that might be a uint64 or a biginteger object.
 * Optionally accepts a uint64 or biginteger object modulus for modular substraction.
 */

Handle<Value> bigFloat::subMul(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  if(args.Length() > 1 && args[0]->IsObject()){
    if(args[1]->IsNumber()){
      bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
      mpfr_submul_ui(*obj->mpFloat_, *obj2->mpFloat_, args[1]->ToInteger()->Value());
    }	
    else if(args[1]->IsObject()){
      bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
      bigFloat* obj3 = ObjectWrap::Unwrap<bigFloat>(args[1]->ToObject());
      mpfr_submul(*obj->mpFloat_, *obj2->mpFloat_, *obj3->mpFloat_);
    }	
    else{
      ThrowException(Exception::TypeError(String::New("Argument 2 must be an integer or a bigint")));
      return scope.Close(Undefined());
    }
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 2){
    obj->mpFloat_ = modRes(obj->mpFloat_, args[2]); 
  }

  return scope.Close(args.This());
}

/* Exclusive XOR.
 * Accepts as main argument a biginteger object and
 * optionally a uint64 or biginteger modulus for modular XOR
 */

Handle<Value> bigFloat::exor(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_xor(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Multiplication by 2 ^ n aka Left Shift.
 * Accepts as main argument a uint64 and
 * optionally a uint64 or biginteger modulus for modular multiplication.
 */

Handle<Value> bigFloat::lShift(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsNumber()){
    mpfr_mul_2exp(*res, *obj->mpFloat_, args[0]->ToNumber()->Value());
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be an integer")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Division by 2 ^ n aka Right Shift.
 * Accepts as main argument a uint64 and
 * optionally a uint64 or biginteger modulus for modular division.
 */

Handle<Value> bigFloat::rShift(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsNumber()){
    mpfr_tdiv_q_2exp(*res, *obj->mpFloat_, args[0]->ToNumber()->Value());
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be an integer")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Bit AND operation.
 * Accepts as main argument a biginteger object and
 * optionally a uint64 or biginteger modulus for modular result
 */

Handle<Value> bigFloat::bitAnd(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_and(*res, *obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Inclusive OR.
 * Accepts as main argument a biginteger object and
 * optionally a uint64 or biginteger modulus.
 */

Handle<Value> bigFloat::bitOr(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_ior(*res, *obj->mpFloat_, *obj2->mpFloat_);

  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Hamming distance between 2 biginteger objects.
 * Accepts as main argument a biginteger object to compare with.
 * Returns the number of bit positions in the 2 biginteger objects with a different value
 */

Handle<Value> bigFloat::hamDist(const Arguments& args) {
  HandleScope scope;
  unsigned long int distance = 0;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    distance = mpfr_hamdist(*obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  return scope.Close(Number::New(distance));
}

/* Population of biginteger object bits.
 * Called with no arguments.
 * Returns number of bits set to 1 in object
 */

Handle<Value> bigFloat::population(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  unsigned long int population = mpfr_popcount(*obj->mpFloat_);

  return scope.Close(Number::New(population));
}

/* Compares 2 biginteger objects.
 * Accepts as main argument a biginteger object.
 * Returns > 0 if argument is smaller than original object, 0 if equal or <0 if bigger.
 */

Handle<Value> bigFloat::cmp(const Arguments& args) {
  HandleScope scope;
  int  result = 0;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    result = mpfr_cmp(*obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
    return scope.Close(Undefined());
  }

  return scope.Close(Number::New(result));
}

/* Exponentiation, normal or modular.
 * Accepts as exponent a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus.
 */

Handle<Value> bigFloat::pow(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *base = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args[0]->IsNumber()){
    mpfr_pow_ui(*res, *base->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args.Length() > 1 && args[0]->IsObject() && args[1]->IsObject()){
    bigFloat* exponent = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    bigFloat* modulus = ObjectWrap::Unwrap<bigFloat>(args[1]->ToObject());
    mpfr_powm(*res, *base->mpFloat_, *exponent->mpFloat_, *modulus->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Arguments 1 and 2 must be two bigints or an integer and a bigint"))); 
    return scope.Close(Undefined());
  }

  if(args.Length() > 1 && args[1]->IsNumber()){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else{		
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Returns cero if biginteger object is even */

Handle<Value> bigFloat::isEven(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  int isEven = mpfr_even_p(*obj->mpFloat_);

  return scope.Close(Number::New(isEven));
}

/* Returns positive if biginteger object is odd */

Handle<Value> bigFloat::isOdd(const Arguments& args) {
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());

  int isOdd = mpfr_odd_p(*obj->mpFloat_);

  return scope.Close(Number::New(isOdd));
}

/* n-th root with or without remainder,  normal or modular.
 * Accepts as exponent a uint64 and optionally a modulus.
 * If first argument is a biginteger object, it will be used to store the n-th remainder of root.
 * Optionally accepts a uint64 or biginteger object for modular n-th roots.
 */

Handle<Value> bigFloat::root(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *base = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_init(*res);

  if(args[0]->IsNumber()){
    mpfr_root(*res, *base->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args.Length() > 1 && args[0]->IsObject() && args[1]->IsNumber()){
    bigFloat* remainder = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_rootrem(*res, *remainder->mpFloat_, *base->mpFloat_, args[1]->ToInteger()->Value());
  }
  else{
    ThrowException(Exception::TypeError(String::New("Arguments 1 and 2 must be an integer and a bigint or a bigint and an integer")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 1 && args[0]->IsNumber()){
    Handle<Value> arg[1] = { External::New(modRes(res, args[1])) };
    result = constructor->NewInstance(1, arg);
  }
  else if(args.Length() > 2){		
    Handle<Value> arg[1] = { External::New(modRes(res, args[2])) };		
    result = constructor->NewInstance(1, arg);
  }	
  else{
    Handle<Value> arg[1] = { External::New(*res) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Modular inversion
 * Accepts a uint64 or a biginteger modulus.
 */

Handle<Value> bigFloat::invert(const Arguments& args) {
    HandleScope scope;
  bigFloat *base = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];	
  mpfr_init(*res);

  if(args.Length() > 0 && args[0]->IsObject()){
    bigFloat* modulus = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_invert(*res, *base->mpFloat_, *modulus->mpFloat_);
  }
  else if(args.Length() >0 && args[0]->IsNumber()){
    mpfr_t *modulus = new mpfr_t[1];
    mpfr_init(*modulus);
    mpfr_set_ui(*modulus, args[0]->ToInteger()->Value());
    mpfr_invert(*res, *base->mpFloat_, *modulus);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument must be an integer or a bigint"))); 
    return scope.Close(Undefined());
  }	

  Handle<Value> arg[1] = { External::New(*res) };				
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Division with or without remainder, normal or modular.
 * Accepts as divisor a uint64 and optionally a modulus.
 * If first argument is a biginteger object, it will be used to store remainder of calculation
 * and second argument must be a uint64 or a biginteger object divisor. 
 * Optionally a uint64 or biginteger modulus is accepted for modular division.
 */

Handle<Value> bigFloat::div(const Arguments& args) {
  HandleScope scope;
  Local<Object> result;
  bigFloat *dividend = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * quotient = new mpfr_t[1];	
  mpfr_init(*quotient);

  if(args.Length() == 1 && args[0]->IsNumber()){
    mpfr_div_ui(*quotient, *dividend->mpFloat_, args[0]->ToInteger()->Value());
  }
  else if(args.Length() == 1 && args[0]->IsObject()){
    bigFloat* divisor = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_div(*quotient, *dividend->mpFloat_, *divisor->mpFloat_);
  }
  else if(args.Length() > 1 && args[0]->IsObject()){
    bigFloat* remainder = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(args[1]->IsObject()){
      bigFloat *divisor = ObjectWrap::Unwrap<bigFloat>(args[1]->ToObject());
      mpfr_tdiv_qr(*quotient, *remainder->mpFloat_, *dividend->mpFloat_, *divisor->mpFloat_);
    }
    else if(args[1]->IsNumber()){
      mpfr_tdiv_qr_ui(*quotient, *remainder->mpFloat_, *dividend->mpFloat_, args[1]->ToInteger()->Value());
    }
    else{
      ThrowException(Exception::TypeError(String::New("Argument 2 must be an integer or a bigint")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be an integer or a bigint followed by a bigint")));
    return scope.Close(Undefined());
  }

  if(args.Length() > 2){	
    Handle<Value> arg[1] = { External::New(modRes(quotient, args[2])) };
    result = constructor->NewInstance(1, arg);
  }
  else{
    Handle<Value> arg[1] = { External::New(*quotient) };		
    result = constructor->NewInstance(1, arg);
  }	

  return scope.Close(result);
}

/* Private function to calculate modular result of a previous operation
 * Accepts a uint64 or a biginteger modulus.
 * Returns result of calculation mod modulus as a GMP integer.
 */

mpfr_t * bigFloat::modRes(mpfr_t * res, Local<Value> modulus){

  mpfr_t * resMod = new mpfr_t[1];	
  mpfr_init(*resMod);
  if(modulus->IsObject()){
    bigFloat* modObj = ObjectWrap::Unwrap<bigFloat>(modulus->ToObject());
    mpfr_mod(*resMod, *res, *modObj->mpFloat_); 
  }
  else if(modulus->IsNumber()){
    mpfr_mod_ui(*resMod, *res, modulus->ToInteger()->Value());
  }
  else{
    ThrowException(Exception::TypeError(String::New("Modulus must be an integer or a bigint")));
  }

  return resMod;
}

    
