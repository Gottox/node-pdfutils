/*
 * PdfExportPageWorker.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFEXPORTPAGEWORKER_H
#define PDFEXPORTPAGEWORKER_H

#include "PdfWorker.h"

class PdfPageController;
class PdfExportPageWorker : public PdfWorker<PdfPageController> {
	friend class PdfPageController;
public:
	PdfExportPageWorker(PdfPageController *controller, NanCallback *callback)
		: PdfWorker(controller, callback) {}

	void Execute();

	void HandleIntermediate(void *data);

	void HandleOKCallback ();
};
#endif /* !PDFEXPORTPAGEWORKER_H */
