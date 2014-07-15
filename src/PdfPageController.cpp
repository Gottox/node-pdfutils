/*
 * PdfPageController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <nan.h>
#include "PdfPageController.h"
#include "PdfPage.h"
#include "PdfWorker.h"
#include "PdfExportPageWorker.h"

void PdfPageController::Init(v8::Handle<v8::Object> exports) {
	// Public
	v8::Local<v8::FunctionTemplate> pub = NanNew<v8::FunctionTemplate>(New);
	pub->SetClassName(NanNew<v8::String>("PdfPage"));
	pub->InstanceTemplate()->SetInternalFieldCount(1);

	// Private
	v8::Local<v8::Object> prv = NanNew<v8::Object>();
	prv->Set(NanNew<v8::String>("as"),
				NanNew<v8::FunctionTemplate>(PdfPageController::As)->GetFunction());

	exports->Set(NanNew<v8::String>("PdfPage"), pub->GetFunction());
	exports->Set(NanNew<v8::String>("_page"), prv);
}

NAN_METHOD(PdfPageController::New) {
	NanScope();
	PdfPageController *controller = new PdfPageController();
	controller->Wrap(args.This());
	controller->setPage(new PdfPage());
	NanReturnValue(args.This());
}

NAN_METHOD(PdfPageController::As) {
	NanScope();
	NanReturnUndefined();
}

void PdfPageController::toJs() {
	PdfPage *page = this->page();
	v8::Local<v8::Object> obj = NanObjectWrapHandle(this);
	this->set(obj, "label", page->label());
	this->set(obj, "width", page->width());
	this->set(obj, "height", page->height());
}

void PdfPageController::fromJs() {
	PdfPage *page = this->page();
	size_t count;
	v8::Local<v8::Object> obj = NanObjectWrapHandle(this);
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
