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
#include <v8.h>

class PdfEngine;
/**
 * @brief Class representation of a PDF-Controller.
 *
 * This class should be filled from a PDFEngine
 */
class PdfController : public node::ObjectWrap {
private:
	static v8::Persistent<v8::Function> constructor;
	/**
	 * @brief Constructor for v8.
	 * @param args arguments from Javascript. Currently not used
	 */
	PdfController(const v8::Arguments& args);

	PdfEngine *_engine;
	PdfDocument *_document;

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
	 * @brief engine which is used for this document
	 */
	PdfEngine *engine();
	/**
	 * @brief  the document model object
	 */
	PdfDocument *document();

	/**
	 * @brief sets engine which is used for this document
	 */
	void setEngine(PdfEngine *engine);
	/**
	 * @brief sets the document model object
	 */
	void setDocument(PdfDocument *document);
};

#endif /* !PDFCONTROLLER_H */
