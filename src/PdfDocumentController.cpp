/*
 * PdfDocumentController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocumentController.h"

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

void PdfDocumentController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
	tpl->SetClassName(NanNew<v8::String>("PdfDocument"));
	v8::Local<v8::ObjectTemplate> instanceTpl = tpl->InstanceTemplate();
	instanceTpl->SetInternalFieldCount(1);

	exports->Set(NanNew<v8::String>("PdfDocument"), tpl->GetFunction());
}

NAN_METHOD(PdfDocumentController::New) {
	NanScope();
	PdfDocumentController *controller = new PdfDocumentController();
	controller->Wrap(args.This());
	controller->setDocument(new PdfDocument());
	NanReturnValue(args.This());
}

void PdfDocumentController::toJs(v8::Handle<v8::Object> &obj) {
	int i;
	PdfDocument *doc = this->document();
	v8::Local<v8::Object> permissionsObj = NanNew<v8::Object>();

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
	obj->Set(NanNew<v8::String>("permissions"), permissionsObj);
	this->set(obj, (char *)"producer", doc->producer());
	this->set(obj, (char *)"subject", doc->subject());
	this->set(obj, (char *)"title", doc->title());
}
void PdfDocumentController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfDocument *doc = this->document();
	size_t count;
	doc->setModDate(obj->Get(NanNew<v8::String>("length"))->IntegerValue());
	doc->setAuthor(NanCString(obj->Get(NanNew<v8::String>("author")), &count));
	doc->setCreationDate(obj->Get(NanNew<v8::String>("creation_date"))->IntegerValue());
	doc->setCreator(NanCString(obj->Get(NanNew<v8::String>("creator")), &count));
	doc->setFormat(NanCString(obj->Get(NanNew<v8::String>("format")), &count));
	doc->setKeywords(NanCString(obj->Get(NanNew<v8::String>("keywords")), &count));
	doc->setLinearized(NanCString(obj->Get(NanNew<v8::String>("linearized")), &count));
	doc->setMetadata(NanCString(obj->Get(NanNew<v8::String>("metadata")), &count));
	doc->setModDate(obj->Get(NanNew<v8::String>("modification_date"))->IntegerValue());
	//doc->setPageLayout(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	//doc->setPageMode(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	//doc->setPermissions(NanCString(obj->Get(NanNew<v8::String>("")), &count));
	doc->setProducer(NanCString(obj->Get(NanNew<v8::String>("producer")), &count));
	doc->setSubject(NanCString(obj->Get(NanNew<v8::String>("subject")), &count));
	doc->setTitle(NanCString(obj->Get(NanNew<v8::String>("title")), &count));
}

PdfDocument *PdfDocumentController::document() {
	return _document;
}
void PdfDocumentController::setDocument(PdfDocument *document) {
	_document = document;
}
