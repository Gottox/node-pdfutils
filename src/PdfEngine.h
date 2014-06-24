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

class PdfEngine : public node::ObjectWrap {
protected:
	PdfEngine();
	virtual ~PdfEngine();

public:
	v8::Local<v8::Object> wrapJs();
	virtual bool isThreadSafe();
};

#endif /* !PDFENGINE_H */
