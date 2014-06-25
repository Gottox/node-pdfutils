/*
 * PdfPage.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFPAGE_H
#define PDFPAGE_H

#include "pdfutils.h"
#include "PdfEngine.h"
#include <node.h>
#include <v8.h>

class PdfPage : public node::ObjectWrap {
public:
	static void Init(v8::Handle<v8::Object> exports);
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Persistent<v8::Function> constructor;

	int _page;
	const char *_label;
	int _width;
	int _height;

	int page();
	const char *label();
	int width();
	int height();

	void setPage(const int page);
	void setLabel(const char *label);
	void setWidth(const int width);
	void setHeight(const int height);
};

#endif /* !PDFPAGE_H */
