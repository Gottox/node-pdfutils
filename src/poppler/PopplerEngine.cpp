/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include "PopplerEngine.h"

PopplerEngine::PopplerEngine() : PdfEngine() {

}

int PopplerEngine::openFromData(char *data, size_t length){
	return 0;
}

int PopplerEngine::openFromFd(int fd) {
	return 0;
}

void PopplerEngine::fillDocument(const PdfDocument *document) {
}
void PopplerEngine::fillPage(int index, const PdfPage *page) {
}

void PopplerEngine::close() {
}

PDF_ENGINE(poppler, "gpl", PopplerEngine::NewInstance)
