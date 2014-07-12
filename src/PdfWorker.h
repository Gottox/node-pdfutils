/*
 * PdfWorker.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFWORKER_H
#define PDFWORKER_H

#include <nan.h>
#include "PdfController.h"

template <class T>
class PdfWorker : public NanAsyncWorker {
private:
	T *_controller;

public:
	PdfWorker(T *controller, NanCallback *callback) : NanAsyncWorker(callback) {
		_controller = controller;
	}

	T *controller() {
		return _controller;
	}
};

#endif /* !PDFWORKER_H */
