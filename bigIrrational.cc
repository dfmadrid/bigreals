#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "bigFloat.h"

using namespace v8;

Handle<Value> bigIrrational(const Arguments& args) {

  HandleScope scope;

  return scope.Close(bigFloat::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  
  bigFloat::Init();

  module->Set(String::NewSymbol("exports"),
    FunctionTemplate::New(bigIrrational)->GetFunction());
 
}

NODE_MODULE(bigIrrational, InitAll)
