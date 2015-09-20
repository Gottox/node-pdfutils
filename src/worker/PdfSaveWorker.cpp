/*
 * PdfSaveWorker.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfSaveWorker.h"
#include "../PdfPageController.h"

void
PdfSaveWorker::Execute() {
	CallIntermediate(NULL);
}

void
PdfSaveWorker::HandleIntermediate(void *data) {

}

void
PdfSaveWorker::HandleOKCallback () {
	Nan::HandleScope scope;
	v8::Local<v8::Value> argv[] = { };
	controller()->toJs();
	if(callback)
		callback->Call(0, argv);
}
