#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "bigInt.h"

using namespace v8;

Handle<Value> factory(const Arguments& args) {

  HandleScope scope;

  return scope.Close(bigInt::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {

  bigInt::Init();

  module->Set(String::NewSymbol("exports"),
    FunctionTemplate::New(factory)->GetFunction());

}

NODE_MODULE(bigIntegers, InitAll)
