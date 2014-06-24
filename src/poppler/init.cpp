/*
 * init.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
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
};

PDF_ENGINE(poppler, "gpl", PopplerEngine::NewInstance)
