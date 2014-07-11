/*
 * PdfPageController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <nan.h>
#include "PdfPageController.h"
#include "PdfPage.h"

void PdfPageController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
	tpl->SetClassName(NanNew<v8::String>("PdfPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	exports->Set(NanNew<v8::String>("PdfPage"), tpl->GetFunction());
}

NAN_METHOD(PdfPageController::New) {
	NanScope();
	PdfPageController *controller = new PdfPageController();
	controller->Wrap(args.This());
	controller->setPage(new PdfPage());
	NanReturnValue(args.This());
}

void PdfPageController::toJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	this->set(obj, "label", page->label());
	this->set(obj, "width", page->width());
	this->set(obj, "height", page->height());
}

void PdfPageController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfPage *page = this->page();
	size_t count;
	page->setLabel(NanCString(obj->Get(NanNew<v8::String>("label")), &count));
	page->setWidth(obj->Get(NanNew<v8::String>("width"))->NumberValue());
	page->setHeight(obj->Get(NanNew<v8::String>("height"))->NumberValue());

}

PdfPage *PdfPageController::page() {
	return _page;
}

void PdfPageController::setPage(PdfPage *page) {
	_page = page;
}
