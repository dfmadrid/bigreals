#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "bigReal.h"

using namespace v8;

Handle<Value> factory(const Arguments& args) {

  HandleScope scope;

  return scope.Close(bigReal::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  
  bigReal::Init();

  module->Set(String::NewSymbol("exports"),
    FunctionTemplate::New(factory)->GetFunction());
 
}

NODE_MODULE(bigReals, InitAll)
