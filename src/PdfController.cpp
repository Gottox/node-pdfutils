/*
 * PdfController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfController.h"
#include "v8utils.h"

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

v8::Persistent<v8::Function> PdfController::constructor;

void PdfController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(NanNew<v8::String>("PdfDocument"));
	v8::Local<v8::ObjectTemplate> instanceTpl = tpl->InstanceTemplate();
	instanceTpl->SetInternalFieldCount(1);

	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(NanNew<v8::String>("PdfDocument"), constructor);
}

NAN_METHOD(PdfController::New) {
	NanScope();
	new PdfController(args);
	NanReturnValue(args.This());
}

PdfController::PdfController(const v8::Arguments& args) {
	this->Wrap(args.This());
	setDocument(new PdfDocument());
	v8Super(args);
}

void PdfController::toJs(v8::Handle<v8::Object> &obj) {
	int i;
	PdfDocument *doc = this->document();
	v8::Local<v8::Object> permissionsObj = v8::Object::New();

	obj->Set(NanNew<v8::String>("length"), NanNew<v8::Integer>(doc->length()));
	obj->Set(NanNew<v8::String>("author"), charToV8(doc->author()));
	obj->Set(NanNew<v8::String>("creation_date"), NanNew<v8::Integer>(doc->creationDate()));
	obj->Set(NanNew<v8::String>("creator"), charToV8(doc->creator()));
	obj->Set(NanNew<v8::String>("format"), charToV8(doc->format()));
	obj->Set(NanNew<v8::String>("keywords"), charToV8(doc->keywords()));
	obj->Set(NanNew<v8::String>("linearized"), NanNew<v8::Boolean>(doc->linearized()));
	obj->Set(NanNew<v8::String>("metadata"), charToV8(doc->metadata()));
	obj->Set(NanNew<v8::String>("modification_date"), NanNew<v8::Integer>(doc->modDate()));
	obj->Set(NanNew<v8::String>("pageLayout"),
			charToV8(getPageLayout(doc->pageLayout() < PAGE_LAYOUT_LAST ? doc->pageLayout() : (PdfPageLayout)0)));
	obj->Set(NanNew<v8::String>("pageMode"),
			charToV8(getPageMode(doc->pageMode() < PAGE_MODE_LAST ? doc->pageMode() : (PdfPageMode)0)));

	for(i = 0; i < PERMISSIONS_LAST; i++) {
		permissionsObj->Set(NanNew<v8::String>(getPermission((PdfPermission)i)), NanNew<v8::Boolean>(doc->permissions() & i));
	}
	obj->Set(NanNew<v8::String>("permissions"), permissionsObj);
	obj->Set(NanNew<v8::String>("producer"), charToV8(doc->producer()));
	obj->Set(NanNew<v8::String>("subject"), charToV8(doc->subject()));
	obj->Set(NanNew<v8::String>("title"), charToV8(doc->title()));
}
void PdfController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfDocument *doc = this->document();
	size_t count;
	doc->setModDate(v8ToInt(obj->Get(NanNew<v8::String>("length"))));
	doc->setAuthor(NanCString(obj->Get(NanNew<v8::String>("author")), &count));
	doc->setCreationDate(v8ToInt(obj->Get(NanNew<v8::String>("creation_date"))));
	doc->setCreator(NanCString(obj->Get(NanNew<v8::String>("creator")), &count));
	doc->setFormat(NanCString(obj->Get(NanNew<v8::String>("format")), &count));
	doc->setKeywords(NanCString(obj->Get(NanNew<v8::String>("keywords")), &count));
	doc->setLinearized(NanCString(obj->Get(NanNew<v8::String>("linearized")), &count));
	doc->setMetadata(NanCString(obj->Get(NanNew<v8::String>("metadata")), &count));
	doc->setModDate(v8ToInt(obj->Get(NanNew<v8::String>("modification_date"))));
	//doc->setPageLayout(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	//doc->setPageMode(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	//doc->setPermissions(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	doc->setProducer(NanCString(obj->Get(NanNew<v8::String>("producer")), &count));
	doc->setSubject(NanCString(obj->Get(NanNew<v8::String>("subject")), &count));
	doc->setTitle(NanCString(obj->Get(NanNew<v8::String>("title")), &count));
}
PdfEngine *PdfController::engine() {
	return _engine;
}
PdfDocument *PdfController::document() {
	return _document;
}

void PdfController::setEngine(PdfEngine *engine) {
	_engine = engine;
}
void PdfController::setDocument(PdfDocument *document) {
	_document = document;
}
