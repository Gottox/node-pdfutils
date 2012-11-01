#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <algorithm>
#include "page.h"
#include "page_job.h"
#include "document.h"
#include "formats.h"

using namespace v8;
using namespace node;
using namespace std;


Persistent<Function> Page::constructor;

Page::Page(Document &document, int index) {
	this->document = &document;
	this->index = index;
	this->pg = poppler_document_get_page(this->document->doc, this->index);
	poppler_page_get_size (this->pg, &this->w, &this->h);
	this->label = poppler_page_get_label(this->pg);
}

Page::~Page() {
	g_free(this->label);
	if(!this->handle_.IsEmpty())
		this->handle_.Dispose();
};


void Page::Init(Handle<Object> target) {
	unsigned int i;
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Local< ObjectTemplate > prt = tpl->PrototypeTemplate();
	
	for(i = 0; formatFunctions[i] != NULL; i++) {
		Local<Function> fnc = FunctionTemplate::New(ConvertTo)->GetFunction();
		Local<String> sym = String::NewSymbol(formatFunctions[i]);
		fnc->SetName(sym);
		prt->Set(sym, fnc);
	}


	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Page"), constructor);
}

Handle<Object> Page::createObject() {
	Handle<Value> argv[] = {
		//this->document->handle_
	};
	Local<Object> instance = (*constructor)->NewInstance(LENGTH(argv), argv);
	instance->Set(String::New("width"), Local<Number>::New(Number::New(this->w)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("height"), Local<Number>::New(Number::New(this->h)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("index"), Local<Number>::New(Number::New(this->index)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("label"), Local<String>::New(String::New(this->label)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->Wrap(Persistent<Object>::New(instance));

	// TODO make weak if there are no jobs running on this page;

	return instance;

}

Handle<Value> Page::ConvertTo(const Arguments& args) {
	HandleScope scope;
	Page* self = ObjectWrap::Unwrap<Page>(args.This());
	int i;

	String::AsciiValue callee(args.Callee()->GetName());
	char *name = *callee;
	int len = callee.length();
	Format format = FORMAT_UNKOWN;
	for(i = 0; formatFunctions[i] != NULL && format == FORMAT_UNKOWN; i++) {
		if(strncmp(name, formatFunctions[i], len) == 0)
			format = (Format)i;
	}
	if(format == FORMAT_UNKOWN)
		return ThrowException(Exception::Error(String::New("unkown format to convert to")));

	PageJob *pj = new PageJob(*self, format);
	if(args.Length() >= 1) {
		if(!args[0]->IsObject())
			return ThrowException(Exception::Error(String::New("first argument must be an Object")));
		pj->calcDimensions(args[0]->ToObject());
	}
	self->document->addJob(pj);

	return pj->handle_;
}

Handle<Value> Page::GetWidth(Local< String > property, const AccessorInfo &info) {
	HandleScope scope;

	Page* page = ObjectWrap::Unwrap<Page>(info.This());

	return scope.Close(Number::New(page->w));
}
Handle<Value> Page::GetHeight(Local< String > property, const AccessorInfo &info) {
	HandleScope scope;

	Page* page = ObjectWrap::Unwrap<Page>(info.This());

	return scope.Close(Number::New(page->h));
}
