/*
 * PdfController.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFDOCUMENTCONTROLLER_H
#define PDFDOCUMENTCONTROLLER_H

#include "PdfEngine.h"
#include <node.h>
#include <nan.h>
#include <v8.h>
#include "PdfController.h"
#include "PdfDocument.h"

class PdfEngine;
/**
 * @brief Class representation of a PDF-Controller.
 *
 * This class should be filled from a PDFEngine
 */
class PdfDocumentController : public PdfController {
private:
	/**
	 * @brief Constructor for v8.
	 * @param args arguments from Javascript. Currently not used
	 */
	PdfDocumentController() : PdfController() {};

	PdfDocument *_document;

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
	 * @brief generates new object from v8
	 */
	static NAN_METHOD(Load);
	/**
	 * @brief generates new object from v8
	 */
	static NAN_METHOD(ToStream);
	/**
	 * @brief writes state of this object to a JS-Object
	 */
	virtual void toJs();
	/**
	 * @brief reads state of this object from a JS-Object
	 */
	virtual void fromJs();

	/**
	 * @brief the document model object
	 */
	PdfDocument *document();
	/**
	 * @brief sets the document model object
	 */
	void setDocument(PdfDocument *document);
};

#endif /* !PDFCONTROLLER_H */
