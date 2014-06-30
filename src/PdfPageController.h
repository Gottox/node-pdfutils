/*
 * PdfPageController.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFPAGE_H
#define PDFPAGE_H

#include "pdfutils.h"
#include "PdfEngine.h"
#include "JsShim.h"
#include <v8.h>
#include <node.h>

class PdfEngine;
/**
 * @brief Class representation of a PDF-Page.
 *
 * This class should be filled from a PDFEngine
 */
class PdfPageController : public JsShim {
	JS_SHIM_H

private:
	PdfPageController(const v8::Arguments &args);
	PdfEngine *_engine;
	int _index;
	const char *_label;
	double _width;
	double _height;

public:
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
	 * @brief index of this page
	 */
	int index();
	/**
	 * @brief label of this page
	 */
	const char *label();
	/**
	 * @brief width of this page
	 */
	double width();
	/**
	 * @brief height of this page
	 */
	double height();

	/**
	 * @brief sets engine of this page
	 */
	void setEngine(PdfEngine *);
	/**
	 * @brief sets index of this page
	 */
	void setIndex(const int indext);
	/**
	 * @brief sets label of this page
	 */
	void setLabel(const char *label);
	/**
	 * @brief sets width of this page
	 */
	void setWidth(const double width);
	/**
	 * @brief sets height of this page
	 */
	void setHeight(const double height);
};

#endif /* !PDFPAGE_H */
