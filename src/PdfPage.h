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
#include "jsShim.h"

class PdfPage : public node::ObjectWrap {
	JS_SHIM_H

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
