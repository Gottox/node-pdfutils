/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include "PopplerEngine.h"
#include <glib.h>
#include <cstring>

void
PopplerEngine::Init() {
	g_type_init();
}

PopplerEngine::PopplerEngine() : PdfEngine() {

}

char *
PopplerEngine::openFromData(char *data, size_t length){
	GError *error = NULL;
	char *errStr;

	this->doc = poppler_document_new_from_data(data, length, this->password(), &error);
	if(error != NULL) {
		errStr = new char[strlen(error->message)+1];
		strcpy(errStr, error->message);
		g_error_free(error);
		return errStr;
	}
	return NULL;
}

char *
PopplerEngine::openFromPath(char *src) {
	GError *error = NULL;
	char *errStr;

	gchar *uri = g_filename_to_uri(src, NULL, &error);

	if(error == NULL) {
		this->doc = poppler_document_new_from_file(uri, this->password(), &error);
		g_free(uri);
	}

	if(error != NULL) {
		errStr = new char[strlen(error->message)+1];
		strcpy(errStr, error->message);
		g_error_free(error);
		return errStr;
	}
	return NULL;
}

void
PopplerEngine::fillDocument(PdfDocument *document) {
	document->setAuthor(poppler_document_get_author(this->doc));
	document->setCreationDate(poppler_document_get_creation_date(this->doc));
	document->setCreator(poppler_document_get_creator(this->doc));
	//document->setFormat(poppler_document_get_format(this->doc));
	document->setKeywords(poppler_document_get_keywords(this->doc));
	document->setLinearized(poppler_document_is_linearized(this->doc));
	document->setMetadata(poppler_document_get_metadata(this->doc));
	document->setModDate(poppler_document_get_modification_date(this->doc));
	//document->setPageLayout(poppler_document_get_(this->doc));
	//document->setPageMode(poppler_document_get(this->doc));
	//document->setPermissions(poppler_document_get(this->doc));
	document->setProducer(poppler_document_get_producer(this->doc));
	document->setSubject(poppler_document_get_subject(this->doc));
	document->setTitle(poppler_document_get_title(this->doc));
}

void PopplerEngine::fillPage(int index, PdfPage *page) {
	double width, height;
	PopplerPage *pg = poppler_document_get_page(this->doc, index);
	page->setLabel(poppler_page_get_label(pg));
	poppler_page_get_size(pg, &width, &height);
	page->setWidth(width);
	page->setHeight(height);
}

void PopplerEngine::close() {
}

PDF_ENGINE(poppler, PopplerEngine, "gpl")
