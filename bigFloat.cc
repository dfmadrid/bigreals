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

