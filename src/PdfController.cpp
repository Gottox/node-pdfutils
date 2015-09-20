/*
 * PdfController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfController.h"

PdfController::~PdfController() {
	if(_engine != NULL)
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
		obj->Set(Nan::New<v8::String>(key).ToLocalChecked(), Nan::Null());
	else
		obj->Set(Nan::New<v8::String>(key).ToLocalChecked(), Nan::New<v8::String>(value).ToLocalChecked());
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const int value) {
	obj->Set(Nan::New<v8::String>(key).ToLocalChecked(), Nan::New<v8::Integer>(value));
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const double value) {
	obj->Set(Nan::New<v8::String>(key).ToLocalChecked(), Nan::New<v8::Number>(value));
}

void PdfController::set(v8::Handle<v8::Object> obj, const char *key, const bool value) {
	obj->Set(Nan::New<v8::String>(key).ToLocalChecked(), Nan::New<v8::Boolean>(value));
}
