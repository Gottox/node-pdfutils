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

	virtual bool isThreadSafe() {
		return false;
	}
	virtual int loadFromData(char *data, size_t length) {
		return 0;
	}
	virtual int loadFromFd(int fd) {
		return 0;
	}

	virtual int pageCount() {
		return 0;
	}
};

PDF_ENGINE(poppler, "gpl", PopplerEngine::NewInstance)
