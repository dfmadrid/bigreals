#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION 
#endif

#ifndef MPFR_USE_INTMAX_T
#define MPFR_USE_INTMAX_T
#endif

#include <node.h>
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
  
  // Defaults Javascript object
  
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  Local<ObjectTemplate> configTpl = ObjectTemplate::New();
  configTpl->SetInternalFieldCount(1);
  configTpl->SetAccessor(String::New("precision"), config::getPrecision, config::setPrecision);
  configTpl->SetAccessor(String::New("rMode"), config::getRmode, config::setRmode);
  
  config *defaults = new config(53, 0);
  Handle<Object> defaultConf = configTpl->NewInstance();
  defaultConf->SetInternalField(0, External::New(defaults));

  tpl->InstanceTemplate()->Set(String::NewSymbol("defaults"), defaultConf);

  // Javascript Prototype of bigReals irrational number object

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
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toInt"),
    FunctionTemplate::New(toInt)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mod"),
    FunctionTemplate::New(mod)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("agMean"),
    FunctionTemplate::New(agMean)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("riemannZ"),
    FunctionTemplate::New(riemannZ)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("gamma"),
    FunctionTemplate::New(gamma)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("euNorm"),
    FunctionTemplate::New(euNorm)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("is"),
    FunctionTemplate::New(is)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());

}

// Defaults object precision property getter

Handle<Value> config::getPrecision(Local<String> property, const AccessorInfo &info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  unsigned long value = static_cast<config*>(ptr)->precision_;	
  
  return Number::New(value);
}
  
// Defaults object precision property setter

void config::setPrecision(Local<String> property, Local<Value> value, const AccessorInfo& info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  Local<String> errorStr;
  
  if(value->NumberValue() < 2 || value->NumberValue() > MPFR_PREC_MAX - 100){
    errorStr = String::Concat(String::New("Precision must be > 2 and < "), Number::New(MPFR_PREC_MAX - 100)->ToString());
    ThrowException(Exception::Error(errorStr));
  }

  static_cast<config*>(ptr)->precision_ = value->NumberValue();
}

// Defaults object rMode property getter

Handle<Value> config::getRmode(Local<String> property, const AccessorInfo &info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  unsigned int value = static_cast<config*>(ptr)->rMode_;

  return Uint32::New(value);
}
  
// Defaults object rMode property setter

void config::setRmode(Local<String> property, Local<Value> value, const AccessorInfo& info) {

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  
  if(value->Uint32Value() > 4){
    ThrowException(Exception::Error(String::New("Rounding mode must be >= 0 and <= 4")));
  }

  static_cast<config*>(ptr)->rMode_ = value->Uint32Value();
}

/* Helper function for parsing a base, a precision, a rounding mode or a number digits from user
 * supplied arguments to a particular function.
 * Checks if the argument exists in the specified position of Arguments& array and if it fits in
 * the expected max/min range.
 * If no argument exists in the specified position of Arguments& array, return defaultValue
 */

