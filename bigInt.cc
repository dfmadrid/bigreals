#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <cstdlib>
#include <node.h>
#include <gmp.h>
#include "bigInt.h"

using namespace v8;

bigInt::bigInt() {};
bigInt::~bigInt(){
	mpz_clear(*gmpInt_);
	free(gmpInt_);
};

Persistent<Function> bigInt::constructor;

void bigInt::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("bigInt"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
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

Handle<Value> bigInt::New(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = new bigInt();
	obj->gmpInt_ = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*obj->gmpInt_);
	int base = 10;

	if(args[0]->IsUndefined()){
		mpz_set_ui(*obj->gmpInt_, 0);
		}
	else if(args[0]->IsNumber()){
		mpz_set_ui(*obj->gmpInt_, args[0]->ToInteger()->Value());
		}	
	else if(args[0]->IsString()){
		if(args.Length() > 1 && args[1]->IsNumber()){
			base = args[1]->ToInteger()->Value();
			if(base < 2 || base > 62) {
    		ThrowException(Exception::Error(String::New("Base two must be >= 2 and <= 62. If empty, default is 10")));
				return scope.Close(Undefined());
      	}
			}
		String::Utf8Value str(args[0]->ToString());
		mpz_set_str(*obj->gmpInt_, *str, base);
		}
	else if(args[0]->IsExternal()){
		obj->gmpInt_ = (mpz_t *) External::Unwrap(args[0]);
		}
	else{
		ThrowException(Exception::TypeError(String::New("Argument must be an integer, a string or a bigint")));
		return scope.Close(Undefined());
		}

	obj->Wrap(args.This());

  return args.This();
}

Handle<Value> bigInt::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> bigInt::inspect(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	Local<String> bigIntValue = String::New(mpz_get_str(0, 10, *obj->gmpInt_));
	Local<String> type = String::Concat(String::New("<bigInteger:"), bigIntValue);

  return scope.Close(String::Concat(type, String::New(">")));
}

Handle<Value> bigInt::toString(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	int base = 10;
	if(args[0]->IsNumber()){
		base = args[0]->ToInteger()->Value();
		if(base < 2 || base > 62) {
    	ThrowException(Exception::Error(String::New("Base must be >= 2 and <= 62. If empty, default is 10")));
			return scope.Close(Undefined());
      }
		}
	else if(args.Length() > 0){
		ThrowException(Exception::TypeError(String::New("Base must be an integer")));
		return scope.Close(Undefined()); 
		}

  return scope.Close(String::New(mpz_get_str(0, base, *obj->gmpInt_)));
}

