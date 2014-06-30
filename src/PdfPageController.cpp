/*
 * PdfPageController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfPageController.h"
#include "PdfPage.h"
#include "v8utils.h"

v8::Persistent<v8::Function> PdfPageController::constructor;

void PdfPageController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("PdfPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(v8::String::NewSymbol("PdfPage"), constructor);
}

v8::Handle<v8::Value> PdfPageController::New(const v8::Arguments& args) {
	v8::HandleScope scope;
	new PdfPageController(args);
	return scope.Close(args.This());
}

PdfPageController::PdfPageController(const v8::Arguments &args) {
	this->Wrap(args.This());
	setPage(new PdfPage());
	v8Super(args);
}

void PdfPageController::toJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	obj->Set(v8::String::NewSymbol("label"), charToV8(page->label()));
	obj->Set(v8::String::NewSymbol("width"), v8::Number::New(page->width()));
	obj->Set(v8::String::NewSymbol("height"), v8::Number::New(page->height()));
}

void PdfPageController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	page->setLabel(v8ToChar(obj->Get(v8::String::NewSymbol("label"))));
	page->setWidth(v8ToDouble(obj->Get(v8::String::NewSymbol("width"))));
	page->setHeight(v8ToDouble(obj->Get(v8::String::NewSymbol("height"))));
}

PdfEngine *PdfPageController::engine() {
	return _engine;
}
PdfPage *PdfPageController::page() {
	return _page;
}

void PdfPageController::setEngine(PdfEngine *engine) {
	_engine = engine;
}
void PdfPageController::setPage(PdfPage *page) {
	_page = page;
}
