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
#include "v8utils.h"

using namespace v8;
Handle<Value> Method(const Arguments& args) {
	HandleScope scope;
	return scope.Close(String::New("world"));
}

/**
 * @brief opens PDF from an FD with specified engine
 * doc - PdfDocument
 * password - String
 * fd - file descriptor
 */
Handle<Value> openFromFd(const Arguments& args) {
	HandleScope scope;
	v8::Local<v8::Object> doc = args[0]->ToObject();
	v8::Local<v8::Object> jsEngine = doc->Get(v8::String::NewSymbol("_engine"))->ToObject();
	PdfEngine *engine = node::ObjectWrap::Unwrap<PdfEngine>(jsEngine);
	engine->setPassword(v8ToChar(args[1]));

	int fd = v8ToInt(args[2]);
	engine->openFromFd(fd);
	// TODO error handling

	return scope.Close(doc);
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("openFromFd"),
			FunctionTemplate::New(Method)->GetFunction());

	PdfPage::Init(exports);
	PdfDocument::Init(exports);
}

NODE_MODULE(pdfutils, init)