Handle<Value> bigFloat::getOpArg(int index, std::string type, const Arguments& args, Handle<Value> defaultValue){

  HandleScope scope;
  Local<String> strStart = String::Concat(String::New(type.c_str()), String::New(" must be "));
  Local<String> strEnd = String::New(">=");

  if(index >= args.Length()){
    return scope.Close(defaultValue);
  }
  else if(type == "Precision" && args[index]->IsNumber()){
    
    // As MPFR adjusts internally precision, Precision must be lower than MPFR_PREC_MAX or Node could crash
  
    if(args[index]->NumberValue() < 2 || args[index]->NumberValue() > MPFR_PREC_MAX - 100){
      strEnd = String::Concat(strEnd, String::Concat(Number::New(2)->ToString(), String::New(" and <= ")));
      strEnd = String::Concat(strEnd, Number::New(MPFR_PREC_MAX - 100)->ToString());
      ThrowException(Exception::Error(String::Concat(strStart, strEnd)));
      return scope.Close(Undefined());
    }
            
    return scope.Close(args[index]);
  }
  else if(type == "Rounding Mode" && args[index]->IsNumber()){
    
    if(args[index]->Uint32Value() > 4){
      strEnd = String::Concat(strEnd, Number::New(0)->ToString());
      strEnd = String::Concat(strEnd, String::Concat(String::New(" and <= "), Number::New(4)->ToString()));
      ThrowException(Exception::Error(String::Concat(strStart, strEnd)));
      return scope.Close(Undefined());
    }

    return scope.Close(args[index]);
  }
  else if(type == "Base" && args[index]->IsNumber()){
  
    // This case is used to parse both strings bases and exp/log bases
    
    if(args[index]->Uint32Value() < 2 || args[index]->Uint32Value() > 62){
      strEnd = String::Concat(strEnd, String::Concat(Number::New(2)->ToString(), String::New(" and <=")));
      strEnd = String::Concat(strEnd, Number::New(63)->ToString());
      ThrowException(Exception::Error(String::Concat(strStart, strEnd)));
      return scope.Close(Undefined());
    }
    
    return scope.Close(args[index]);
  }
  else if(type == "Number of Digits" && args[index]->IsNumber()){
      
    if(args[index]->NumberValue() > MPFR_PREC_MAX - 100){
      strEnd = String::Concat(strEnd, String::Concat(Number::New(0)->ToString(), String::New(" and <=")));
      strEnd = String::Concat(strEnd, Number::New(MPFR_PREC_MAX - 100)->ToString());
      ThrowException(Exception::Error(String::Concat(strStart, strEnd)));
      return scope.Close(Undefined());
    }

    return scope.Close(args[index]);
  }
  
  return scope.Close(Undefined());
}

/* bigReals irrational number constructor.
 * Accepts a number specified by a string with an optional base, an integer or double number or a string specifying a
 * constant like log2, pi, euler's or catalan's to be used as initialization value.
 * If no argument is provided, instantiates the number to NaN.
 * Internally is called with a pointer to a mpfr struct as argument to return the result of operations
 */

Handle<Value> bigFloat::New(const Arguments& args){

  HandleScope scope;
  bigFloat *obj = new bigFloat();

  // Get the defaults object precision and rMode properties.

  Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  obj->precision_ = (mpfr_prec_t) defaults->Get(String::New("precision"))->NumberValue();
  obj->rMode_ = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->Uint32Value();
  obj->mpFloat_ = new mpfr_t[1];
  int base;

  
  if(args[0]->IsExternal()){
    obj->mpFloat_ = (mpfr_t *) External::Unwrap(args[0]);
    obj->precision_ = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
    obj->rMode_ = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  }
  else if(args[0]->IsUndefined()){
    mpfr_init2(*obj->mpFloat_, obj->precision_);
  }
  else if(args[0]->IsNumber()){
    obj->precision_ = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
    obj->rMode_ = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
    mpfr_init2(*obj->mpFloat_, obj->precision_);
    mpfr_set_d(*obj->mpFloat_, args[0]->NumberValue(), obj->rMode_);
  }	
  else if(args[0]->IsString()){
    String::Utf8Value str(args[0]->ToString());
    std::string argString = *str;
    
    if(argString == "Pi" || argString == "Catalan" || argString == "Euler" || argString == "log2"){
      obj->precision_ = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
      obj->rMode_ = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
      mpfr_init2(*obj->mpFloat_, obj->precision_);
      
      if(argString == "Pi"){
        mpfr_const_pi(*obj->mpFloat_, obj->rMode_);
      }
      if(argString == "Catalan"){
        mpfr_const_catalan(*obj->mpFloat_, obj->rMode_);
      }
      if(argString == "Euler"){
        mpfr_const_euler(*obj->mpFloat_, obj->rMode_);
      }
      if(argString == "log2"){
        mpfr_const_log2(*obj->mpFloat_, obj->rMode_);
      }
    
    }
    else{
      base = getOpArg(1, "Base", args, Uint32::New(10))->Uint32Value();
      obj->precision_ = (mpfr_prec_t) getOpArg(2, "Precision", args, Number::New(obj->precision_))->NumberValue();
      obj->rMode_ = (mpfr_rnd_t) getOpArg(3, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();	
      mpfr_init2(*obj->mpFloat_, obj->precision_);
      mpfr_set_str(*obj->mpFloat_, *str, base, obj->rMode_);
    }
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a number or a string. Leave blank to initialize to NaN.")));
    return scope.Close(Undefined());
  }

  obj->Wrap(args.This());

  return args.This();
}

// Simulates the behaviour a Javascript's new operator

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


/* Returns the value, precision and rounding mode of a bigReal number inspected by console.log().
 * The value of the bigReal number is the one needed by MPFR to exactly convert back the string to
 * the same bigReal number rounding to the nearest.
 */

Handle<Value> bigFloat::inspect(const Arguments& args){

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());	
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  Local<String> base, precision, rMode, inspection;

  std::string realValue(mpfr_get_str(NULL, exponent, 10, 0, *obj->mpFloat_, obj->rMode_));
  unsigned long position = (unsigned long) *exponent;
  
  if(realValue.at(0) == '-'){
    position++;
  }
  if(position < realValue.length()){
    realValue.insert(position, ".");
  }
  
  base = String::Concat(String::New("{ bigFloat: "), String::New(realValue.c_str()));
  precision = String::Concat(String::New(" , precision: "), Number::New(obj->precision_)->ToString());
  rMode = String::Concat(String::New(" , rMode: "), Uint32::New(obj->rMode_)->ToString());
  inspection = String::Concat(String::Concat(base, String::Concat(precision, rMode)), String::New(" }"));

  return scope.Close(inspection);
}

