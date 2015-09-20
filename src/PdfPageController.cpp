/*
 * PdfPageController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <nan.h>
#include "PdfPageController.h"
#include "PdfPage.h"
#include "worker/PdfWorker.h"
#include "worker/PdfExportPageWorker.h"

void PdfPageController::Init(v8::Handle<v8::Object> exports) {
	// Public
	v8::Local<v8::FunctionTemplate> pub = Nan::New<v8::FunctionTemplate>(New);
	pub->SetClassName(Nan::New<v8::String>("PdfPage").ToLocalChecked());
	pub->InstanceTemplate()->SetInternalFieldCount(1);

	// Private
	v8::Local<v8::Object> prv = Nan::New<v8::Object>();
	prv->Set(Nan::New<v8::String>("as").ToLocalChecked(),
				Nan::New<v8::FunctionTemplate>(PdfPageController::As)->GetFunction());

	exports->Set(Nan::New<v8::String>("PdfPage").ToLocalChecked(), pub->GetFunction());
	exports->Set(Nan::New<v8::String>("_page").ToLocalChecked(), prv);
}

NAN_METHOD(PdfPageController::New) {
	Nan::HandleScope scope;
	PdfPageController *controller = new PdfPageController();
	controller->Wrap(info.This());
	controller->setPage(new PdfPage());
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(PdfPageController::As) {
	Nan::HandleScope scope;
	info.GetReturnValue().Set(Nan::Undefined());
}

void PdfPageController::toJs() {
	PdfPage *page = this->page();
	v8::Local<v8::Object> obj = this->handle();
	this->set(obj, "label", page->label());
	this->set(obj, "width", page->width());
	this->set(obj, "height", page->height());
}

void PdfPageController::fromJs() {
	PdfPage *page = this->page();
	v8::Local<v8::Object> obj = this->handle();
	page->setLabel(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("label").ToLocalChecked())));
	page->setWidth(obj->Get(Nan::New<v8::String>("width").ToLocalChecked())->NumberValue());
	page->setHeight(obj->Get(Nan::New<v8::String>("height").ToLocalChecked())->NumberValue());
}

PdfPage *PdfPageController::page() {
	return _page;
}

void PdfPageController::setPage(PdfPage *page) {
	_page = page;
}
