/*
 * PopplerEngine.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef POPPLERENGINE_H
#define POPPLERENGINE_H

#include <node.h>
#include <v8.h>
#include "../PdfEngineFactory.h"
#include <glib/poppler.h>

class PopplerEngine : public PdfEngine {
public:
	static void Init();
	PopplerEngine();

	static PdfEngine *NewInstance() {
		return new PopplerEngine();
	}

	virtual char *openFromData(char *data, size_t length);
	virtual char *openFromPath(char *src);
	virtual void fillDocument(PdfController *document);
	virtual void fillPage(int index, PdfPage *page);
	virtual void close();

private:
	PopplerDocument *doc;
};

#endif /* !POPPLERENGINE_H */
