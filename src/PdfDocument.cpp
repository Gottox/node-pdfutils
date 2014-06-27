/*
 * PdfDocument.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocument.h"
#include "v8utils.h"

JS_SHIM(PdfDocument)

PdfDocument::PdfDocument(const v8::Arguments& args) {

}
void PdfDocument::toJs(v8::Handle<v8::Object> &obj) {
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
void PdfDocument::fromJs(v8::Handle<v8::Object> &obj) {
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
PdfEngine *PdfDocument::engine() {
	return _engine;
}
int PdfDocument::length() {
	return _length;
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
const char * PdfDocument::keywords() {
	return _keywords;
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
void PdfDocument::setLength(const int length) {
	_length = length;
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
void PdfDocument::setKeywords(const char *keywords) {
	_keywords = keywords;
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