/* Returns a string in the optionally specified base between 2 and 62 representing the bigReal number.
 * An optional number of most significant digits to print and rounding mode could be provided.
 * If no base is provided, base 10 is assumed. If no number of digits is provided, MPFR will print
 * the number of most significant digits needed to exactly convert back the string to the same bigReal
 * number rounding to the nearest.
 */

Handle<Value> bigFloat::toString(const Arguments& args) {

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  int base;
  size_t numBytes;
  mpfr_rnd_t rMode;
  
  base = getOpArg(0, "Base", args, Uint32::New(10))->Uint32Value();
  numBytes = (size_t) getOpArg(1, "Number of Digits", args, Uint32::New(0))->Uint32Value();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
    
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  std::string realValue(mpfr_get_str(NULL, exponent, base, numBytes, *obj->mpFloat_, rMode));
  unsigned long position = (unsigned long) *exponent;
  
  if(realValue.at(0) == '-'){
    position++;
  }
  if(position < realValue.length()){
    realValue.insert(position, ".");
  }
  
  return scope.Close(String::New(realValue.c_str()));
}

/* Gets or sets the precision of the bigReal Irrational number.
 * If a number >= 2 and <= MPFR_PREC_MAX - 100 is provided, sets the precision of the bigReal rounding to
 * the optionally specified rounding mode and returning the new bigReal number.
 * If no arguments are provided, returns the current precision of the bigReal.
 */

Handle<Value> bigFloat::precision(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_prec_t precision = 0;
  
  if(args[0]->IsUndefined()){
    precision = mpfr_get_prec(*obj->mpFloat_);
    return scope.Close(Number::New(precision));
  }
  else {
    obj->precision_ = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(obj->precision_))->NumberValue();
    mpfr_prec_round(*obj->mpFloat_, obj->precision_, obj->rMode_);
    return scope.Close(args.This());
  }
    
}

/* Gets or sets the rounding mode of the bigReal irrational number.
 * If number >=0 and <=4 is provided, sets the new rounding mode of the bigReal and returns the new bigReal number.
 * If no arguments are provided, returns the current rounding mode of the bigReal.
 */

