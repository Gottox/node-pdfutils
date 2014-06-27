/*
 * PdfPage.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfPage.h"
#include "v8utils.h"

JS_SHIM(PdfPage)

PdfPage::PdfPage(const v8::Arguments &args) : JsShim(args) {

}

void PdfPage::toJs(v8::Handle<v8::Object> &obj) {
	obj->Set(v8::String::NewSymbol("label"), charToV8(this->label()));
	obj->Set(v8::String::NewSymbol("width"), v8::Number::New(this->width()));
	obj->Set(v8::String::NewSymbol("height"), v8::Number::New(this->height()));
}

void PdfPage::fromJs(v8::Handle<v8::Object> &obj) {
	this->setLabel(v8ToChar(obj->Get(v8::String::NewSymbol("label"))));
	this->setWidth(v8ToDouble(obj->Get(v8::String::NewSymbol("width"))));
	this->setHeight(v8ToDouble(obj->Get(v8::String::NewSymbol("height"))));
}

PdfEngine *PdfPage::engine(){
	return _engine;
}

int PdfPage::index(){
	return _index;
}
const char *PdfPage::label(){
	return _label;
}
double PdfPage::width(){
	return _width;
}
double PdfPage::height(){
	return _height;
}

void PdfPage::setEngine(PdfEngine *engine){
	_engine = engine;
}
void PdfPage::setIndex(const int index){
	_index = index;
}
void PdfPage::setLabel(const char *label){
	_label = label;
}
void PdfPage::setWidth(const double width){
	_width = width;
}
void PdfPage::setHeight(const double height){
	_height = height;
}
