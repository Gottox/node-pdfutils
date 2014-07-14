/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include "PdfEngine.h"
#include "PdfEngineFactory.h"
#include "PdfDocumentController.h"
#include "PdfPageController.h"

void init(v8::Handle<v8::Object> exports) {
	PdfPageController::Init(exports);
	PdfDocumentController::Init(exports);
}

NODE_MODULE(pdfutils, init)
