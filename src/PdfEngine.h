/*
 * PdfEngine.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFENGINE_H
#define PDFENGINE_H

#include <v8.h>
#include <node.h>
#include <vector>
#include "pdfutils.h"
#include "PdfPageController.h"
#include "PdfDocument.h"

class PdfPageController;
class PdfDocument;
/**
 * @brief Base Class for interaction with a PDF-Engine such as Poppler
 *
 * This class operates as binder between the PDF-Engine and the PDFDocument.
 * It may operate from multiple threads.
 */
class PdfEngine {
private:
	const char *_password;

protected:
	PdfEngine();
	virtual ~PdfEngine();

public:
	inline void setPassword(const char *password) {
		_password = password;
	}

	inline const char* password() {
		return _password;
	}

	inline void error(char *msg) {
	
	}

	virtual bool isThreadSafe();

	virtual char* openFromData(char *data, size_t length) = 0;
	virtual char* openFromPath(char *src) = 0;

	virtual void fillDocument(PdfDocument *document) = 0;
	virtual void fillPage(int index, PdfPageController *page) = 0;

	virtual void close() = 0;

};

#endif /* !PDFENGINE_H */
