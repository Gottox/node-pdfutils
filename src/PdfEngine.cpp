/*
 * backend.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfEngine.h"

PdfEngine::PdfEngine() {
	_password = NULL;
}

PdfEngine::~PdfEngine() {

}

bool PdfEngine::isThreadSafe() {
	return false;
}
