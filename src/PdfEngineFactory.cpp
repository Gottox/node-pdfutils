/*
 * backend.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfEngineFactory.h"

PdfEngineFactory::PdfEngineFactory(v8::Handle<v8::Object> &exports,
		const char *name,
		// Macro args:
		const char *license,
		const PdfEngineInit pdfInit) {
	v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New();
	tpl->SetInternalFieldCount(1);
	v8::Local<v8::Object> jsobj = tpl->NewInstance();
	this->Wrap(jsobj);
	jsobj->Set(v8::String::NewSymbol("license"), v8::String::New(license));
	jsobj->Set(v8::String::NewSymbol("name"), v8::String::New(name));
	exports->Set(v8::String::NewSymbol("handle"), jsobj);
}

PdfEngine *PdfEngineFactory::newInstance() {
	return this->init();
}
