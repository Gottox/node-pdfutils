/*
 * PdfExportPageWorker.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfExportPageWorker.h"
#include "../PdfPageController.h"

void
PdfExportPageWorker::Execute() {
	CallIntermediate(NULL);
}

void
PdfExportPageWorker::HandleIntermediate(void *data) {

}

void
PdfExportPageWorker::HandleOKCallback () {
	NanScope();
	v8::Local<v8::Value> argv[] = { };
	controller()->toJs();
	if(callback)
		callback->Call(0, argv);
}
