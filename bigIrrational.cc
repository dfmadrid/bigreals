#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include <mpfr.h>
#include "bigFloat.h"
using namespace v8;

Handle<Value> bigIrrational(const Arguments& args) {

  HandleScope scope;

  return scope.Close(bigFloat::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  
  Handle<Value> precision = Integer::New(53);
  Handle<Value> rMode = Integer::New((int32_t) MPFR_RNDD);
  bigFloat::Init(precision, rMode);

  module->Set(String::NewSymbol("exports"),
    FunctionTemplate::New(bigIrrational)->GetFunction());
 
}

NODE_MODULE(bigIrrational, InitAll)
