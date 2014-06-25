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
	PopplerEngine() : PdfEngine() {
	
	}

	virtual int openFromData(char *data, size_t length){
		return 0;
	}

	virtual int openFromFd(int fd) {
		return 0;
	}

	virtual void fillDocument(const PdfDocument *document) {
	}
	virtual void fillPage(int index, const PdfPage *page) {
	}

	virtual void close() {
	}

};

PDF_ENGINE(poppler, "gpl", PopplerEngine::NewInstance)
