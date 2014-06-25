/*
 * PdfDocument.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocument.h"

JS_SHIM(PdfDocument)

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
