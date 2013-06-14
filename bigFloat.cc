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
  tpl->PrototypeTemplate()->Set(String::NewSymbol("ln"),
    FunctionTemplate::New(ln)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("log"),
    FunctionTemplate::New(log)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("e"),
    FunctionTemplate::New(e)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("exp"),
    FunctionTemplate::New(exp)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("cos"),
    FunctionTemplate::New(cos)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("sin"),
    FunctionTemplate::New(sin)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("tan"),
    FunctionTemplate::New(tan)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("atan2"),
    FunctionTemplate::New(atan2)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("sec"),
    FunctionTemplate::New(sec)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("cosec"),
    FunctionTemplate::New(cosec)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("cotan"),
    FunctionTemplate::New(cotan)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("fac"),
    FunctionTemplate::New(fac)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("agmean"),
    FunctionTemplate::New(agmean)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("eunorm"),
    FunctionTemplate::New(eunorm)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("whatIs"),
    FunctionTemplate::New(whatIs)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("isRegular"),
    FunctionTemplate::New(isRegular)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("isOrdinary"),
    FunctionTemplate::New(isOrdinary)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());

}

Handle<Value> bigFloat::getPrecision(Local<String> property, const AccessorInfo &info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
      unsigned long value = static_cast<config*>(ptr)->precision_;	
  return Number::New(value);
}
  
void bigFloat::setPrecision(Local<String> property, Local<Value> value, const AccessorInfo& info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
      static_cast<config*>(ptr)->precision_ = value->NumberValue();
  }

Handle<Value> bigFloat::getRmode(Local<String> property, const AccessorInfo &info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  unsigned value = static_cast<config*>(ptr)->rMode_;
  return Uint32::New(value);
}
  
void bigFloat::setRmode(Local<String> property, Local<Value> value, const AccessorInfo& info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
      static_cast<config*>(ptr)->rMode_ = value->Uint32Value();
  }

