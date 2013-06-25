#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#ifndef BIGREAL_H
#define BIGREAL_H

#include <node.h>
#include <gmp.h>
#include <stdio.h>
#include <mpfr.h>

// Defaults config class

class config {
	public:	
		config(long int precision, int rMode) : precision_(precision), rMode_(rMode) { }
		long int precision_;
		int rMode_;

		static v8::Handle<v8::Value> getPrecision(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		static void setPrecision(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> getRmode(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		static void setRmode(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

// bigReals irrational number class

class bigReal : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
	private:
		bigReal();
		~bigReal();
		static v8::Persistent<v8::Function> constructor;
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> inspect(const v8::Arguments& args);
		static v8::Handle<v8::Value> toString(const v8::Arguments& args);
		static v8::Handle<v8::Value> print(const v8::Arguments& args);
		static v8::Handle<v8::Value> precision(const v8::Arguments& args);
		static v8::Handle<v8::Value> rMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> add(const v8::Arguments& args);
		static v8::Handle<v8::Value> sub(const v8::Arguments& args);
		static v8::Handle<v8::Value> mul(const v8::Arguments& args);
		static v8::Handle<v8::Value> div(const v8::Arguments& args);
		static v8::Handle<v8::Value> pow(const v8::Arguments& args);
		static v8::Handle<v8::Value> root(const v8::Arguments& args);
		static v8::Handle<v8::Value> cmp(const v8::Arguments& args);
		static v8::Handle<v8::Value> abs(const v8::Arguments& args);
		static v8::Handle<v8::Value> ln(const v8::Arguments& args);
		static v8::Handle<v8::Value> log(const v8::Arguments& args);
		static v8::Handle<v8::Value> e(const v8::Arguments& args);
		static v8::Handle<v8::Value> exp(const v8::Arguments& args);
		static v8::Handle<v8::Value> cos(const v8::Arguments& args);
		static v8::Handle<v8::Value> sin(const v8::Arguments& args);
		static v8::Handle<v8::Value> tan(const v8::Arguments& args);
		static v8::Handle<v8::Value> atan2(const v8::Arguments& args);
		static v8::Handle<v8::Value> sec(const v8::Arguments& args);
		static v8::Handle<v8::Value> cosec(const v8::Arguments& args);
		static v8::Handle<v8::Value> cotan(const v8::Arguments& args);
		static v8::Handle<v8::Value> fac(const v8::Arguments& args);
		static v8::Handle<v8::Value> toInt(const v8::Arguments& args);
		static v8::Handle<v8::Value> mod(const v8::Arguments& args);
		static v8::Handle<v8::Value> riemannZ(const v8::Arguments& args);
		static v8::Handle<v8::Value> gamma(const v8::Arguments& args);
		static v8::Handle<v8::Value> agMean(const v8::Arguments& args);
		static v8::Handle<v8::Value> euNorm(const v8::Arguments& args);
		static v8::Handle<v8::Value> is(const v8::Arguments& args);
		static v8::Handle<v8::Value> getOpArg(int index, std::string type, const v8::Arguments& arg, v8::Handle<v8::Value> defaultValue);
		mpfr_t *mpFloat_;
		mpfr_prec_t precision_;
		mpfr_rnd_t rMode_;
};

#endif

