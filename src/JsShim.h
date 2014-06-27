/*
 * jsShim.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef JSSHIM_H
#define JSSHIM_H

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

#define JS_SHIM_H \
public: \
	static void Init(v8::Handle<v8::Object> exports); \
	static v8::Handle<v8::Value> New(const v8::Arguments& args); \
private: \
	static v8::Persistent<v8::Function> constructor;

#define JS_SHIM(cls) \
v8::Persistent<v8::Function> cls::constructor; \
 \
void cls::Init(v8::Handle<v8::Object> exports) { \
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New); \
	tpl->SetClassName(v8::String::NewSymbol(#cls)); \
	tpl->InstanceTemplate()->SetInternalFieldCount(1); \
 \
	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction()); \
	exports->Set(v8::String::NewSymbol(#cls), constructor); \
} \
 \
v8::Handle<v8::Value> cls::New(const v8::Arguments& args) { \
	v8::HandleScope scope; \
	int i = 0; \
 \
	cls* obj = new cls(args); \
	obj->Wrap(args.This()); \
	v8::Local<v8::Function> super = v8::Local<v8::Function>::Cast(args.Callee()->Get(v8::String::NewSymbol("super_"))); \
	v8::Local<v8::Value> argv[args.Length()]; \
	for(i = 0; i < args.Length(); i++) \
		argv[i] = args[i]; \
	super->Call(args.This(), args.Length(), argv); \
	return scope.Close(args.This()); \
}

class JsShim : public node::ObjectWrap {
protected:
	JsShim(const v8::Arguments& args) {}
public:
	virtual void toJs(v8::Handle<v8::Object> &obj) = 0;
	virtual void fromJs(v8::Handle<v8::Object> &obj) = 0;
};

#endif /* !JSSHIM_H */
