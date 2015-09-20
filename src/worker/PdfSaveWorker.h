/*
 * PdfSaveWorker.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFSAVEWORKER_H
#define PDFSAVEWORKER_H

#include "PdfWorker.h"

class PdfPageController;
class PdfSaveWorker : public PdfWorker<PdfPageController> {
	friend class PdfPageController;
public:
	PdfSaveWorker(PdfPageController *controller, Nan::Callback *callback)
		: PdfWorker(controller, callback) {}

	void Execute();

	void HandleIntermediate(void *data);

	void HandleOKCallback ();
};
#endif /* !PDFEXPORTPAGEWORKER_H */
