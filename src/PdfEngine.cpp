/*
 * backend.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfEngine.h"

PdfEngine::PdfEngine() {

}

PdfEngine::~PdfEngine() {

}

v8::Local<v8::Object> PdfEngine::wrapJs() {
	v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New();
	tpl->SetInternalFieldCount(1);
	v8::Local<v8::Object> jsObj = tpl->NewInstance();
	this->Wrap(jsObj);
	return jsObj;
}

bool PdfEngine::isThreadSafe() {
	return false;
}
