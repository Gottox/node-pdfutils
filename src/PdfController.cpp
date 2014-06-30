/*
 * PdfController.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfController.h"
#include "v8utils.h"

v8::Persistent<v8::Function> PdfController::constructor;

void PdfController::Init(v8::Handle<v8::Object> exports) {
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("PdfController"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	exports->Set(v8::String::NewSymbol("PdfController"), constructor);
}

v8::Handle<v8::Value> PdfController::New(const v8::Arguments& args) {
	v8::HandleScope scope;

	PdfController* obj = new PdfController(args);
	return scope.Close(args.This());
}

PdfController::PdfController(const v8::Arguments& args) {
	this->Wrap(args.This());
	v8Super(args);
}
void PdfController::toJs(v8::Handle<v8::Object> &obj) {
	obj->Set(v8::String::NewSymbol("length"), v8::Integer::New(this->length()));
	obj->Set(v8::String::NewSymbol("author"), charToV8(this->author()));
	obj->Set(v8::String::NewSymbol("creation_date"), v8::Integer::New(this->creationDate()));
	obj->Set(v8::String::NewSymbol("creator"), charToV8(this->creator()));
	//obj->Set(v8::String::NewSymbol("format"), charToV8(this->format()));
	obj->Set(v8::String::NewSymbol("keywords"), charToV8(this->keywords()));
	obj->Set(v8::String::NewSymbol("linearized"), v8::Boolean::New(this->linearized()));
	obj->Set(v8::String::NewSymbol("metadata"), charToV8(this->metadata()));
	obj->Set(v8::String::NewSymbol("modification_date"), v8::Integer::New(this->modDate()));
	//obj->Set(v8::String::NewSymbol("pageLayout"), charToV8(this->pageLayout()));
	//obj->Set(v8::String::NewSymbol("pageMode"), charToV8(this->pageMode()));
	//obj->Set(v8::String::NewSymbol("permissions"), charToV8(this->permissions()));
	obj->Set(v8::String::NewSymbol("producer"), charToV8(this->producer()));
	obj->Set(v8::String::NewSymbol("subject"), charToV8(this->subject()));
	obj->Set(v8::String::NewSymbol("title"), charToV8(this->title()));
}
void PdfController::fromJs(v8::Handle<v8::Object> &obj) {
	this->setModDate(v8ToInt(obj->Get(v8::String::NewSymbol("length"))));
	this->setAuthor(v8ToChar(obj->Get(v8::String::NewSymbol("author"))));
	this->setCreationDate(v8ToInt(obj->Get(v8::String::NewSymbol("creation_date"))));
	this->setCreator(v8ToChar(obj->Get(v8::String::NewSymbol("creator"))));
	//this->setFormat(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	this->setKeywords(v8ToChar(obj->Get(v8::String::NewSymbol("keywords"))));
	this->setLinearized(v8ToChar(obj->Get(v8::String::NewSymbol("linearized"))));
	this->setMetadata(v8ToChar(obj->Get(v8::String::NewSymbol("metadata"))));
	this->setModDate(v8ToInt(obj->Get(v8::String::NewSymbol("modification_date"))));
	//this->setPageLayout(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	//this->setPageMode(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	//this->setPermissions(v8ToChar(obj->Get(v8::String::NewSymbol(""))));
	this->setProducer(v8ToChar(obj->Get(v8::String::NewSymbol("producer"))));
	this->setSubject(v8ToChar(obj->Get(v8::String::NewSymbol("subject"))));
	this->setTitle(v8ToChar(obj->Get(v8::String::NewSymbol("title"))));
}
PdfEngine *PdfController::engine() {
	return _engine;
}
int PdfController::length() {
	return _length;
}
const char *PdfController::author() {
	return _author;
}
int PdfController::creationDate() {
	return _creationDate;
}
const char *PdfController::creator() {
	return _creator;
}
const char *PdfController::format() {
	return _format;
}
const char * PdfController::keywords() {
	return _keywords;
}
bool PdfController::linearized() {
	return _linearized;
}
const char *PdfController::metadata() {
	return _metadata;
}
int PdfController::modDate() {
	return _modDate;
}
enum PageLayout PdfController::pageLayout() {
	return _pageLayout;
}
enum PageMode PdfController::pageMode() {
	return _pageMode;
}
enum Permission PdfController::permissions() {
	return _permissions;
}
const char *PdfController::producer() {
	return _producer;
}
const char *PdfController::subject() {
	return _subject;
}
const char *PdfController::title() {
	return _title;
}

void PdfController::setEngine(PdfEngine *engine) {
	_engine = engine;
}
void PdfController::setLength(const int length) {
	_length = length;
}
void PdfController::setAuthor(const char *author) {
	_author = author;
}
void PdfController::setCreationDate(const int creationDate) {
	_creationDate = creationDate;
}
void PdfController::setCreator(const char *creator) {
	_creator = creator;
}
void PdfController::setFormat(const char *format) {
	_format = format;
}
void PdfController::setKeywords(const char *keywords) {
	_keywords = keywords;
}
void PdfController::setLinearized(const bool linearized) {
	_linearized = linearized;
}
void PdfController::setMetadata(const char *metadata) {
	_metadata = metadata;
}
void PdfController::setModDate(const int modDate) {
	_modDate = modDate;
}
void PdfController::setPageLayout(const enum PageLayout pageLayout) {
	_pageLayout = pageLayout;
}
void PdfController::setPageMode(const enum PageMode pageMode) {
	_pageMode = pageMode;
}
void PdfController::setPermissions(const enum Permission permissions) {
	_permissions = permissions;
}
void PdfController::setProducer(const char *producer) {
	_producer = producer;
}
void PdfController::setSubject(const char *subject) {
	_subject = subject;
}
void PdfController::setTitle(const char *title) {
	_title = title;
}
