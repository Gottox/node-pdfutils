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
Handle<Value> openFromPath(const Arguments& args) {
	int i;
	char *error;
	HandleScope scope;
	v8::Local<v8::Object> jsDoc = args[0]->ToObject();
	v8::Local<v8::Function> pdfPageFactory = v8::Function::Cast(*args[1]);
	PdfDocument *doc = node::ObjectWrap::Unwrap<PdfDocument>(jsDoc);
	v8::Local<v8::Object> jsEngine = jsDoc->Get(v8::String::NewSymbol("_engine"))->ToObject();

	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	PdfEngine *engine = factory->newInstance();
	engine->setPassword(v8ToChar(args[2]));
	doc->setEngine(engine);

	char *src = v8ToChar(args[3]);
	if((error = engine->openFromPath(src))) {
		THROW_FREE(Error, error);
	}
	engine->fillDocument(doc);
	doc->toJs(jsDoc);

	Handle< Value > argv[] = { jsDoc, v8::Integer::New(doc->length()) };
	pdfPageFactory->Call(v8::Context::GetCurrent()->Global(), 2, argv);

	for(i = 0; i < doc->length(); i++) {
		v8::Local<v8::Object> jsPage = jsDoc->Get(i)->ToObject();
		PdfPage *page = node::ObjectWrap::Unwrap<PdfPage>(jsPage);
		engine->fillPage(i, page);
		page->toJs(jsPage);
	}

	return scope.Close(jsDoc);
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("openFromPath"),
			FunctionTemplate::New(openFromPath)->GetFunction());

	PdfPage::Init(exports);
	PdfDocument::Init(exports);
}

NODE_MODULE(pdfutils, init)
