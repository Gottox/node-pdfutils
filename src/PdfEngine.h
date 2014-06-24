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

class PdfEngine : public node::ObjectWrap {
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

	v8::Local<v8::Object> wrapJs();

	virtual bool isThreadSafe();
	virtual int loadFromData(char *data, size_t length) = 0;
	virtual int loadFromFd(int fd) = 0;

	virtual int pageCount() = 0;
	virtual PdfPage *page(int number) = 0;

	virtual char *author() = 0;
	virtual int creationDate() = 0;
	virtual char *creator() = 0;
	virtual char *format() = 0;
	virtual std::vector<char *> keywords() = 0;
	virtual bool linearized() = 0;
	virtual char *metadata() = 0;
	virtual int modDate() = 0;
	virtual enum PageLayout pageLayout() = 0;
	virtual enum PageMode pageMode() = 0;
	virtual enum Permission permissions() = 0;
	virtual char *producer() = 0;
	virtual char *subject() = 0;
	virtual char *title() = 0;
};

#endif /* !PDFENGINE_H */
