#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION 
#endif

#ifndef MPFR_USE_INTMAX_T
#define MPFR_USE_INTMAX_T
#endif

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
  
  // Instance Vars
  
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  Local<ObjectTemplate> configTpl = ObjectTemplate::New();
  configTpl->SetInternalFieldCount(1);
  configTpl->SetAccessor(String::New("precision"), getPrecision, setPrecision);
  configTpl->SetAccessor(String::New("rMode"), getRmode, setRmode);
  
  config *defaults = new config(53, 3);
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

  if(args[0]->IsUndefined()){
    mpfr_init2(*obj->mpFloat_, (mpfr_prec_t) obj->precision_);
  }
  else if(args[0]->IsNumber()){
    if(args.Length() > 1 && args[1]->IsNumber()){
      obj->precision_ = (mpfr_prec_t) args[1]->ToInteger()->Value();			
    }
    if(args.Length() > 2 && args[2]->IsInt32()){
      obj->rMode_ = (mpfr_rnd_t) args[2]->ToInt32()->Value();
    }
    mpfr_init2(*obj->mpFloat_, (mpfr_prec_t) obj->precision_);
    mpfr_set_d(*obj->mpFloat_, args[0]->ToNumber()->Value(), (mpfr_rnd_t) obj->rMode_);
  }	
  else if(args[0]->IsString()){
    if(args.Length() > 1 && args[1]->IsNumber()){
      base = args[1]->ToInt32()->Value();
      if(base < 2 || base > 62) {
        ThrowException(Exception::Error(String::New("Base two must be >= 2 and <= 62. If empty, default is 10")));
        return scope.Close(Undefined());
      }
    }
    if(args.Length() > 2 && args[2]->IsInt32()){
      obj->precision_ = (mpfr_prec_t) args[2]->ToInt32()->Value();
    }
    if(args.Length() > 3 && args[3]->IsInt32()){
      obj->rMode_ = (mpfr_rnd_t) args[3]->ToInt32()->Value();
    }
  mpfr_init2(*obj->mpFloat_, (mpfr_prec_t) obj->precision_);
  String::Utf8Value str(args[0]->ToString());
  mpfr_set_str(*obj->mpFloat_, *str, base, (mpfr_rnd_t) obj->rMode_);
  }
  else if(args[0]->IsExternal()){
    obj->mpFloat_ = (mpfr_t *) External::Unwrap(args[0]);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Argument 1 must be a number or a string.")));
    return scope.Close(Undefined());
  }

  obj->Wrap(args.This());

  return scope.Close(args.This());
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
  printf("%d", obj->rMode_);	
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  Local<String> floatValue = String::New(mpfr_get_str(NULL, exponent, 10, 0, *obj->mpFloat_, (mpfr_rnd_t) obj->rMode_));
  Local<String> expString = String::Concat(String::New("e"), Number::New((uint64_t) *exponent)->ToString());
  Local<String> type = String::Concat(String::Concat(String::New("<bigFloat:"), floatValue), expString);

  return scope.Close(String::Concat(type, String::New(">")));
}

/* Returns a string representing the bigFloat.
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
  
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  Local<String> floatValue = String::New(mpfr_get_str(NULL, exponent, base, 0, *obj->mpFloat_, (mpfr_rnd_t) obj->rMode_));
  Local<String> expString = Number::New((uint64_t) *exponent)->ToString();
  Local<String> baseString = String::Concat(floatValue, String::New("e"));
        
  
  return scope.Close(String::Concat(baseString, expString));
}

/* Gets or sets the precision of the Irrational number.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::precision(const Arguments& args) {
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
  
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  Local<String> floatValue = String::New(mpfr_get_str(NULL, exponent, base, 0, *obj->mpFloat_, (mpfr_rnd_t) obj->rMode_));
  Local<String> expString = Number::New((uint64_t) *exponent)->ToString();
  Local<String> baseString = String::Concat(floatValue, String::New("e"));
        
  
  return scope.Close(String::Concat(baseString, expString));
}

/* Gets or sets the rounding mode of the irrationla number.
 * An optional base to convert the string to could be optionally provided.
 * Accepts no arguments or a int32 >= 2 and <=62.
 */

Handle<Value> bigFloat::rMode(const Arguments& args) {
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
  
  mpfr_exp_t *exponent = new mpfr_exp_t[1];
  Local<String> floatValue = String::New(mpfr_get_str(NULL, exponent, base, 0, *obj->mpFloat_, (mpfr_rnd_t) obj->rMode_));
  Local<String> expString = Number::New((uint64_t) *exponent)->ToString();
  Local<String> baseString = String::Concat(floatValue, String::New("e"));
        
  
  return scope.Close(String::Concat(baseString, expString));
}

/* Addition, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::add(const Arguments& args) {
  HandleScope scope;

  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_add_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_sub_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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

/* Substraction, normal or modular.
 * Accepts as main argument a uint64 or a biginteger object and
 * optionally a uint64 or biginteger modulus for modular addition.
 */

Handle<Value> bigFloat::mul(const Arguments& args) {
  HandleScope scope;

  bigFloat *obj = ObjectWrap::Unwrap<bigFloat>(args.This());
  mpfr_t * res = new mpfr_t[1];
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_mul_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_div_d(*res, *obj->mpFloat_, args[0]->ToNumber()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_pow_si(*res, *obj->mpFloat_, args[0]->ToInteger()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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
        Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  mpfr_init2(*res, precision);
        
  if(args[0]->IsNumber()){
    mpfr_root(*res, *obj->mpFloat_, args[0]->ToInteger()->Value(), rMode);
  }
  else if(args[0]->IsObject()){
    bigFloat* obj2 = ObjectWrap::Unwrap<bigFloat>(args[0]->ToObject());
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
  Local<Object> defaults = args.This()->Get(String::New("defaults"))->ToObject();
  mpfr_prec_t precision = (mpfr_prec_t ) defaults->Get(String::New("precision"))->ToInteger()->Value();
  mpfr_rnd_t rMode = (mpfr_rnd_t) defaults->Get(String::New("rMode"))->ToInt32()->Value();
    
  if(args.Length() > 2){
    if(args[2]->IsNumber()){
      precision = (mpfr_prec_t) args[2]->ToInteger()->Value();
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Precision must be an integer")));
      return scope.Close(Undefined());
    }
  }
  if(args.Length() > 3){
    if(args[3]->IsInt32()){
      rMode = (mpfr_rnd_t) args[3]->ToInt32()->Value();
      if((int) rMode < 0 || (int) rMode > 4){
        ThrowException(Exception::TypeError(String::New("Rounding mode must be 0, 1, 2, 3 or 4")));
        return scope.Close(Undefined());
      }	
    }
    else{		
      ThrowException(Exception::TypeError(String::New("Rounding must be an integer")));
      return scope.Close(Undefined());
    }
  }
      
  
        
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
