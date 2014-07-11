/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include "PdfEngine.h"
#include "PdfEngineFactory.h"
#include "PdfController.h"
#include "PdfPageController.h"
#include "v8utils.h"

/**
 * @brief opens PDF from an FD with specified engine
 * doc - PdfController
 * password - String
 * fd - file descriptor
 */
NAN_METHOD(openFromPath) {
	int i;
	char *error;
	NanScope();
	v8::Local<v8::Object> jsDoc = args[0]->ToObject();
	v8::Local<v8::Function> pdfPageFactory = v8::Function::Cast(*args[1]);
	PdfController *doc = node::ObjectWrap::Unwrap<PdfController>(jsDoc);
	v8::Local<v8::Object> jsEngine = jsDoc->Get(v8::String::NewSymbol("_engine"))->ToObject();

	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	PdfEngine *engine = factory->newInstance();
	engine->setPassword(v8ToChar(args[2]));
	doc->setEngine(engine);

	char *src = v8ToChar(args[3]);
	if((error = engine->openFromPath(src))) {
		THROW_FREE(Error, error);
	}
	engine->fillDocument(doc->document());
	doc->toJs(jsDoc);

	v8::Handle<v8::Value> argv[] = { jsDoc, v8::Integer::New(doc->document()->length()) };
	pdfPageFactory->Call(v8::Context::GetCurrent()->Global(), 2, argv);

	for(i = 0; i < doc->document()->length(); i++) {
		v8::Local<v8::Object> jsPage = jsDoc->Get(i)->ToObject();
		PdfPageController *page = node::ObjectWrap::Unwrap<PdfPageController>(jsPage);
		engine->fillPage(i, page->page());
		page->toJs(jsPage);
	}

	NanReturnValue(jsDoc);
}

/**
 * @brief opens PDF from an FD with specified engine
 * doc - PdfController
 * password - String
 * fd - file descriptor
 */
NAN_METHOD(openFromData) {
	int i;
	char *error;
	NanScope();
	v8::Local<v8::Object> jsDoc = args[0]->ToObject();
	v8::Local<v8::Function> pdfPageFactory = v8::Function::Cast(*args[1]);
	PdfController *doc = node::ObjectWrap::Unwrap<PdfController>(jsDoc);
	v8::Local<v8::Object> jsEngine = jsDoc->Get(v8::String::NewSymbol("_engine"))->ToObject();

	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	PdfEngine *engine = factory->newInstance();
	engine->setPassword(v8ToChar(args[2]));
	doc->setEngine(engine);

	char *data = node::Buffer::Data(args[3]);
	int len = node::Buffer::Length(args[3]);
	if((error = engine->openFromData(data, len))) {
		THROW_FREE(Error, error);
	}
	engine->fillDocument(doc->document());
	doc->toJs(jsDoc);

	v8::Handle<v8::Value> argv[] = { jsDoc, v8::Integer::New(doc->document()->length()) };
	pdfPageFactory->Call(v8::Context::GetCurrent()->Global(), 2, argv);

	for(i = 0; i < doc->document()->length(); i++) {
		v8::Local<v8::Object> jsPage = jsDoc->Get(i)->ToObject();
		PdfPageController *page = node::ObjectWrap::Unwrap<PdfPageController>(jsPage);
		engine->fillPage(i, page->page());
		page->toJs(jsPage);
	}

	NanReturnValue(jsDoc);
}

void init(v8::Handle<v8::Object> exports) {
	exports->Set(NanNew<v8::String>("openFromPath"),
			NanNew<v8::FunctionTemplate>(openFromPath)->GetFunction());
	exports->Set(NanNew<v8::String>("openFromData"),
			NanNew<v8::FunctionTemplate>(openFromData)->GetFunction());

	PdfPageController::Init(exports);
	PdfController::Init(exports);
}

NODE_MODULE(pdfutils, init)
