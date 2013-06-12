#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION 
#endif

#ifndef MPFR_USE_INTMAX_T
#define MPFR_USE_INTMAX_T
#endif

#include <node.h>
#include <iostream>
#include <string>
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
  
  // Instance Vars
  
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  Local<ObjectTemplate> configTpl = ObjectTemplate::New();
  configTpl->SetInternalFieldCount(1);
  configTpl->SetAccessor(String::New("precision"), getPrecision, setPrecision);
  configTpl->SetAccessor(String::New("rMode"), getRmode, setRmode);
  
  config *defaults = new config(53, 0);
  Handle<Object> defaultConf = configTpl->NewInstance();
  defaultConf->SetInternalField(0, External::New(defaults));

  tpl->InstanceTemplate()->Set(String::NewSymbol("defaults"), defaultConf);

  // Javascript Prototype of object

  tpl->PrototypeTemplate()->Set(String::NewSymbol("inspect"),
    FunctionTemplate::New(inspect)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
    FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("precision"),
    FunctionTemplate::New(precision)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("rMode"),
    FunctionTemplate::New(rMode)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("add"),
    FunctionTemplate::New(add)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("sub"),
    FunctionTemplate::New(sub)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mul"),
    FunctionTemplate::New(mul)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("div"),
    FunctionTemplate::New(div)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("pow"),
    FunctionTemplate::New(pow)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("root"),
    FunctionTemplate::New(root)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("cmp"),
    FunctionTemplate::New(cmp)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("abs"),
    FunctionTemplate::New(abs)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());

}

Handle<Value> bigFloat::getPrecision(Local<String> property, const AccessorInfo &info) {

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
        long int value = static_cast<config*>(ptr)->precision_;	
    return Integer::New(value);
}
  
void bigFloat::setPrecision(Local<String> property, Local<Value> value, const AccessorInfo& info) {

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
        static_cast<config*>(ptr)->precision_ = value->IntegerValue();
  }

Handle<Value> bigFloat::getRmode(Local<String> property, const AccessorInfo &info) {

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));

    void* ptr = wrap->Value();
        int value = static_cast<config*>(ptr)->rMode_;
    return Integer::New(value);
}
  
void bigFloat::setRmode(Local<String> property, Local<Value> value, const AccessorInfo& info) {

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
        static_cast<config*>(ptr)->rMode_ = value->Int32Value();
  }

Handle<Value> bigFloat::getOpPrecision(Handle<Value> precHandle, Handle<Number> precision){

  HandleScope scope;

  if(precHandle->IsNumber()){
    return scope.Close(Number::New(precHandle->ToInteger()->Value()));
  }
  else{
    ThrowException(Exception::TypeError(String::New("Precision must be a long integer")));
    return scope.Close(Undefined());
  }
}

Handle<Value> bigFloat::getOpRmode(Handle<Value> rModeHandle, Handle<Integer> rMode){

  HandleScope scope;

  if(rModeHandle->IsUint32()){
    rMode = rModeHandle->ToUint32();

    if(rMode->Value() > 4){
      ThrowException(Exception::TypeError(String::New("Rounding mode must be either 0, 1, 2, 3, or 4")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Rounding mode must be a positive integer")));
    return scope.Close(Undefined());
  }

  return scope.Close(Uint32::New(rMode->Value()));

}

/* Constructor.
 * Accepts a uint64 number, a string in a provided base or a GMP integer.
 * If no argument is provided, instantiates the object to a biginteger with value 0.
 */

Handle<Value> bigFloat::New(const Arguments& args){
  HandleScope scope;
  
  bigFloat *obj = new bigFloat();
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  obj->precision_ = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  obj->rMode_ = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
  obj->mpFloat_ = new mpfr_t[1];
  int base = 10;
  
  if(args[0]->IsExternal()){
    obj->mpFloat_ = (mpfr_t *) External::Unwrap(args[0]);
  }
  else if(args[0]->IsUndefined()){
    mpfr_init2(*obj->mpFloat_, obj->precision_);
  }
  else if(args[0]->IsNumber()){
    if(args.Length() > 1){
      obj->precision_ = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) obj->precision_))->ToInteger()->Value();
      if(args.Length() > 2){
        obj->rMode_ = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) obj->rMode_))->ToUint32()->Value();
      }
    }
    mpfr_init2(*obj->mpFloat_, obj->precision_);
    mpfr_set_d(*obj->mpFloat_, args[0]->ToNumber()->Value(), obj->rMode_);
  }	
  else if(args[0]->IsString()){
    if(args.Length() > 2){
      obj->precision_ = (mpfr_prec_t) getOpPrecision(args[2], Number::New((long int) obj->precision_))->ToInteger()->Value();
      if(args.Length() > 3){
        obj->rMode_ = (mpfr_rnd_t) getOpRmode(args[3], Integer::New((unsigned int) obj->rMode_))->ToUint32()->Value();
      }
    }
    if(args.Length() > 1){
      if(args[1]->IsUint32()){
        base = args[1]->ToUint32()->Value();
        if(base < 2 || base > 62) {
          ThrowException(Exception::Error(String::New("Base must be >= 2 and <= 62. If empty, default is 10")));
          return scope.Close(Undefined());
        }
      }
      else{
        ThrowException(Exception::TypeError(String::New("Base must be a positive integer")));
        return scope.Close(Undefined()); 
      }
    }
    mpfr_init2(*obj->mpFloat_, obj->precision_);
    String::Utf8Value str(args[0]->ToString());
    mpfr_set_str(*obj->mpFloat_, *str, base, obj->rMode_);
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
  mpfr_exp_t *exponent = new mpfr_exp_t[1];

  String::Utf8Value str(String::New(mpfr_get_str(NULL, exponent, 10, obj->precision_, *obj->mpFloat_, obj->rMode_)));
  std::string floatValue = *str;
  unsigned long int position = (unsigned long int) *exponent;
  if(floatValue.at(0) == '-'){
    position++;
  }
  if(position < floatValue.length()){
    floatValue.insert(position, ".");
  }
  if(floatValue.at(0) == '-'){
    position++;
  }

  Local<String> base = String::Concat(String::New("{ bigFloat: "), String::New(floatValue.c_str()));
  Local<String> precision = String::Concat(String::New(" , precision: "), Number::New((long int) obj->precision_)->ToString());
  Local<String> type = String::Concat(String::Concat(base, precision), String::New(" }"));

  return scope.Close(type);
}

