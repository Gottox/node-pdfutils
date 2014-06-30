/*
 * PdfDocument.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include "PdfEngine.h"

class PdfEngine;
/**
 * @brief Class representation of a PDF-Document.
 *
 * This class should be filled from a PDFEngine
 */
class PdfDocument {
private:
	int _length;
	const char *_author;
	int _creationDate;
	const char *_creator;
	const char *_format;
	const char * _keywords;
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
	/**
	 * @brief number of pages in this docoument
	 */
	int length();
	/**
	 * @brief author of this document
	 */
	const char *author();
	/**
	 * @brief creation date of this document
	 */
	int creationDate();
	/**
	 * @brief creator of this document
	 */
	const char *creator();
	/**
	 * @brief format of this document
	 */
	const char *format();
	/**
	 * @brief keywords of this document
	 */
	const char *keywords();
	/**
	 * @brief whether this document is linearized
	 */
	bool linearized();
	/**
	 * @brief metadata of this document
	 */
	const char *metadata();
	/**
	 * @brief modification date of this document
	 */
	int modDate();
	/**
	 * @brief page layout of this document
	 */
	enum PageLayout pageLayout();
	/**
	 * @brief page mode of this document
	 */
	enum PageMode pageMode();
	/**
	 * @brief permissions of this document
	 */
	enum Permission permissions();
	/**
	 * @brief producer of this document
	 */
	const char *producer();
	/**
	 * @brief subject of this document
	 */
	const char *subject();
	/**
	 * @brief title of this document
	 */
	const char *title();

	/**
	 * @brief set number of pages in this document
	 */
	void setLength(const int length);
	/**
	 * @brief sets author of this document
	 */
	void setAuthor(const char *author);
	/**
	 * @brief sets creation date of this document
	 */
	void setCreationDate(const int creationDate);
	/**
	 * @brief sets creator of this document
	 */
	void setCreator(const char *creator);
	/**
	 * @brief sets format of this document
	 */
	void setFormat(const char *format);
	/**
	 * @brief sets keywords of this document
	 */
	void setKeywords(const char * keywords);
	/**
	 * @brief sets whether this document is linearized
	 */
	void setLinearized(const bool linearized);
	/**
	 * @brief sets modification date of this document
	 */
	void setMetadata(const char *metadata);
	/**
	 * @brief sets page layout of this document
	 */
	void setModDate(const int modDate);
	/**
	 * @brief sets page layout of this document
	 */
	void setPageLayout(const enum PageLayout pageLayout);
	/**
	 * @brief sets page mode of this document
	 */
	void setPageMode(const enum PageMode pageMode);
	/**
	 * @brief sets permissions of this document
	 */
	void setPermissions(const enum Permission permissions);
	/**
	 * @brief sets producer of this document
	 */
	void setProducer(const char *producer);
	/**
	 * @brief sets subject of this document
	 */
	void setSubject(const char *subject);
	/**
	 * @brief sets title of this document
	 */
	void setTitle(const char *title);
};

#endif /* !PDFDOCUMENT_H */
