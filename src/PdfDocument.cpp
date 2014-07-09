/*
 * PdfDocument.cpp
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#include "PdfDocument.h"

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
enum PdfPageLayout PdfDocument::pageLayout() {
	return _pageLayout;
}
enum PdfPageMode PdfDocument::pageMode() {
	return _pageMode;
}
int PdfDocument::permissions() {
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
void PdfDocument::setPageLayout(const enum PdfPageLayout pageLayout) {
	_pageLayout = pageLayout;
}
void PdfDocument::setPageMode(const enum PdfPageMode pageMode) {
	_pageMode = pageMode;
}
void PdfDocument::setPermissions(const int permissions) {
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
