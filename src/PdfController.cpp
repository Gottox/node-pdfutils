/*
 * PdfController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfController.h"
#include "v8utils.h"

const char *pageLayouts[] = {
	[PAGE_LAYOUT_UNSET] = NULL,
	[PAGE_LAYOUT_SINGLE_PAGE] = "singlePage",
	[PAGE_LAYOUT_ONE_COLUMN] = "oneColumn",
	[PAGE_LAYOUT_TWO_COLUMN_LEFT] = "columnLeft",
	[PAGE_LAYOUT_TWO_COLUMN_RIGHT] = "columnRight",
	[PAGE_LAYOUT_TWO_PAGE_LEFT] = "twoPageLeft",
	[PAGE_LAYOUT_TWO_PAGE_RIGHT] = "twoPageRight"
};

const char *pageModes[] = {
	[PAGE_MODE_UNSET] = NULL,
	[PAGE_MODE_NONE] = "none",
	[PAGE_MODE_USE_OUTLINES] = "outlines",
	[PAGE_MODE_USE_THUMBS] = "thumbs",
	[PAGE_MODE_FULL_SCREEN] = "fullscreen",
	[PAGE_MODE_USE_OC] = "oc",
	[PAGE_MODE_USE_ATTACHMENTS] = "attachments"
};
const char *permissions[] = {
	[PERMISSIONS_PRINT] = "print",
	[PERMISSIONS_MODIFY] = "modify",
	[PERMISSIONS_COPY] = "copy",
	[PERMISSIONS_ADD_NOTES] = "addNotes",
	[PERMISSIONS_FILL_FORM] = "fillForm",
	[PERMISSIONS_EXTRACT_CONTENTS] = "extractContents",
	[PERMISSIONS_ASSEMBLE] = "assemble",
	[PERMISSIONS_PRINT_HIGH_RESOLUTION] = "printHighResolution"
};

v8::Persistent<v8::Function> PdfController::constructor;

void PdfController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("PdfDocument"));
	v8::Local<v8::ObjectTemplate> instanceTpl = tpl->InstanceTemplate();
	instanceTpl->SetInternalFieldCount(1);

	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(v8::String::NewSymbol("PdfDocument"), constructor);
}

v8::Handle<v8::Value> PdfController::New(const v8::Arguments& args) {
	v8::HandleScope scope;
	new PdfController(args);
	return scope.Close(args.This());
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

	obj->Set(v8::String::NewSymbol("length"), v8::Integer::New(doc->length()));
	obj->Set(v8::String::NewSymbol("author"), charToV8(doc->author()));
	obj->Set(v8::String::NewSymbol("creation_date"), v8::Integer::New(doc->creationDate()));
	obj->Set(v8::String::NewSymbol("creator"), charToV8(doc->creator()));
	//obj->Set(v8::String::NewSymbol("format"), charToV8(doc->format()));
	obj->Set(v8::String::NewSymbol("keywords"), charToV8(doc->keywords()));
	obj->Set(v8::String::NewSymbol("linearized"), v8::Boolean::New(doc->linearized()));
	obj->Set(v8::String::NewSymbol("metadata"), charToV8(doc->metadata()));
	obj->Set(v8::String::NewSymbol("modification_date"), v8::Integer::New(doc->modDate()));
	obj->Set(v8::String::NewSymbol("pageLayout"),
			charToV8(pageLayouts[doc->pageLayout() < PAGE_LAYOUT_LAST ? doc->pageLayout() : 0]));
	obj->Set(v8::String::NewSymbol("pageMode"),
			charToV8(pageModes[doc->pageMode() < PAGE_MODE_LAST ? doc->pageMode() : 0]));

	for(i = 0; i < PERMISSIONS_LAST; i++) {
		permissionsObj->Set(v8::String::NewSymbol(permissions[i]), v8::Boolean::New(doc->permissions() & i));
	}
	obj->Set(v8::String::NewSymbol("permissions"), permissionsObj);
	obj->Set(v8::String::NewSymbol("producer"), charToV8(doc->producer()));
	obj->Set(v8::String::NewSymbol("subject"), charToV8(doc->subject()));
	obj->Set(v8::String::NewSymbol("title"), charToV8(doc->title()));
}
void PdfController::fromJs(v8::Handle<v8::Object> &obj) {
	PdfDocument *doc = this->document();
	doc->setModDate(v8ToInt(obj->Get(v8::String::NewSymbol("length"))));
	doc->setAuthor(v8ToChar(obj->Get(v8::String::NewSymbol("author"))));
	doc->setCreationDate(v8ToInt(obj->Get(v8::String::NewSymbol("creation_date"))));
	doc->setCreator(v8ToChar(obj->Get(v8::String::NewSymbol("creator"))));
	//doc->setFormat(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	doc->setKeywords(v8ToChar(obj->Get(v8::String::NewSymbol("keywords"))));
	doc->setLinearized(v8ToChar(obj->Get(v8::String::NewSymbol("linearized"))));
	doc->setMetadata(v8ToChar(obj->Get(v8::String::NewSymbol("metadata"))));
	doc->setModDate(v8ToInt(obj->Get(v8::String::NewSymbol("modification_date"))));
	//doc->setPageLayout(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	//doc->setPageMode(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	//doc->setPermissions(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	doc->setProducer(v8ToChar(obj->Get(v8::String::NewSymbol("producer"))));
	doc->setSubject(v8ToChar(obj->Get(v8::String::NewSymbol("subject"))));
	doc->setTitle(v8ToChar(obj->Get(v8::String::NewSymbol("title"))));
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
