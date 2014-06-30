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
	obj->Set(v8::String::NewSymbol("label"), charToV8(this->label()));
	obj->Set(v8::String::NewSymbol("width"), v8::Number::New(this->width()));
	obj->Set(v8::String::NewSymbol("height"), v8::Number::New(this->height()));
}

void PdfPageController::fromJs(v8::Handle<v8::Object> &obj) {
	this->setLabel(v8ToChar(obj->Get(v8::String::NewSymbol("label"))));
	this->setWidth(v8ToDouble(obj->Get(v8::String::NewSymbol("width"))));
	this->setHeight(v8ToDouble(obj->Get(v8::String::NewSymbol("height"))));
}

PdfEngine *PdfPageController::engine(){
	return _engine;
}
PdfPage *PdfPageController::page(){
	return _page;
}

int PdfPageController::index(){
	return _index;
}
const char *PdfPageController::label(){
	return _label;
}
double PdfPageController::width(){
	return _width;
}
double PdfPageController::height(){
	return _height;
}

void PdfPageController::setEngine(PdfEngine *engine){
	_engine = engine;
}
void PdfPageController::setPage(PdfPage *page){
	_page = page;
}
void PdfPageController::setIndex(const int index){
	_index = index;
}
void PdfPageController::setLabel(const char *label){
	_label = label;
}
void PdfPageController::setWidth(const double width){
	_width = width;
}
void PdfPageController::setHeight(const double height){
	_height = height;
}