Handle<Value> bigFloat::rMode(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_rnd_t rMode;
  
  if(args[0]->IsUndefined()){
    rMode = obj->rMode_;
    return scope.Close(Integer::New(rMode));
  }
  else {
    // TODO: Round the bigReal number besides changin the internal rMode field

    obj->rMode_ = (mpfr_rnd_t) getOpArg(0, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
    return scope.Close(args.This());
  }
    
}

/* Addition with an integer, a double or another bigReal number.
 * If operand is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */


Handle<Value> bigFloat::add(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Substraction with an integer, a double or another bigReal number.
 * If operand is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */

Handle<Value> bigFloat::sub(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Multiplication by an integer, a double or another bigReal number.
 * If operand is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */

Handle<Value> bigFloat::mul(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Division by an integer, a double or another bigReal number. NaN and infinity are valid results.
 * If divisor is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */

Handle<Value> bigFloat::div(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  
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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Exponentiation to the power of an integer, a double or another bigReal number.
 * If exponent is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */

Handle<Value> bigFloat::pow(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* n-th root with an integer, a double or another bigReal number as exponent.
 * If exponent is an integer, it will be implicitly casted to a double.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 * If no precision is provided, chooses the highest precision between the ones of the two operands.
 */

Handle<Value> bigFloat::root(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Natural/Napierian logarithm of the bigReal number.
 * Optionally a precision and rounding mode of the calculation's result could be providedd.
 */


Handle<Value> bigFloat::ln(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  
  mpfr_init2(*res, precision);
  mpfr_log(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Base 2 or base 10 Logarithm of a bigReal number exponent.
 * Accepts as arguments a the base of the logarithm, either 2 or 10.
 * Optionally a precision and rounding mode of the operation could be provided.
 */


Handle<Value> bigFloat::log(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
  int base;
  
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  base = getOpArg(0, "Base", args, Uint32::New(0))->Uint32Value();

  mpfr_init2(*res, precision);

  if(base == 2){
    mpfr_log2(*res, *obj->mpFloat_, rMode);
  }			 
  else if(base == 10){
    mpfr_log10(*res, *obj->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
    return scope.Close(Undefined());
  }	

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Exponentiation base e of a bigReal number as exponent.
 * Optionally a precision and rounding mode of could be provided.
 */


Handle<Value> bigFloat::e(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  
  mpfr_init2(*res, precision);
  mpfr_exp(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Base 2 or base 10 Logarithm of a bigReal number exponent.
 * Accepts as arguments a the base of the logarithm, either 2 or 10.
 * Optionally a precision and rounding mode of the operation could be provided.
 */


Handle<Value> bigFloat::exp(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
  unsigned base;

  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  base = getOpArg(2, "Base", args, Uint32::New(0))->Uint32Value();

  mpfr_init2(*res, precision);

  if(base == 2){
    mpfr_exp2(*res, *obj->mpFloat_, rMode);
  }			 
  else if(base == 10){
    mpfr_exp10(*res, *obj->mpFloat_, rMode);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Base must 2 or 10")));
    return scope.Close(Undefined());
  }	

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Cosine, arc-cosine, hyperbolic cosine and inverse hyperbolic cosine of a bigReal number.
 * Optionally a string specifying alternate cosine functions could be provided, by default normal
 * cosine will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::cos(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cosType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string cosType = *str;
  
    if(cosType == "arc"){
      mpfr_acos(*res, *obj->mpFloat_, rMode);
    }
    else if(cosType == "hyp"){
      mpfr_cosh(*res, *obj->mpFloat_, rMode);
    }
    else if(cosType == "invhyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_cos(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Sine, arc-sine, hyperbolic sine and inverse hyperbolic sine of a bigReal number.
 * Optionally a string specifying alternate sine functions could be provided, by default normal
 * sine will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::sin(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> sineType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string sinType = *str;
  
    if(sinType == "arc"){
      mpfr_asin(*res, *obj->mpFloat_, rMode);
    }
    else if(sinType == "hyp"){
      mpfr_sinh(*res, *obj->mpFloat_, rMode);
    }
    else if(sinType == "invhyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_sin(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Tangent, arc-tangent, hyperbolic tangent and inverse hyperbolic tangent of a bigReal number.
 * Optionally a string specifying alternate tangent functions could be provided, by default normal
 * tangent will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::tan(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> tanType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string tanType = *str;
  
    if(tanType == "arc"){
      mpfr_atan(*res, *obj->mpFloat_, rMode);
    }
    else if(tanType == "hyp"){
      mpfr_tanh(*res, *obj->mpFloat_, rMode);
    }
    else if(tanType == "invhyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_tan(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}



/* Arc-tangent2 of a two bigReal numbers.
 * An integer, a double or another bigReal number could be supplied as operand. If a number is provided, it will be
 * casted explicitly to a double.
 * Optionally a precision and rounding mode could be provided.
 */

Handle<Value> bigFloat::atan2(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Secant and hyperbolic secant of a bigReal number.
 * Optionally a string specifying an alternate secant function could be provided, by default normal
 * secant will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::sec(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> secType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string secType = *str;

    if(secType == "hyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_sec(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Cosecant and hyperbolic cosecant of a bigReal number.
 * Optionally a string specifying an alternate cosecant function could be provided, by default normal
 * cosecant will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::cosec(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cosecType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string cosecType = *str;

    if(cosecType == "hyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_csc(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Cotangent and hyperbolic cotangent of a bigReal number.
 * Optionally a string specifying an alternate cotangent function could be provided, by default normal
 * cotangent will be calculated.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::cotan(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> cotanType;
  
  if(args[0]->IsString()){
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    String::Utf8Value str(args[0]->ToString());
    std::string cotanType = *str;

    if(cotanType == "hyp"){
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
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(precision))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_coth(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Factorial of a bigReal number.
 * Optionally a precision and rounding mode of the result could be provided.
 */

Handle<Value> bigFloat::fac(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  mpfr_init2(*res, precision);

  unsigned long factorized = mpfr_get_ui(*obj->mpFloat_, rMode);
  mpfr_fac_ui(*res, factorized, rMode);

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Rounding to an integer fuctions.
 * By default will round the nearest integer rounding in the given direction provided by user supplied
 * rounding mode or the internal rounding mode of the bigReal number.
 * Alternatively rounding functions could be optionally provided specifying the type as a string.
 * Ceil(rounding to the next or equal), floor(rounding to the next lower or equal), round(rounding to
 * the nearest with halfaway cases rounded away from zero) and trunc(rounding to the next toward zero).
 */

Handle<Value> bigFloat::toInt(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> roundType;
  
  if(args[0]->IsString()){
    String::Utf8Value str(args[0]->ToString());
    std::string roundType = *str;
    precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
    mpfr_init2(*res, precision);
    
    if(roundType == "ceil"){
      mpfr_ceil(*res, *obj->mpFloat_);
    }
    if(roundType == "floor"){
      mpfr_floor(*res, *obj->mpFloat_);
    }
    if(roundType == "round"){
      mpfr_round(*res, *obj->mpFloat_);
    }
    if(roundType == "trunc"){
      mpfr_trunc(*res, *obj->mpFloat_);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Rounding type must be \"ceil\", \"floor\", \"round\" or \"trunc\".")));
      return scope.Close(Undefined());
    }	
  }
  else{
    precision = (mpfr_prec_t) getOpArg(0, "Precision", args, Number::New(obj->precision_))->NumberValue();
    rMode = (mpfr_rnd_t) getOpArg(1, "Rounding Mode", args, Uint32::New(rMode))->Uint32Value();
    mpfr_init2(*res, precision);
    mpfr_rint(*res, *obj->mpFloat_, rMode);
  }

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}



/* Mod/remainder fuctions */

Handle<Value> bigFloat::mod(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_t * op2 = new mpfr_t[1];
  mpfr_prec_t precision = obj->precision_;
  mpfr_rnd_t rMode = obj->rMode_;
  Local<String> modType;
  
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  mpfr_init2(*res, precision);
  mpfr_init2(*op2, precision);

  if(args[0]->IsNumber()){
    mpfr_set_d(*op2, args[0]->NumberValue(), rMode);
    mpfr_fmod(*res, *obj->mpFloat_, *op2, rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat *op2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
    mpfr_fmod(*res, *obj->mpFloat_, *op2->mpFloat_, rMode);
  }
  else if(args[0]->IsString()){
    String::Utf8Value str(args[0]->ToString());
    std::string modType = *str;
  
    if(args[0]->IsNumber()){
      mpfr_init2(*op2, precision);
      mpfr_set_d(*op2, args[0]->NumberValue(), rMode);
    }
    else if(args[0]->IsObject()){
      bigFloat *obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
      op2 = obj2->mpFloat_;
    }
    
    if(modType == "remainder"){
      mpfr_remainder(*res, *obj->mpFloat_, *op2, rMode);
    }
    else{
      ThrowException(Exception::TypeError(String::New("Mod type must be \"remainder\". If blank default is round towards zero")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must a bigReal object, the string \"remainder\" or a number")));
    return scope.Close(Undefined());
  }

  mpfr_clear(*op2);
  delete(op2);
  
  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}



/* Arithmetic geometric mean.
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::agMean(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t *res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

  if(args[0]->IsNumber()){
    mpfr_t *op2 = new mpfr_t[1];
    mpfr_init2(*op2, precision);
    mpfr_init2(*res, precision);
    mpfr_set_d(*op2, args[0]->NumberValue(), rMode); 
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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}


/* Euclidean norm. 
 * Accepts as arguments a double or a uint64 signed integer and
 * optionally a precision and rounding mode of the operation.
 * If no precision is provided, chooses the highest precision
 * of the 2 operands.
 */


Handle<Value> bigFloat::euNorm(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();

  if(args[0]->IsNumber()){
    mpfr_t * op2 = new mpfr_t[1];
    mpfr_init2(*op2, precision);
    mpfr_init2(*res, precision);
    mpfr_set_d(*op2, args[0]->NumberValue(), rMode); 
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

  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Riemann Zeta function.
 */

Handle<Value> bigFloat::riemannZ(const Arguments& args) {

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  mpfr_init2(*res, precision);
          
  mpfr_zeta(*res, *obj->mpFloat_, rMode);
  
  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Gamma function.
 */

Handle<Value> bigFloat::gamma(const Arguments& args) {

  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
  mpfr_prec_t precision;
  mpfr_rnd_t rMode;
    
  precision = (mpfr_prec_t) getOpArg(1, "Precision", args, Number::New(obj->precision_))->NumberValue();
  rMode = (mpfr_rnd_t) getOpArg(2, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  mpfr_init2(*res, precision);
          
  mpfr_gamma(*res, *obj->mpFloat_, rMode);
  
  Handle<Value> arg[3] = { External::New(*res), Number::New(precision), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

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
  mpfr_rnd_t rMode;
    
  rMode = (mpfr_rnd_t) getOpArg(0, "Rounding Mode", args, Uint32::New(obj->rMode_))->Uint32Value();
  
  mpfr_init2(*res, obj->precision_);	
  mpfr_abs(*res, *obj->mpFloat_, rMode);

  Handle<Value> arg[3] = { External::New(*res), Number::New(obj->precision_), Uint32::New(rMode) };		
  Local<Object> result = constructor->NewInstance(3, arg);

  return scope.Close(result);
}

/* Returns if object is NaN, Infinity, Zero, an integer an ordinary number or an ordinary number.*/

Handle<Value> bigFloat::is(const Arguments& args) {
  
  HandleScope scope;
  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  Handle<Boolean> result = Boolean::New(false);

  if(args[0]->IsString()){
    String::Utf8Value str(args[0]->ToString());
    std::string type = *str;
  
    if(type == "NaN"){
      result = Boolean::New(mpfr_nan_p(*obj->mpFloat_));
    }
    else if(type == "Infinity"){
      result = Boolean::New(mpfr_inf_p(*obj->mpFloat_));
    }
    else if(type == "Zero"){
      result = Boolean::New(mpfr_zero_p(*obj->mpFloat_));
    }
    else if(type == "Integer"){
      result = Boolean::New(mpfr_integer_p(*obj->mpFloat_));
    }
    else if(type == "Number"){
      result = Boolean::New(mpfr_number_p(*obj->mpFloat_));
    }
    else if(type == "Regular"){
      result = Boolean::New(mpfr_regular_p(*obj->mpFloat_));
    }
    else{
      ThrowException(Exception::TypeError(String::New("Argument must be \"NaN\", \"Infinity\", \"Zero\", \"Integer\", \"Number\" or \"Regular\".")));
      return scope.Close(Undefined());
    }	
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a string specifying the type to compare with.")));
    return scope.Close(Undefined());
  }	

  return scope.Close(result);
}
