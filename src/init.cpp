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
#include "PdfDocumentController.h"
#include "PdfPageController.h"
#include "v8utils.h"

/**
 * @brief opens PDF from an FD with specified engine
 * doc - PdfDocumentController
 * password - String
 * fd - file descriptor
 */
NAN_METHOD(openFromPath) {
	int i;
	size_t count;
	char *error;
	NanScope();
	v8::Local<v8::Object> jsDoc = args[0]->ToObject();
	v8::Local<v8::Function> pdfPageFactory = args[1].As<v8::Function>();
	PdfDocumentController *doc = node::ObjectWrap::Unwrap<PdfDocumentController>(jsDoc);
	v8::Local<v8::Object> jsEngine = jsDoc->Get(NanNew<v8::String>("_engine"))->ToObject();

	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	PdfEngine *engine = factory->newInstance();
	if(args[2]->IsString())
		engine->setPassword(NanCString(args[2], &count));
	doc->setEngine(engine);

	char *src = NanCString(args[3], &count);
	if((error = engine->openFromPath(src))) {
		// TODO: Make sure we do this right
		NanThrowError(error);
		delete error;
		NanReturnNull();
	}
	engine->fillDocument(doc->document());
	doc->toJs(jsDoc);

	v8::Handle<v8::Value> argv[] = { jsDoc, NanNew<v8::Integer>(doc->document()->length()) };
	pdfPageFactory->Call(jsDoc, 2, argv);

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
 * doc - PdfDocumentController
 * password - String
 * fd - file descriptor
 */
NAN_METHOD(openFromData) {
	int i;
	size_t count;
	char *error;
	NanScope();
	v8::Local<v8::Object> jsDoc = args[0]->ToObject();
	v8::Local<v8::Function> pdfPageFactory = args[1].As<v8::Function>();
	PdfDocumentController *doc = node::ObjectWrap::Unwrap<PdfDocumentController>(jsDoc);
	v8::Local<v8::Object> jsEngine = jsDoc->Get(NanNew<v8::String>("_engine"))->ToObject();

	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	PdfEngine *engine = factory->newInstance();
	if(args[2]->IsString())
		engine->setPassword(NanCString(args[2], &count));
	doc->setEngine(engine);

	char *data = node::Buffer::Data(args[3]);
	int len = node::Buffer::Length(args[3]);
	if((error = engine->openFromData(data, len))) {
		// TODO: Make sure we do this right
		NanThrowError(error);
		delete error;
		NanReturnNull();
	}
	engine->fillDocument(doc->document());
	doc->toJs(jsDoc);

	v8::Handle<v8::Value> argv[] = { jsDoc, NanNew<v8::Integer>(doc->document()->length()) };
	pdfPageFactory->Call(jsDoc, 2, argv);

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
	PdfDocumentController::Init(exports);
}

NODE_MODULE(pdfutils, init)
