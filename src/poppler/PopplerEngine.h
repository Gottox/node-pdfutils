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

class PopplerEngine : public PdfEngine {
public:
	static PdfEngine *NewInstance() {
		return new PopplerEngine();
	}

	virtual int openFromData(char *data, size_t length);
	virtual int openFromFd(int fd);
	virtual void fillDocument(const PdfDocument *document);
	virtual void fillPage(int index, const PdfPage *page);
	virtual void close();

private:
	PopplerEngine();


};

#endif /* !POPPLERENGINE_H */
