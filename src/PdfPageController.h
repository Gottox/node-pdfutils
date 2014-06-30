/*
 * PdfPageController.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFPAGECONTROLLER_H
#define PDFPAGECONTROLLER_H

#include "pdfutils.h"
#include "PdfPage.h"
#include <v8.h>
#include <node.h>

class PdfEngine;
/**
 * @brief Class representation of a PDF-Page.
 *
 * This class should be filled from a PDFEngine
 */
class PdfPageController : public node::ObjectWrap {
private:
	static v8::Persistent<v8::Function> constructor;
	PdfPageController(const v8::Arguments &args);
	PdfEngine *_engine;
	PdfPage *_page;

public:
	/**
	 * @brief exports this class to v8
	 */
	static void Init(v8::Handle<v8::Object> exports);
	/**
	 * @brief generates new object from v8
	 */
	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	/**
	 * @brief writes state of this object to a JS-Object
	 */
	virtual void toJs(v8::Handle<v8::Object> &obj);
	/**
	 * @brief reads state of this object from a JS-Object
	 */
	virtual void fromJs(v8::Handle<v8::Object> &obj);
	/**
	 * @brief engine used to create this page
	 */
	PdfEngine *engine();
	/**
	 * @brief page
	 */
	PdfPage *page();

	/**
	 * @brief sets engine of this page
	 */
	void setEngine(PdfEngine *);
	/**
	 * @brief sets engine of this page
	 */
	void setPage(PdfPage *);
};

#endif /* !PDFPAGECONTROLLER_H */
