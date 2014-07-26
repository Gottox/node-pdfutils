/*
 * PdfController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfController.h"

PdfController::~PdfController() {
	delete _engine;
}

PdfEngine *PdfController::engine() {
	return _engine;
}

void PdfController::setEngine(PdfEngine *engine) {
	_engine = engine;
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const char* value) {
	if(value == NULL)
		obj->Set(NanNew<v8::String>(key), NanNull());
	else
		obj->Set(NanNew<v8::String>(key), NanNew<v8::String>(value));
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const int value) {
	obj->Set(NanNew<v8::String>(key), NanNew<v8::Integer>(value));
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const double value) {
	obj->Set(NanNew<v8::String>(key), NanNew<v8::Number>(value));
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const bool value) {
	obj->Set(NanNew<v8::String>(key), NanNew<v8::Boolean>(value));
}
