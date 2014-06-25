/*
 * PdfPage.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfPage.h"

v8::Persistent<v8::Function> PdfPage::constructor;

void PdfPage::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("PdfPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(v8::String::NewSymbol("PdfPage"), constructor);
}

v8::Handle<v8::Value> PdfPage::New(const v8::Arguments& args) {
	v8::HandleScope scope;
	int i;

	PdfPage* obj = new PdfPage();
	obj->Wrap(args.This());
	v8::Local<v8::Function> super = v8::Local<v8::Function>::Cast(args.Callee()->Get(v8::String::NewSymbol("super_")));
	v8::Local<v8::Value> argv[args.Length()];
	for(i = 0; i < args.Length(); i++)
		argv[i] = args[i];
	super->Call(args.This(), args.Length(), argv);
	return args.This();
}

int PdfPage::page(){
	return _page;
}
const char *PdfPage::label(){
	return _label;
}
int PdfPage::width(){
	return _width;
}
int PdfPage::height(){
	return _height;
}

void PdfPage::setPage(const int page){
	_page = page;
}
void PdfPage::setLabel(const char *label){
	_label = label;
}
void PdfPage::setWidth(const int width){
	_width = width;
}
void PdfPage::setHeight(const int height){
	_height = height;
}
