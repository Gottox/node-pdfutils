/*
 * PdfPage.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFPAGE_H
#define PDFPAGE_H

#include "pdfutils.h"
#include "PdfEngine.h"

class PdfPage : public node::ObjectWrap {
	virtual int page();
	virtual char *label();
	virtual int width();
	virtual int height();
};

#endif /* !PDFPAGE_H */