Handle<Value> bigFloat::getOpArg(Handle<Value> arg, Handle<Value> defaultValue){

  HandleScope scope;

  if(arg->IsUndefined()){
    return scope.Close(defaultValue);
  }
  else if(arg->IsNumber()){
    return scope.Close(arg);
  }
  else if(arg->IsUint32()){
    if(arg->Uint32Value() > 4){
      ThrowException(Exception::TypeError(String::New("Rounding mode must be either 0, 1, 2, 3, or 4")));
      return scope.Close(defaultValue);
    }
    else{
      return scope.Close(arg);
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Precision must be a long integer and rounding mode must be 0, 1, 2, 3 or 4")));
    return scope.Close(defaultValue);
  }
}

/* Constructor.
 * Accepts a uint64 number, a string in a provided base or a GMP integer.
 * If no argument is provided, instantiates the object to a biginteger with value 0.
 */

Handle<Value> bigFloat::New(const Arguments& args){

  HandleScope scope;
  bigFloat *obj = new bigFloat();
  Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  obj->precision_ = (mpfr_prec_t) defaults->Get(String::New("precision"))->NumberValue();
  obj->rMode_ = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->Uint32Value();
  obj->mpFloat_ = new mpfr_t[1];
  int base = 10, numArgs = args.Length();

  if(args[0]->IsExternal()){
    obj->mpFloat_ = (mpfr_t *) External::Unwrap(args[0]);
  }
  else if(args[0]->IsUndefined()){
    mpfr_init2(*obj->mpFloat_, obj->precision_);
  }
  else if(args[0]->IsNumber()){
    if(numArgs > 1){
      obj->precision_ = (mpfr_prec_t) getOpArg(args[1], Number::New(obj->precision_))->NumberValue();
    }
    if(numArgs > 2){
      obj->rMode_ = (mpfr_rnd_t) getOpArg(args[2], Uint32::New(obj->rMode_))->Uint32Value();
    }
    mpfr_init2(*obj->mpFloat_, obj->precision_);
    mpfr_set_d(*obj->mpFloat_, args[0]->NumberValue(), obj->rMode_);
  }	
  else if(args[0]->IsString()){
    Local<String> argString = args[0]->ToString();
    Local<String> Pi = String::New("pi");
    Local<String> Catalan = String::New("Catalan");
    Local<String> Euler = String::New("Euler");
    if(argString == Pi || argString == Catalan || argString == Euler){
      if(numArgs > 2){
        obj->precision_ = (mpfr_prec_t) getOpArg(args[2], Number::New(obj->precision_))->NumberValue();
      }
      if(numArgs > 3){
        obj->rMode_ = (mpfr_rnd_t) getOpArg(args[3], Uint32::New(obj->rMode_))->Uint32Value();
      }
      mpfr_init2(*obj->mpFloat_, obj->precision_);
      if(argString == Pi){
        mpfr_const_pi(*obj->mpFloat_, obj->rMode_);
      }
      if(argString == Catalan){
        mpfr_const_catalan(*obj->mpFloat_, obj->rMode_);
      }
      if(argString == Euler){
        mpfr_const_euler(*obj->mpFloat_, obj->rMode_);
      }
      mpfr_free_cache();
    }
    else{
      if(args.Length() > 1){
        if(args[1]->IsUint32()){
          base = args[1]->Uint32Value();
          if(base < 2 || base > 62) {
            ThrowException(Exception::Error(String::New("Base must be >= 2 and <= 62. If empty, default is 10")));
            return scope.Close(Undefined());
          }
          if(numArgs > 2){
            obj->precision_ = (mpfr_prec_t) getOpArg(args[2], Number::New(obj->precision_))->NumberValue();
          }
          if(numArgs > 3){
            obj->rMode_ = (mpfr_rnd_t) getOpArg(args[3], Uint32::New(obj->rMode_))->Uint32Value();
          }
        }
        else{
          ThrowException(Exception::TypeError(String::New("Base must be a positive integer")));
          return scope.Close(Undefined()); 
        }
      }
      mpfr_init2(*obj->mpFloat_, obj->precision_);
      String::Utf8Value str(argString);
      mpfr_set_str(*obj->mpFloat_, *str, base, obj->rMode_);
    }
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
  int argc = args.Length();
  Handle<Value> argv[argc];

  for(int cont = 0; cont < argc; cont++){
    argv[cont] = args[cont];
  }
      
  Local<Object> instance = constructor->NewInstance(argc, argv);
  
  return scope.Close(instance);
}


// Returns a string when object is inspected by console.log().

Handle<Value> bigFloat::inspect(const Arguments& args){

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());	
  mpfr_exp_t *exponent = new mpfr_exp_t[1];

  String::Utf8Value str(String::New(mpfr_get_str(NULL, exponent, 10, 0, *obj->mpFloat_, obj->rMode_)));
  std::string floatValue = *str;
  unsigned long position = (unsigned long) *exponent;
  
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
  size_t numBytes = 0;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    numBytes = (size_t) getOpArg(args[1], Number::New(numBytes))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }
  
  if(args[0]->IsUint32()){
    base = args[0]->Uint32Value();
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
  String::Utf8Value str(String::New(mpfr_get_str(NULL, exponent, base, numBytes, *obj->mpFloat_, rMode)));
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
    precision = args[0]->IntegerValue();
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
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::rMode(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  int rMode = 0;
  
  if(args[0]->IsUndefined()){
    rMode = (int) obj->rMode_;
  }
  else if(args[0]->IsUint32()){
    rMode = args[0]->Uint32Value();
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
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_add_d(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_sub_d(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_mul_d(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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

/* Substraction.
 * Accepts as main argument a uint64 or a biginteger object and
 */

Handle<Value> bigFloat::div(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_div_d(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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
 */

Handle<Value> bigFloat::pow(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_pow_si(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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
 */

Handle<Value> bigFloat::root(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_init2(*res, precision);
    mpfr_root(*res, *obj->mpFloat_, args[0]->NumberValue(), rMode);
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

/* Natural/Neperian Logarithm.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::ln(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 0){
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
  }
  
  mpfr_init2(*res, precision);
  mpfr_log(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}
/* Base 2 or base 10 Logarithm.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::log(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsUint32()){
    unsigned int base = args[0]->Uint32Value();
    mpfr_init2(*res, precision);
    if(base == 2){
      mpfr_log2(*res, *obj->mpFloat_, rMode);
    }			 
    else if(base != 10){
      mpfr_log10(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
    return scope.Close(Undefined());
  }	

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Exponentiation base e.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::e(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 0){
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
  }
  
  mpfr_init2(*res, precision);
  mpfr_exp(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}
/* 2-th or 10-th Exponentiation base e.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::exp(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsUint32()){
    unsigned int base = args[0]->Uint32Value();
    mpfr_init2(*res, precision);
    if(base == 2){
      mpfr_exp2(*res, *obj->mpFloat_, rMode);
    }			 
    else if(base != 10){
      mpfr_exp10(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
    return scope.Close(Undefined());
  }	

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Cos, arc-cosine and hyperbolic cosine
 */

Handle<Value> bigFloat::cos(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cosType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    cosType = args[0]->ToString();
    if(cosType == String::New("arc")){
      mpfr_acos(*res, *obj->mpFloat_, rMode);
    }
    else if(cosType == String::New("hyp")){
      mpfr_cosh(*res, *obj->mpFloat_, rMode);
    }
    else if(cosType == String::New("invhyp")){
      mpfr_acosh(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Cosine type must be \"arc\", \"hyp\" or \"invhyp\". Leave blank for normal cosine")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_cos(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_cos(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Sine, arc-sine and hyperbolic sine
 */

Handle<Value> bigFloat::sin(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> sineType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    sineType = args[0]->ToString();
    if(sineType == String::New("arc")){
      mpfr_asin(*res, *obj->mpFloat_, rMode);
    }
    else if(sineType == String::New("hyp")){
      mpfr_sinh(*res, *obj->mpFloat_, rMode);
    }
    else if(sineType == String::New("invhyp")){
      mpfr_asinh(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Sine type must be \"arc\", \"hyp\" or \"invhyp\". Leave blank for normal sine")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_sin(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_sin(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Tangent, arc-tangent and hyperbolic tangent
 */

Handle<Value> bigFloat::tan(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> tanType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    tanType = args[0]->ToString();
    if(tanType == String::New("arc")){
      mpfr_atan(*res, *obj->mpFloat_, rMode);
    }
    else if(tanType == String::New("hyp")){
      mpfr_tanh(*res, *obj->mpFloat_, rMode);
    }
    else if(tanType == String::New("invhyp")){
      mpfr_atanh(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Tangent type must be \"arc\", \"hyp\" or \"invhyp\". Leave blank for normal tangent")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_tan(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_tan(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}



/* Arc-tangent2.
 */

Handle<Value> bigFloat::atan2(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_atan2(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Secant and hyperbolic secant
 */

Handle<Value> bigFloat::sec(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> secType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    secType = args[0]->ToString();
    if(secType == String::New("hyp")){
      mpfr_sech(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Secant type must be \"hyp\". Leave blank for normal secant")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_sec(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_sec(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Cosecant and hyperbolic cosecant
 */

Handle<Value> bigFloat::cosec(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cosecType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    cosecType = args[0]->ToString();
    if(cosecType == String::New("hyp")){
      mpfr_csch(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Cosecant type must be \"hyp\". Leave blank for normal cosecant")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_csc(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_csc(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Cotangent and hyperbolic cotangent
 */

Handle<Value> bigFloat::cotan(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cotanType;
  
  if(args[0]->IsString()){
    if(args.Length() > 1){
      precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
      if(args.Length() > 2){
        rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
      }
    }
    mpfr_init2(*res, precision);
    cotanType = args[0]->ToString();
    if(cotanType == String::New("hyp")){
      mpfr_coth(*res, *obj->mpFloat_, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Cotangent type must be \"hyp\". Leave blank for normal cotanget")));
      return scope.Close(Undefined());
    }	
  }
  else if(args[0]->IsUndefined()){		
    mpfr_init2(*res, precision);
    mpfr_coth(*res, *obj->mpFloat_, rMode);
  }
  else{
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 1){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
    mpfr_init2(*res, precision);
    mpfr_coth(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Factorial.
 */

Handle<Value> bigFloat::fac(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 0){
    precision = (mpfr_prec_t) getOpArg(args[0], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[1], Integer::New(rMode))->Uint32Value();
    }
  }
  mpfr_init2(*res, precision);
  unsigned long int factorized = mpfr_get_ui(*obj->mpFloat_, rMode);
  mpfr_fac_ui(*res, factorized, rMode);

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Arithmetic geometric mean.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::agmean(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t *res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_t *op2 = new mpfr_t[1];
    mpfr_init2(*op2, precision);
    mpfr_init2(*res, precision);
    mpfr_agm(*res, *obj->mpFloat_, *op2, rMode);
    mpfr_clear(*op2);
    delete(op2);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_agm(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}


/* Euclidean norm. 
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::eunorm(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  if(args.Length() > 1){
    precision = (mpfr_prec_t) getOpArg(args[1], Number::New(precision))->IntegerValue();
    if(args.Length() > 2){
      rMode = (mpfr_rnd_t) getOpArg(args[2], Integer::New(rMode))->Uint32Value();
    }
  }

  if(args[0]->IsNumber()){
    mpfr_t * op2 = new mpfr_t[1];
    mpfr_init2(*op2, precision);
    mpfr_init2(*res, precision);
    mpfr_hypot(*res, *obj->mpFloat_, *op2, rMode);
    mpfr_clear(*op2);
    delete(op2);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    if(obj2->precision_ > precision){
      mpfr_init2(*res, obj2->precision_);
    }
    else{
      mpfr_init2(*res, precision);
    }
    mpfr_hypot(*res, *obj->mpFloat_, *obj2->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigfloat or number")));
    return scope.Close(Undefined());
  }

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Comparison.
 */

Handle<Value> bigFloat::cmp(const Arguments& args) {

  HandleScope scope;
  int result = 0;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
        
  if(args[0]->IsNumber()){
    result = mpfr_cmp_d(*obj->mpFloat_, args[0]->NumberValue());
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
    
  if(args.Length() > 0){
      rMode = (mpfr_rnd_t) getOpArg(args[0], Integer::New(rMode))->Uint32Value();
  }
  
  mpfr_init2(*res, precision);	
  mpfr_abs(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[1] = { External::New(*res) };		
  Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

/* Returns if number is a regular number (neither NaN, nor Infinity nor Zero) 
 * Accepts as optional argument a unsigned integer as rounding mode.
 */

Handle<Value> bigFloat::isRegular(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  Local<String> type;
  
  return scope.Close(Boolean::New(mpfr_regular_p(*obj->mpFloat_)));
}

/* Returns if number is an ordinary number (neither NaN nor Infinity) 
 * Accepts as optional argument a unsigned integer as rounding mode.
 */

Handle<Value> bigFloat::isOrdinary(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  Local<String> type;
  
  return scope.Close(Boolean::New(mpfr_number_p(*obj->mpFloat_)));
}

/* Returns if object is NaN, Infinity, Zero or an ordinary number.
 * Accepts as optional argument a unsigned integer as rounding mode.
 */

Handle<Value> bigFloat::whatIs(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  Local<String> type;
  
  int result = mpfr_nan_p(*obj->mpFloat_);
  
  if(result != 0){
    return scope.Close(String::New("NaN"));
  }
  
  result = mpfr_inf_p(*obj->mpFloat_);
  
  if(result != 0){		
    return scope.Close(String::New("Infinity"));
  }
  
  result = mpfr_zero_p(*obj->mpFloat_);
  
  if(result != 0){		
    return scope.Close(String::New("Zero"));
  }
  else{		
    return scope.Close(String::New("Ordinary"));
  }
}
