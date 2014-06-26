/*
 * PdfPage.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfPage.h"

JS_SHIM(PdfPage)

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
