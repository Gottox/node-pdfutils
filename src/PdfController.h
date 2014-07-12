/*
 * PdfController.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFCONTROLLER_H
#define PDFCONTROLLER_H

#include "PdfEngine.h"
#include <node.h>
#include <nan.h>
#include <v8.h>
#include "PdfDocument.h"

class PdfEngine;
/**
 * @brief Class representation of a PDF-Controller.
 *
 * This class should be filled from a PDFEngine
 */
class PdfController : public node::ObjectWrap {
protected:
	/**
	 * @brief Constructor for v8.
	 */
	PdfController() {};

	void set(v8::Handle<v8::Object> obj, const char key[], const char* value);
	void set(v8::Handle<v8::Object> obj, const char key[], const int value);
	void set(v8::Handle<v8::Object> obj, const char key[], const double value);
	void set(v8::Handle<v8::Object> obj, const char key[], const bool value);

private:
	PdfEngine *_engine;

public:
	/**
	 * @brief writes state of this object to a JS-Object
	 */
	virtual void toJs() = 0;
	/**
	 * @brief reads state of this object from a JS-Object
	 */
	virtual void fromJs() = 0;
	/**
	 * @brief engine which is used for this document
	 */
	PdfEngine *engine();

	/**
	 * @brief sets engine which is used for this document
	 */
	void setEngine(PdfEngine *engine);
};

#endif /* !PDFCONTROLLER_H */