Handle<Value> bigInt::add(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);


	if(args[0]->IsNumber()){
		mpz_add_ui(*res, *obj->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_add(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::sub(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args[0]->IsNumber()){
		mpz_sub_ui(*res, *obj->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_sub(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::mul(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args[0]->IsNumber()){
		mpz_mul_ui(*res, *obj->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_mul(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::addMul(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	if(args.Length() > 1 && args[0]->IsObject()){
		if(args[1]->IsObject()){
			bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
			bigInt* obj3 = ObjectWrap::Unwrap<bigInt>(args[1]->ToObject());
			mpz_addmul(*obj->gmpInt_, *obj2->gmpInt_, *obj3->gmpInt_);
			}
		else if(args[1]->IsNumber()){
			bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
			mpz_addmul_ui(*obj->gmpInt_, *obj2->gmpInt_, args[1]->ToInteger()->Value());
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
		obj->gmpInt_ = modRes(obj->gmpInt_, args[2]); 
		}

  return scope.Close(args.This());
}

Handle<Value> bigInt::accMul(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_mul(*res, *obj->gmpInt_, *obj2->gmpInt_);
		}
	else if(args[0]->IsNumber()){
		mpz_mul_ui(*res, *obj->gmpInt_, args[0]->ToInteger()->Value());
			}
	else{
		ThrowException(Exception::TypeError(String::New("Argument 1 must an integer or a bigint")));
		return scope.Close(Undefined());
		}

	if(args.Length() > 1){
		obj->gmpInt_ = modRes(res, args[1]); 
		}
	else{
		obj->gmpInt_ = res;
	}

	return scope.Close(args.This());
}

Handle<Value> bigInt::subMul(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	if(args.Length() > 1 && args[0]->IsObject()){
		if(args[1]->IsNumber()){
			bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
			mpz_submul_ui(*obj->gmpInt_, *obj2->gmpInt_, args[1]->ToInteger()->Value());
			}	
		else if(args[1]->IsObject()){
			bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
			bigInt* obj3 = ObjectWrap::Unwrap<bigInt>(args[1]->ToObject());
			mpz_submul(*obj->gmpInt_, *obj2->gmpInt_, *obj3->gmpInt_);
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
		obj->gmpInt_ = modRes(obj->gmpInt_, args[2]); 
		}

  return scope.Close(args.This());
}

Handle<Value> bigInt::exor(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_xor(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::lShift(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsNumber()){
		mpz_mul_2exp(*res, *obj->gmpInt_, args[0]->ToNumber()->Value());
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

Handle<Value> bigInt::rShift(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsNumber()){
		mpz_tdiv_q_2exp(*res, *obj->gmpInt_, args[0]->ToNumber()->Value());
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

Handle<Value> bigInt::bitAnd(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_and(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::bitOr(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_ior(*res, *obj->gmpInt_, *obj2->gmpInt_);
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

Handle<Value> bigInt::hamDist(const Arguments& args) {
  HandleScope scope;

	unsigned long int distance = 0;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		distance = mpz_hamdist(*obj->gmpInt_, *obj2->gmpInt_);
		}
	else{
		ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
		return scope.Close(Undefined());
		}

  return scope.Close(Number::New(distance));
}

Handle<Value> bigInt::population(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	unsigned long int population = mpz_popcount(*obj->gmpInt_);

  return scope.Close(Number::New(population));
}

Handle<Value> bigInt::cmp(const Arguments& args) {
  HandleScope scope;

	int  result = 0;
  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* obj2 = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		result = mpz_cmp(*obj->gmpInt_, *obj2->gmpInt_);
		}
	else{
		ThrowException(Exception::TypeError(String::New("Argument 1 must be a bigint")));
		return scope.Close(Undefined());
		}

  return scope.Close(Number::New(result));
}

Handle<Value> bigInt::pow(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
 	bigInt *base = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args[0]->IsNumber()){
		mpz_pow_ui(*res, *base->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args.Length() > 1 && args[0]->IsObject() && args[1]->IsObject()){
		bigInt* exponent = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		bigInt* modulus = ObjectWrap::Unwrap<bigInt>(args[1]->ToObject());
		mpz_powm(*res, *base->gmpInt_, *exponent->gmpInt_, *modulus->gmpInt_);
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

Handle<Value> bigInt::isEven(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	int isEven = mpz_even_p(*obj->gmpInt_);

  return scope.Close(Number::New(isEven));
}

Handle<Value> bigInt::isOdd(const Arguments& args) {
  HandleScope scope;

  bigInt *obj = ObjectWrap::Unwrap<bigInt>(args.This());

	int isOdd = mpz_odd_p(*obj->gmpInt_);

  return scope.Close(Number::New(isOdd));
}

Handle<Value> bigInt::root(const Arguments& args) {
  HandleScope scope;

  Handle<Value> result;
 	bigInt *base = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args[0]->IsNumber()){
		mpz_root(*res, *base->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args.Length() > 1 && args[0]->IsObject() && args[1]->IsNumber()){
		bigInt* remainder = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_rootrem(*res, *remainder->gmpInt_, *base->gmpInt_, args[1]->ToInteger()->Value());
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

Handle<Value> bigInt::invert(const Arguments& args) {
  HandleScope scope;

 	bigInt *base = ObjectWrap::Unwrap<bigInt>(args.This());
	mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*res);

	if(args.Length() > 0 && args[0]->IsObject()){
		bigInt* modulus = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_invert(*res, *base->gmpInt_, *modulus->gmpInt_);
		}
	else if(args.Length() >0 && args[0]->IsNumber()){
		mpz_t *modulus = (mpz_t *) malloc(sizeof(mpz_t));
		mpz_init(*modulus);
		mpz_set_ui(*modulus, args[0]->ToInteger()->Value());
		mpz_invert(*res, *base->gmpInt_, *modulus);
		}
	else{
		ThrowException(Exception::TypeError(String::New("Argument must be an integer or a bigint"))); 
		return scope.Close(Undefined());
		}	

  Handle<Value> arg[1] = { External::New(*res) };				
	Local<Object> result = constructor->NewInstance(1, arg);

  return scope.Close(result);
}

Handle<Value> bigInt::div(const Arguments& args) {
  HandleScope scope;

	Handle<Value> result;
  bigInt *dividend = ObjectWrap::Unwrap<bigInt>(args.This());
  mpz_t *quotient = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*quotient);

	if(args.Length() == 1 && args[0]->IsNumber()){
		mpz_div_ui(*quotient, *dividend->gmpInt_, args[0]->ToInteger()->Value());
		}
	else if(args.Length() == 1 && args[0]->IsObject()){
		bigInt* divisor = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		mpz_div(*quotient, *dividend->gmpInt_, *divisor->gmpInt_);
		}
	else if(args.Length() > 1 && args[0]->IsObject()){
		bigInt* remainder = ObjectWrap::Unwrap<bigInt>(args[0]->ToObject());
		if(args[1]->IsObject()){
			bigInt *divisor = ObjectWrap::Unwrap<bigInt>(args[1]->ToObject());
			mpz_tdiv_qr(*quotient, *remainder->gmpInt_, *dividend->gmpInt_, *divisor->gmpInt_);
			}
		else if(args[1]->IsNumber()){
			mpz_tdiv_qr_ui(*quotient, *remainder->gmpInt_, *dividend->gmpInt_, args[1]->ToInteger()->Value());
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

mpz_t * bigInt::modRes(mpz_t * res, Local<Value> modulus){

	mpz_t *modRes = (mpz_t *) malloc(sizeof(mpz_t));
	mpz_init(*modRes);
	if(modulus->IsObject()){
		bigInt* modObj = ObjectWrap::Unwrap<bigInt>(modulus->ToObject());
		mpz_mod(*modRes, *res, *modObj->gmpInt_); 
		}
	else if(modulus->IsNumber()){
		mpz_mod_ui(*modRes, *res, modulus->ToInteger()->Value());
		}
	else{
		ThrowException(Exception::TypeError(String::New("Modulus must be an integer or a bigint")));
		}

	return modRes;
}
