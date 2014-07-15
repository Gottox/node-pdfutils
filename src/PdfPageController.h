/*
 * PdfPageController.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFPAGECONTROLLER_H
#define PDFPAGECONTROLLER_H

#include <v8.h>
#include <node.h>
#include <nan.h>
#include "PdfController.h"
#include "PdfPage.h"

class PdfEngine;
/**
 * @brief Class representation of a PDF-Page.
 *
 * This class should be filled from a PDFEngine
 */
class PdfPageController : public PdfController {
private:
	PdfPageController() : PdfController() {};
	PdfPage *_page;

public:
	/**
	 * @brief exports this class to v8
	 */
	static void Init(v8::Handle<v8::Object> exports);
	/**
	 * @brief generates new object from v8
	 */
	static NAN_METHOD(New);
	/**
	 * @brief converts this page to format
	 */
	static NAN_METHOD(As);
	/**
	 * @brief writes state of this object to a JS-Object
	 */
	virtual void toJs();
	/**
	 * @brief reads state of this object from a JS-Object
	 */
	virtual void fromJs();
	/**
	 * @brief page
	 */
	PdfPage *page();
	/**
	 * @brief sets model of this page
	 */
	void setPage(PdfPage *);
};

#endif /* !PDFPAGECONTROLLER_H */