/* Returns a string representing the bigFloat.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::toString(const Arguments& args) {

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  int base = 10;
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }
  
  if(args[0]->IsUint32()){
    base = args[0]->ToUint32()->Value();
    if(base < 2 || base > 62) {
      ThrowException(Exception::Error(String::New("Base must be >= 2 and <= 62. If empty, default is 10")));
      return scope.Close(Undefined());
    }
  }
  else if(args.Length() > 0){
    ThrowException(Exception::TypeError(String::New("Base must be a positive integer")));
    return scope.Close(Undefined()); 
  }
  
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  String::Utf8Value str(String::New(mpfr_get_str(NULL, exponent, base, precision, *obj->mpFloat_, rMode)));
  std::string floatValue = *str;
  unsigned long int position = (unsigned long int) *exponent;
  if(floatValue.at(0) == '-'){
    position++;
  }
  if(position < floatValue.length()){
    floatValue.insert(position, ".");
  }

  return scope.Close(String::New(floatValue.c_str()));
}

/* Gets or sets the precision of the Irrational number.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::precision(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  long int precision = 0;
  
  if(args[0]->IsUndefined()){
    precision = (long int) mpfr_get_prec(*obj->mpFloat_);
  }
  else if(args[0]->IsNumber()){
    precision = args[0]->ToInteger()->Value();
    mpfr_prec_round(*obj->mpFloat_, (mpfr_prec_t) precision, obj->rMode_);
    obj->precision_ = (mpfr_prec_t) precision;
  }
  else{
    ThrowException(Exception::TypeError(String::New("Precision must be a long integer. Leave blank to get precision of object")));
    return scope.Close(Undefined()); 
  }
    
  return scope.Close(Number::New(precision));
}

/* Gets or sets the rounding mode of the irrational number.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::rMode(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  int rMode = 0;
  
  if(args[0]->IsUndefined()){
    rMode = (int) obj->rMode_;
  }
  else if(args[0]->IsNumber()){
    rMode = args[0]->ToInteger()->Value();
    obj->rMode_ = (mpfr_rnd_t) rMode;
  }
  else{
    ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3, or 4. Leave blank to get rounding mode of object")));
    return scope.Close(Undefined()); 
  }
    
  return scope.Close(Integer::New(rMode));
}

/* Addition.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::add(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_add_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_add(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Substraction, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::sub(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_sub_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_sub(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat, a double or a long integer")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Multiplication.
 * Accepts as main argument a double or long integer and 
 * optionally a long integer precision and unsigned int integer rounding mode of the operation.
 */

Handle<Value> bigFloat::mul(const Arguments& args) {
  HandleScope scope;

  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_mul_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_mul(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Substraction, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::div(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_div_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_div(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}
/* Exponentiation, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::pow(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_pow_si(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_pow(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or long integer")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* n-th root, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::root(const Arguments& args) {
  HandleScope scope;

  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpPrecision(args[1], Number::New((long int) precision))->ToInteger()->Value();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpRmode(args[2], Integer::New((unsigned int) rMode))->ToUint32()->Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_root(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    long int exponent = mpfr_get_sj(*obj2->mpFloat_, rMode);
    mpfr_root(*res, *obj->mpFloat_, exponent, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or long integer")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}
/* Comparison, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::cmp(const Arguments& args) {

  HandleScope scope;
  int result = 0;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
        
  if(args[0]->IsNumber()){
    result = mpfr_cmp_d(*obj->mpFloat_, args[0]->ToNumber()->Value());
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    result = mpfr_cmp(*obj->mpFloat_, *obj2->mpFloat_);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  return scope.Close(Integer::New(result));
}

/* Absolute value.
 * Accepts as optional argument a unsigned integer as rounding mode.
 */

Handle<Value> bigFloat::abs(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t *res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpRmode(args[0], Integer::New((unsigned int) rMode))->ToUint32()->Value();
  }
  
  mpfr_init2(*res, precision);	
  mpfr_abs(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}
