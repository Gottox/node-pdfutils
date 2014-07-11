/*
 * PdfPageController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <nan.h>
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

NAN_METHOD(PdfPageController::New) {
	NanScope();
	new PdfPageController(args);
	NanReturnValue(args.This());
}

PdfPageController::PdfPageController(const v8::Arguments &args) {
	this->Wrap(args.This());
	setPage(new PdfPage());
	v8Super(args);
}

void PdfPageController::toJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	obj->Set(NanNew<v8::String>("label"), NanNew<v8::String>(page->label()));
	obj->Set(NanNew<v8::String>("width"), NanNew<v8::Number>(page->width()));
	obj->Set(NanNew<v8::String>("height"), NanNew<v8::Number>(page->height()));
}

void PdfPageController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	size_t count;
	page->setLabel(NanCString(obj->Get(NanNew<v8::String>("label")), &count));
	page->setWidth(v8ToDouble(obj->Get(NanNew<v8::String>("width"))));
	page->setHeight(v8ToDouble(obj->Get(NanNew<v8::String>("height"))));

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
