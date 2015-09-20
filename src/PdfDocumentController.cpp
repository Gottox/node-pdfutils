/*
 * PdfDocumentController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocumentController.h"

#include "worker/PdfWorker.h"
#include "PdfEngine.h"
#include "PdfEngineFactory.h"
#include "PdfPageController.h"

class PdfOpenWorker : public PdfWorker<PdfDocumentController> {
	friend class PdfDocumentController;
public:
	PdfOpenWorker(PdfDocumentController *controller, Nan::Callback *callback)
		: PdfWorker(controller, callback) {}

	void HandleOKCallback () {
		Nan::HandleScope scope;
		controller()->toJs();
		if(callback)
			callback->Call(0, NULL);
	}
};

class PdfOpenFileWorker : public PdfOpenWorker {
private:
	char *_src;

public:
	PdfOpenFileWorker(PdfDocumentController *controller, char *src, Nan::Callback *callback)
		: PdfOpenWorker(controller, callback) {
			_src = src;
		}

	void Execute () {
		PdfEngine *engine = controller()->engine();
		char *error = engine->openFromPath(_src);
		if(error) {
			SetErrorMessage(error);
			return;
		}
		engine->fillDocument(controller()->document());
	}
};

class PdfOpenDataWorker : public PdfOpenWorker {
private:
	char *_data;
	size_t _length;

public:
	PdfOpenDataWorker(PdfDocumentController *controller, char *data, size_t length, Nan::Callback *callback)
		: PdfOpenWorker(controller, callback) {
			_data = data;
			_length = length;
		}

	void Execute () {
		PdfEngine *engine = controller()->engine();
		char *error = engine->openFromData(_data, _length);
		if(error) {
			SetErrorMessage(error);
			return;
		}
		engine->fillDocument(controller()->document());
	}
};

inline const char *getPageLayout(enum PdfPageLayout layout) {
	switch(layout) {
		case PAGE_LAYOUT_UNSET:
		case PAGE_LAYOUT_LAST:
			return NULL;
		case PAGE_LAYOUT_SINGLE_PAGE:
			return "singlePage";
		case PAGE_LAYOUT_ONE_COLUMN:
			return "oneColumn";
		case PAGE_LAYOUT_TWO_COLUMN_LEFT:
			return "columnLeft";
		case PAGE_LAYOUT_TWO_COLUMN_RIGHT:
			return "columnRight";
		case PAGE_LAYOUT_TWO_PAGE_LEFT:
			return "twoPageLeft";
		case PAGE_LAYOUT_TWO_PAGE_RIGHT:
			return "twoPageRight";
	}
	return NULL;
}

inline const char *getPageMode(enum PdfPageMode mode) {
	switch(mode) {
		case PAGE_MODE_UNSET:
		case PAGE_MODE_LAST:
			return NULL;
		case PAGE_MODE_NONE:
			return "none";
		case PAGE_MODE_USE_OUTLINES:
			return "outlines";
		case PAGE_MODE_USE_THUMBS:
			return "thumbs";
		case PAGE_MODE_FULL_SCREEN:
			return "fullscreen";
		case PAGE_MODE_USE_OC:
			return "oc";
		case PAGE_MODE_USE_ATTACHMENTS:
			return "attachments";
	}
	return NULL;
}

inline const char *getPermission(enum PdfPermission permission) {
	switch(permission) {
		case PERMISSIONS_PRINT:
		case PERMISSIONS_LAST:
			return "print";
		case PERMISSIONS_MODIFY:
			return "modify";
		case PERMISSIONS_COPY:
			return "copy";
		case PERMISSIONS_ADD_NOTES:
			return "addNotes";
		case PERMISSIONS_FILL_FORM:
			return "fillForm";
		case PERMISSIONS_EXTRACT_CONTENTS:
			return "extractContents";
		case PERMISSIONS_ASSEMBLE:
			return "assemble";
		case PERMISSIONS_PRINT_HIGH_RESOLUTION:
			return "printHighResolution";
	}
	return NULL;
}

PdfDocumentController::~PdfDocumentController() {
	delete this->_document;
}

void PdfDocumentController::Init(v8::Handle<v8::Object> exports) {
	// Public
	v8::Local<v8::FunctionTemplate> pub = Nan::New<v8::FunctionTemplate>(New);
	pub->SetClassName(Nan::New<v8::String>("PdfDocument").ToLocalChecked());
	pub->InstanceTemplate()->SetInternalFieldCount(1);

	// Private
	v8::Local<v8::Object> prv = Nan::New<v8::Object>();
	prv->Set(Nan::New<v8::String>("load").ToLocalChecked(),
				Nan::New<v8::FunctionTemplate>(PdfDocumentController::Load)->GetFunction());
	prv->Set(Nan::New<v8::String>("toStream").ToLocalChecked(),
				Nan::New<v8::FunctionTemplate>(PdfDocumentController::As)->GetFunction());
	exports->Set(Nan::New<v8::String>("PdfDocument").ToLocalChecked(), pub->GetFunction());
	exports->Set(Nan::New<v8::String>("_document").ToLocalChecked(), prv);
}

NAN_METHOD(PdfDocumentController::New) {
	Nan::HandleScope scope;
	PdfDocumentController *controller = new PdfDocumentController();
	controller->Wrap(info.This());
	controller->setDocument(new PdfDocument());
	controller->MakeWeak();
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(PdfDocumentController::Load) {
	const char *error;
	PdfOpenWorker *worker;
	Nan::HandleScope scope;

	// get JS-Objects
	v8::Local<v8::Object> jsSelf = info.This();
	v8::Local<v8::Value> jsSrc = info[0];
	v8::Local<v8::Object> jsOptions = info[1].As<v8::Object>();
	v8::Local<v8::Function> jsCallback = info[2].As<v8::Function>();
	v8::Local<v8::Object> jsEngine = jsOptions->Get(Nan::New<v8::String>("engine").ToLocalChecked()).As<v8::Object>();

	// get C++-Objects from JS
	PdfDocumentController *self = node::ObjectWrap::Unwrap<PdfDocumentController>(jsSelf);
	PdfEngineFactory *factory = node::ObjectWrap::Unwrap<PdfEngineFactory>(jsEngine);
	Nan::Callback *callback = jsCallback->IsFunction() ? new Nan::Callback(jsCallback) : NULL;

	// set options on the engine
	PdfEngine *engine = factory->newInstance();
	if(jsOptions->Has(Nan::New<v8::String>("password").ToLocalChecked()))
		engine->setPassword(*Nan::Utf8String(jsOptions->Get(Nan::New<v8::String>("password").ToLocalChecked())));
	self->setEngine(engine);

	// open from file
	if(jsSrc->IsString()) {
		char *src = *Nan::Utf8String(jsSrc);
		worker = new PdfOpenFileWorker(self, src, callback);
	}
	// open from data
	else {
		char *data = node::Buffer::Data(jsSrc.As<v8::Object>());
		int len = node::Buffer::Length(jsSrc.As<v8::Object>());
		worker = new PdfOpenDataWorker(self, data, len, callback);
	}

	// asynchronous call
	if(callback) {
		Nan::AsyncQueueWorker(worker);
		info.GetReturnValue().Set(Nan::False());
	}
	// synchronous call
	else {
		worker->Execute();
		error = worker->ErrorMessage();
		if(error != NULL) {
			Nan::ThrowError(error);
		}
		else {
			worker->HandleOKCallback();
		}
		delete worker;
		info.GetReturnValue().Set(Nan::True());
	}
}

NAN_METHOD(PdfDocumentController::As) {
	Nan::HandleScope scope;
	info.GetReturnValue().Set(info.This());
}

void PdfDocumentController::toJs() {
	int i;
	PdfDocument *doc = this->document();
	v8::Local<v8::Object> obj = this->handle();
	v8::Local<v8::Object> permissionsObj = Nan::New<v8::Object>();

	this->set(obj, (char *)"length", doc->length());
	this->set(obj, (char *)"author", doc->author());
	this->set(obj, (char *)"creation_date", doc->creationDate());
	this->set(obj, (char *)"creator", doc->creator());
	this->set(obj, (char *)"format", doc->format());
	this->set(obj, (char *)"keywords", doc->keywords());
	this->set(obj, (char *)"linearized", doc->linearized());
	this->set(obj, (char *)"metadata", doc->metadata());
	this->set(obj, (char *)"modification_date", doc->modDate());
	this->set(obj, (char *)"pageLayout",
			getPageLayout(doc->pageLayout() < PAGE_LAYOUT_LAST ? doc->pageLayout() : (PdfPageLayout)0));
	this->set(obj, (char *)"pageMode",
			getPageMode(doc->pageMode() < PAGE_MODE_LAST ? doc->pageMode() : (PdfPageMode)0));

	for(i = 0; i < PERMISSIONS_LAST; i++) {
		this->set(permissionsObj, getPermission((PdfPermission)i), bool(doc->permissions() & i));
	}
	obj->Set(Nan::New<v8::String>("permissions").ToLocalChecked(), permissionsObj);
	this->set(obj, (char *)"producer", doc->producer());
	this->set(obj, (char *)"subject", doc->subject());
	this->set(obj, (char *)"title", doc->title());
}
void PdfDocumentController::fromJs() {
	PdfDocument *doc = this->document();
	v8::Local<v8::Object> obj = this->handle();
	doc->setModDate(obj->Get(Nan::New<v8::String>("length").ToLocalChecked())->IntegerValue());
	doc->setAuthor(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("author").ToLocalChecked())));
	doc->setCreationDate(obj->Get(Nan::New<v8::String>("creation_date").ToLocalChecked())->IntegerValue());
	doc->setCreator(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("creator").ToLocalChecked())));
	doc->setFormat(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("format").ToLocalChecked())));
	doc->setKeywords(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("keywords").ToLocalChecked())));
	doc->setLinearized(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("linearized").ToLocalChecked())));
	doc->setMetadata(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("metadata").ToLocalChecked())));
	doc->setModDate(obj->Get(Nan::New<v8::String>("modification_date").ToLocalChecked())->IntegerValue());
	//doc->setPageLayout(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("").ToLocalChecked())));
	//doc->setPageMode(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("").ToLocalChecked())));
	//doc->setPermissions(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("").ToLocalChecked())));
	doc->setProducer(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("producer").ToLocalChecked())));
	doc->setSubject(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("subject").ToLocalChecked())));
	doc->setTitle(*Nan::Utf8String(obj->Get(Nan::New<v8::String>("title").ToLocalChecked())));
}

PdfDocument *PdfDocumentController::document() {
	return _document;
}
void PdfDocumentController::setDocument(PdfDocument *document) {
	_document = document;
}
