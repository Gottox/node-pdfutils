/*
 * PdfEngine.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFENGINE_H
#define PDFENGINE_H

#include <v8.h>
#include <node.h>
#include <vector>
#include "pdfutils.h"
#include "PdfPage.h"

class PdfEngine {
private:
	const char *_password;

protected:
	PdfEngine();
	virtual ~PdfEngine();

public:
	inline void setPassword(const char *password) {
		_password = password;
	}

	inline const char* password() {
		return _password;
	}

	virtual bool isThreadSafe();
	virtual int loadFromData(char *data, size_t length) = 0;
	virtual int loadFromFd(int fd) = 0;

	virtual int pageCount() = 0;
};

#endif /* !PDFENGINE_H */
