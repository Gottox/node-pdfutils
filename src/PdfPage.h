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
#include <v8.h>
#include <node.h>

class PdfEngine;
/**
 * @brief Class representation of a PDF-Page.
 *
 * This class should be filled from a PDFEngine
 */
class PdfPage : public node::ObjectWrap {
	JS_SHIM_H

private:
	PdfPage(const v8::Arguments &args) {
		
	}
	PdfEngine *_engine;
	int _page;
	const char *_label;
	int _width;
	int _height;

public:
	PdfEngine *engine();
	int page();
	const char *label();
	int width();
	int height();

	void setEngine(PdfEngine *);
	void setPage(const int page);
	void setLabel(const char *label);
	void setWidth(const int width);
	void setHeight(const int height);
};

#endif /* !PDFPAGE_H */
