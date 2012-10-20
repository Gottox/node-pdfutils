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
}

Page::~Page() {
	
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
	this->Wrap(instance);
	instance->Set(String::New("width"), Local<Number>::New(Number::New(this->w)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::New("height"), Local<Number>::New(Number::New(this->h)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

	return instance;

}

Handle<Value> Page::ConvertTo(const Arguments& args) {
	HandleScope scope;

	Page* self = ObjectWrap::Unwrap<Page>(args.This());

	if(args.Length() < 1 || !args[2]->IsFunction())
		return ThrowException(Exception::Error(String::New("first parameter must be a Function")));

	uv_work_t *req = new uv_work_t;
	req->data = self;


	return Undefined();
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
