/*
 * PdfPage.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfPage.h"
#include "jsShim.h"

JS_SHIM(PdfPage)

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
