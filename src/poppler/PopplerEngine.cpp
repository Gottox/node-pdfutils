/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include "PopplerEngine.h"
#include <glib.h>
#include <cstring>


enum PdfPageLayout getPageLayout(PopplerPageLayout layout) {
	switch(layout) {
		case POPPLER_PAGE_LAYOUT_UNSET:
			return PAGE_LAYOUT_UNSET;
		case POPPLER_PAGE_LAYOUT_SINGLE_PAGE:
			return PAGE_LAYOUT_SINGLE_PAGE;
		case POPPLER_PAGE_LAYOUT_ONE_COLUMN:
			return PAGE_LAYOUT_ONE_COLUMN;
		case POPPLER_PAGE_LAYOUT_TWO_COLUMN_LEFT:
			return PAGE_LAYOUT_TWO_COLUMN_LEFT;
		case POPPLER_PAGE_LAYOUT_TWO_COLUMN_RIGHT:
			return PAGE_LAYOUT_TWO_COLUMN_RIGHT;
		case POPPLER_PAGE_LAYOUT_TWO_PAGE_LEFT:
			return PAGE_LAYOUT_TWO_PAGE_LEFT;
		case POPPLER_PAGE_LAYOUT_TWO_PAGE_RIGHT:
			return PAGE_LAYOUT_TWO_PAGE_RIGHT;
	}
	return PAGE_LAYOUT_UNSET;
};

enum PdfPageMode getPageMode(PopplerPageMode mode) {
	switch(mode) {
	case POPPLER_PAGE_MODE_UNSET:
		return PAGE_MODE_UNSET;
	case POPPLER_PAGE_MODE_NONE:
		return PAGE_MODE_NONE;
	case POPPLER_PAGE_MODE_USE_OUTLINES:
		return PAGE_MODE_USE_OUTLINES;
	case POPPLER_PAGE_MODE_USE_THUMBS:
		return PAGE_MODE_USE_THUMBS;
	case POPPLER_PAGE_MODE_FULL_SCREEN:
		return PAGE_MODE_FULL_SCREEN;
	case POPPLER_PAGE_MODE_USE_OC:
		return PAGE_MODE_USE_OC;
	case POPPLER_PAGE_MODE_USE_ATTACHMENTS:
		return PAGE_MODE_USE_ATTACHMENTS;
	}
	return PAGE_MODE_UNSET;
};

void
PopplerEngine::Init() {
#if !GLIB_CHECK_VERSION(2, 36, 0)
	g_type_init();
#endif
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
	int perm;
	document->setLength(poppler_document_get_n_pages(this->doc));
	document->setAuthor(poppler_document_get_author(this->doc));
	document->setCreationDate(poppler_document_get_creation_date(this->doc));
	document->setCreator(poppler_document_get_creator(this->doc));
	//document->setFormat(poppler_document_get_format(this->doc));
	document->setKeywords(poppler_document_get_keywords(this->doc));
	document->setLinearized(poppler_document_is_linearized(this->doc));
	document->setMetadata(poppler_document_get_metadata(this->doc));
	document->setModDate(poppler_document_get_modification_date(this->doc));
	document->setPageLayout(getPageLayout(poppler_document_get_page_layout(this->doc)));
	document->setPageMode(getPageMode(poppler_document_get_page_mode(this->doc)));
	perm = poppler_document_get_permissions(this->doc);
	document->setPermissions(
			(perm & POPPLER_PERMISSIONS_OK_TO_PRINT ? 1 << PERMISSIONS_PRINT : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_MODIFY ? 1 << PERMISSIONS_MODIFY : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_COPY ? 1 << PERMISSIONS_COPY : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_ADD_NOTES ? 1 << PERMISSIONS_ADD_NOTES : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_FILL_FORM ? 1 << PERMISSIONS_FILL_FORM : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_EXTRACT_CONTENTS ? 1 << PERMISSIONS_EXTRACT_CONTENTS : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_ASSEMBLE ? 1 << PERMISSIONS_ASSEMBLE : 0) |
			(perm & POPPLER_PERMISSIONS_OK_TO_PRINT_HIGH_RESOLUTION ? 1 << PERMISSIONS_PRINT_HIGH_RESOLUTION : 0)
			);
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

char*
PopplerEngine::renderPage(int index, PdfRenderFormat format, PdfWriter &writer) {
	return ERROR_RENDER_NOT_SUPPORTED;
}

char*
PopplerEngine::savePdf(PdfPage *pages, PdfWriter &writer) {
	return ERROR_SAVE_NOT_SUPPORTED;
}


PDF_ENGINE(poppler, PopplerEngine, "gpl")
