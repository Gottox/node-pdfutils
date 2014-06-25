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

Handle<Value> syncLoad(const Arguments& args) {
	HandleScope scope;
	Local<Object> doc = args[0]->ToObject();
	//Local<String> password = args[1]->ToString();

	//char *data = node::Buffer::Data(args[2]);
	//size_t length = node::Buffer::Length(args[2]);

	//Local<Object> engineWrap = doc->Get(String::NewSymbol("_engine"))->ToObject();
	//PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(engineWrap);

	//PdfEngine *engine = factory->newInstance();

	//engine->loadData(data, length);

	return scope.Close(doc);
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("asyncLoad"),
			FunctionTemplate::New(Method)->GetFunction());
	exports->Set(String::NewSymbol("asyncLoadFromFd"),
			FunctionTemplate::New(Method)->GetFunction());

	exports->Set(String::NewSymbol("syncLoad"),
			FunctionTemplate::New(syncLoad)->GetFunction());
	exports->Set(String::NewSymbol("syncLoadFromFd"),
			FunctionTemplate::New(Method)->GetFunction());

	PdfPage::Init(exports);
	PdfDocument::Init(exports);
}

NODE_MODULE(pdfutils, init)
