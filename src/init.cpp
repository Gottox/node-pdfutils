/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "PdfEngine.h"
#include "PdfEngineFactory.h"
#include "PdfDocument.h"
#include "PdfPage.h"

using namespace v8;
Handle<Value> Method(const Arguments& args) {
	HandleScope scope;
	return scope.Close(String::New("world"));
}

/*
 * JsArgs:
 * doc - PdfDocument
 * password - String
 *
 */
Handle<Value> openFromFd(const Arguments& args) {
	HandleScope scope;
	v8::Local<v8::Object> doc = args[0]->ToObject();
	v8::Local<v8::Object> jsEngine = doc->Get(v8::String::NewSymbol("_engine"))->ToObject();
	PdfEngine *engine = node::ObjectWrap::Unwrap<PdfEngine>(jsEngine);

	return scope.Close(doc);
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("openFromFd"),
			FunctionTemplate::New(Method)->GetFunction());

	PdfPage::Init(exports);
	PdfDocument::Init(exports);
}

NODE_MODULE(pdfutils, init)
