/*
 * PdfDocument.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include "PdfPage.h"
#include "PdfEngine.h"
#include "jsShim.h"
#include <node.h>
#include <v8.h>
 
class PdfEngine;
/**
 * @brief Class representation of a PDF-Document.
 *
 * This class should be filled from a PDFEngine
 */
class PdfDocument : public node::ObjectWrap {
	JS_SHIM_H

private:
	PdfDocument(const v8::Arguments& args) {
		
	}
	PdfEngine *_engine;
	const char *_author;
	int _creationDate;
	const char *_creator;
	const char *_format;
	std::vector<char *> _keywords;
	bool _linearized;
	const char *_metadata;
	int _modDate;
	enum PageLayout _pageLayout;
	enum PageMode _pageMode;
	enum Permission _permissions;
	const char *_producer;
	const char *_subject;
	const char *_title;

public:
	PdfEngine *engine();
	const char *author();
	int creationDate();
	const char *creator();
	const char *format();
	std::vector<const char *> keywords();
	bool linearized();
	const char *metadata();
	int modDate();
	enum PageLayout pageLayout();
	enum PageMode pageMode();
	enum Permission permissions();
	const char *producer();
	const char *subject();
	const char *title();

	void setEngine(PdfEngine *engine);
	void setAuthor(const char *author);
	void setCreationDate(const int creationDate);
	void setCreator(const char *creator);
	void setFormat(const char *format);
	void setKeywords(const std::vector <const char *> keywords);
	void setLinearized(const bool linearized);
	void setMetadata(const char *metadata);
	void setModDate(const int modDate);
	void setPageLayout(const enum PageLayout pageLayout);
	void setPageMode(const enum PageMode pageMode);
	void setPermissions(const enum Permission permissions);
	void setProducer(const char *producer);
	void setSubject(const char *subject);
	void setTitle(const char *title);
};

#endif /* !PDFDOCUMENT_H */
