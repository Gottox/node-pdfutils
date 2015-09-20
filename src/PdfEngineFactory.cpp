/*
 * backend.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfEngineFactory.h"

PdfEngineFactory::PdfEngineFactory(v8::Handle<v8::Object> &exports,
			const char *name,
			const PdfEngineInit pdfInit,
			// Macro args:
			const char *license) {
	v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New();
	tpl->SetInternalFieldCount(1);
	v8::Local<v8::Object> jsobj = tpl->NewInstance();
	this->init = pdfInit;
	this->Wrap(jsobj);
	jsobj->Set(Nan::New<v8::String>("license").ToLocalChecked(), Nan::New<v8::String>(license).ToLocalChecked());
	jsobj->Set(Nan::New<v8::String>("name").ToLocalChecked(), Nan::New<v8::String>(name).ToLocalChecked());

	exports->Set(Nan::New<v8::String>("engine").ToLocalChecked(), jsobj);
}

PdfEngine *PdfEngineFactory::newInstance() {
	return this->init();
}
