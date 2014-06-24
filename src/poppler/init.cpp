/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include <node.h>
#include <v8.h>
#include "../PdfEngineFactory.h"

class PopplerEngine : public PdfEngine {
public:
	static PdfEngine *NewInstance() {
		return new PopplerEngine();
	}

private:
	PopplerEngine() {
	
	}

	virtual int loadFromData(char *data, size_t length) {
		return -1;
	}

	virtual int loadFromFd(int fd) {
		return -1;
	}

	virtual int pageCount() {
		return 0;
	}

	virtual PdfPage *page(int number) {
		return NULL;
	}

	virtual char *author() {
		return NULL;
	}

	virtual int creationDate() {
		return -1;
	}

	virtual char *creator() {
		return NULL;
	}

	virtual char *format() {
		return NULL;
	}

	virtual std::vector<char *> keywords() {
		return std::vector<char *>();
	}

	virtual bool linearized() {
		return NULL;
	}

	virtual char *metadata() {
		return NULL;
	}

	virtual int modDate() {
		return 0;
	}

	virtual enum PageLayout pageLayout() {
		return PAGE_LAYOUT_UNSET;
	}

	virtual enum PageMode pageMode() {
		return PAGE_MODE_UNSET;
	}

	virtual enum Permission permissions() {
		return PERMISSIONS_FULL;
	}

	virtual char *producer() {
		return NULL;
	}

	virtual char *subject() {
		return NULL;
	}

	virtual char *title() {
		return NULL;
	}
};

PDF_ENGINE(poppler, "gpl", PopplerEngine::NewInstance)
