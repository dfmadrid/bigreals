#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#include <node.h>
#include "bigInt.h"

using namespace v8;

Handle<Value> bigInteger(const Arguments& args) {
  HandleScope scope;

  return scope.Close(bigInt::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  bigInt::Init();

  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(bigInteger)->GetFunction());

}

NODE_MODULE(bigInteger, InitAll)
