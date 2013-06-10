#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif
#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include <node.h>
#include <gmp.h>
#include <mpfr.h>

class config {
	public:	
		config(long int precision, int rMode) : precision_(precision), rMode_(rMode) { }
		long int precision_;
		int rMode_;
};

class bigFloat : public node::ObjectWrap {
	public:
		static void Init();
		static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
		mpfr_t *mpFloat_;
		config *defaults_;
	private:
		bigFloat();
		~bigFloat();
		static v8::Persistent<v8::Function> constructor;
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> inspect(const v8::Arguments& args);
		static v8::Handle<v8::Value> toString(const v8::Arguments& args);
		static v8::Handle<v8::Value> getPrecision(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		static void setPrecision(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> getRmode(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		static void setRmode(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
};

#endif

