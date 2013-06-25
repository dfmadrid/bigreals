#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#ifndef BIGINT_H
#define BIGINT_H

#include <node.h>
#include <stdio.h>
#include <gmp.h>

class bigInt : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
		mpz_t *gmpInt_;

	private:
		bigInt();
		~bigInt();
		static v8::Persistent<v8::Function> constructor;
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> inspect(const v8::Arguments& args);
		static v8::Handle<v8::Value> toString(const v8::Arguments& args);
		static v8::Handle<v8::Value> add(const v8::Arguments& args);
		static v8::Handle<v8::Value> sub(const v8::Arguments& args);
		static v8::Handle<v8::Value> mul(const v8::Arguments& args);
		static v8::Handle<v8::Value> addMul(const v8::Arguments& args);
		static v8::Handle<v8::Value> accMul(const v8::Arguments& args);
		static v8::Handle<v8::Value> subMul(const v8::Arguments& args);
		static v8::Handle<v8::Value> exor(const v8::Arguments& args);
		static v8::Handle<v8::Value> bitAnd(const v8::Arguments& args);
		static v8::Handle<v8::Value> bitOr(const v8::Arguments& args);
		static v8::Handle<v8::Value> population(const v8::Arguments& args);
		static v8::Handle<v8::Value> hamDist(const v8::Arguments& args);
		static v8::Handle<v8::Value> lShift(const v8::Arguments& args);
		static v8::Handle<v8::Value> rShift(const v8::Arguments& args);
		static v8::Handle<v8::Value> cmp(const v8::Arguments& args);
		static v8::Handle<v8::Value> isEven(const v8::Arguments& args);
		static v8::Handle<v8::Value> isOdd(const v8::Arguments& args);
		static v8::Handle<v8::Value> pow(const v8::Arguments& args);
		static v8::Handle<v8::Value> root(const v8::Arguments& args);
		static v8::Handle<v8::Value> invert(const v8::Arguments& args);
		static v8::Handle<v8::Value> div(const v8::Arguments& args);
		static mpz_t * modRes(mpz_t * res, v8::Local<v8::Value> modulus);
		
};

#endif

