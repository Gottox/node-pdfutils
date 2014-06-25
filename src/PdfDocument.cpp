/*
 * PdfDocument.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocument.h"

v8::Persistent<v8::Function> PdfDocument::constructor;

void PdfDocument::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("PdfDocument"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(v8::String::NewSymbol("PdfDocument"), constructor);
}

v8::Handle<v8::Value> PdfDocument::New(const v8::Arguments& args) {
	v8::HandleScope scope;
	int i = 0;

	PdfDocument* obj = new PdfDocument();
	obj->Wrap(args.This());
	v8::Local<v8::Function> super = v8::Local<v8::Function>::Cast(args.Callee()->Get(v8::String::NewSymbol("super_")));
	v8::Local<v8::Value> argv[args.Length()];
	for(i = 0; i < args.Length(); i++)
		argv[i] = args[i];
	super->Call(args.This(), args.Length(), argv);
	return scope.Close(args.This());
}

PdfEngine *PdfDocument::engine() {
	return _engine;
}
const char *PdfDocument::author() {
	return _author;
}
int PdfDocument::creationDate() {
	return _creationDate;
}
const char *PdfDocument::creator() {
	return _creator;
}
const char *PdfDocument::format() {
	return _format;
}
std::vector<const char *> PdfDocument::keywords() {
	// TODO
	return std::vector<const char*>();
}
bool PdfDocument::linearized() {
	return _linearized;
}
const char *PdfDocument::metadata() {
	return _metadata;
}
int PdfDocument::modDate() {
	return _modDate;
}
enum PageLayout PdfDocument::pageLayout() {
	return _pageLayout;
}
enum PageMode PdfDocument::pageMode() {
	return _pageMode;
}
enum Permission PdfDocument::permissions() {
	return _permissions;
}
const char *PdfDocument::producer() {
	return _producer;
}
const char *PdfDocument::subject() {
	return _subject;
}
const char *PdfDocument::title() {
	return _title;
}

void PdfDocument::setEngine(PdfEngine *engine) {
	_engine = engine;
}
void PdfDocument::setAuthor(const char *author) {
	_author = author;
}
void PdfDocument::setCreationDate(const int creationDate) {
	_creationDate = creationDate;
}
void PdfDocument::setCreator(const char *creator) {
	_creator = creator;
}
void PdfDocument::setFormat(const char *format) {
	_format = format;
}
void PdfDocument::setKeywords(const std::vector <const char *> keywords) {
	// TODO
}
void PdfDocument::setLinearized(const bool linearized) {
	_linearized = linearized;
}
void PdfDocument::setMetadata(const char *metadata) {
	_metadata = metadata;
}
void PdfDocument::setModDate(const int modDate) {
	_modDate = modDate;
}
void PdfDocument::setPageLayout(const enum PageLayout pageLayout) {
	_pageLayout = pageLayout;
}
void PdfDocument::setPageMode(const enum PageMode pageMode) {
	_pageMode = pageMode;
}
void PdfDocument::setPermissions(const enum Permission permissions) {
	_permissions = permissions;
}
void PdfDocument::setProducer(const char *producer) {
	_producer = producer;
}
void PdfDocument::setSubject(const char *subject) {
	_subject = subject;
}
void PdfDocument::setTitle(const char *title) {
	_title = title;
}
