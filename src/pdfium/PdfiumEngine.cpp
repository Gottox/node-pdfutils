/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include "PdfiumEngine.h"
#include <glib.h>
#include <cstring>


void
PdfiumEngine::Init() {
}

PdfiumEngine::PdfiumEngine() : PdfEngine() {
}

PdfiumEngine::~PdfiumEngine() {
}

char *
PdfiumEngine::openFromData(char *data, size_t length){
	return NULL;
}

char *
PdfiumEngine::openFromPath(char *src) {
	return NULL;
}

void
PdfiumEngine::fillDocument(PdfDocument *document) {
}

void PdfiumEngine::fillPage(int index, PdfPage *page) {
}

void PdfiumEngine::close() {
}

char*
PdfiumEngine::renderPage(int index, PdfRenderFormat format, PdfExportPageWorker *writer) {
	return ERROR_RENDER_NOT_SUPPORTED;
}

char*
PdfiumEngine::savePdf(PdfPage *pages, PdfSaveWorker *writer) {
	return ERROR_SAVE_NOT_SUPPORTED;
}


PDF_ENGINE(poppler, PdfiumEngine, "gpl")
