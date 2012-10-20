#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include "page.h"
#include "page_job.h"
#include "document.h"
#include "util.h"
#include "formats.h"

using namespace v8;
using namespace node;


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

Local<Object> Page::createObject() {
	const unsigned argc = 1;
	Handle<Value> argv[argc] = {
		this->document->handle_
	};
	Local<Object> instance = (*constructor)->NewInstance(argc, argv);
	instance->Set(String::New("width"), Local<Number>::New(Number::New(this->w)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("height"), Local<Number>::New(Number::New(this->h)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("label"), Local<String>::New(String::New(this->label)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("document"), Local<Object>::New(this->document->handle_), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->Wrap(instance);

	return instance;

}

Handle<Value> Page::ConvertTo(const Arguments& args) {
	HandleScope scope;
	int i;

	Page* self = ObjectWrap::Unwrap<Page>(args.This());

	char *name = str2chr(args.Callee()->GetName());
	Format format = FORMAT_UNKOWN;
	for(i = 0; formatFunctions[i] != NULL && format == FORMAT_UNKOWN; i++) {
		if(strcmp(name, formatFunctions[i]) == 0)
			format = (Format)i;
	}
	if(format == FORMAT_UNKOWN)
		return ThrowException(Exception::Error(String::New("unkown format to convert to")));

	PageJob *pj = new PageJob(*self, format);
	uv_mutex_lock(&self->document->jobMutex);
	self->document->jobs.push(pj);
	uv_mutex_unlock(&self->document->jobMutex);
	uv_async_send(&self->document->bgMessage);

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
