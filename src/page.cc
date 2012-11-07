#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <sstream>
#include <algorithm>
#include "page.h"
#include "page_job.h"
#include "document.h"
#include "formats.h"

using namespace v8;
using namespace node;
using namespace std;


Persistent<Function> Page::constructor;

void Page::Init(Handle<Object> target) {
	unsigned int i;
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Local< ObjectTemplate > prt = tpl->PrototypeTemplate();
	
	for(i = 0; formatName(i) != NULL; i++) {
		Local<Function> fnc = FunctionTemplate::New(ConvertTo)->GetFunction();
		Local<String> sym = String::NewSymbol(formatName(i));
		fnc->SetName(sym);
		prt->Set(sym, fnc);
	}


	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Page"), constructor);
}

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

Handle<Object> Page::getObject() {
	if(!this->handle_.IsEmpty())
		return this->handle_;

	Handle<Value> argv[] = {
		//this->document->handle_
	};
	Local<Object> instance = (*constructor)->NewInstance(LENGTH(argv), argv);
	instance->Set(String::NewSymbol("width"), Local<Number>::New(Number::New(this->w)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("height"), Local<Number>::New(Number::New(this->h)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("index"), Local<Number>::New(Number::New(this->index)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("label"), Local<String>::New(String::New(this->label)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->Wrap(Persistent<Object>::New(instance));

	instance->Set(String::NewSymbol("links"), this->createLinks());

	// TODO make weak if there are no jobs running on this page;

	return instance;
}

Handle<Array> Page::createLinks() {
	GList *links = poppler_page_get_link_mapping(this->pg), *p;
	int length = g_list_length(links), i;
	Local<Array> result = Array::New(length);
	PopplerActionAny *action;

	Local<Object> obj;
	for (i = 0, p = links; p; p = p->next, i++) {
		std::stringstream istr;
		istr << i;
		PopplerLinkMapping *link = (PopplerLinkMapping *) p->data;
		char *title = ((PopplerActionAny *)link->action)->title;
		obj = Object::New();
		result->Set(String::NewSymbol(istr.str().c_str()), obj);
		obj->Set(String::NewSymbol("x"),
				Number::New(this->w - link->area.x1));
		obj->Set(String::NewSymbol("y"),
				Number::New(this->h - link->area.y1));
		obj->Set(String::NewSymbol("width"),
				Number::New(this->w - (link->area.x2 - link->area.x1)));
		obj->Set(String::NewSymbol("height"),
				Number::New(this->h - (link->area.y2 - link->area.x1)));
		obj->Set(String::NewSymbol("title"),
				title ? String::New(title) : Null());
		
		action = (PopplerActionAny *)link->action;
		Handle<String> type = String::NewSymbol("type"); 
		switch(action->type) {
		case POPPLER_ACTION_GOTO_DEST:
			obj->Set(type, String::New("goto"));
			break;
		case POPPLER_ACTION_GOTO_REMOTE:
			obj->Set(type, String::New("remote"));
			break;
		case POPPLER_ACTION_LAUNCH:
			obj->Set(type, String::New("launch"));
			break;
		case POPPLER_ACTION_URI:
			obj->Set(type, String::New("uri"));
			break;
		case POPPLER_ACTION_NAMED:
			obj->Set(type, String::New("named"));
			break;
		case POPPLER_ACTION_MOVIE:
			obj->Set(type, String::New("movie"));
			break;
		case POPPLER_ACTION_RENDITION:
			obj->Set(type, String::New("rendition"));
			break;
		case POPPLER_ACTION_OCG_STATE:
			obj->Set(type, String::New("ocgState"));
			break;
		case POPPLER_ACTION_JAVASCRIPT:
			obj->Set(type, String::New("javascript"));
			break;
		default:
			obj->Set(type, Null());
		}
	}


	poppler_page_free_link_mapping(links);
	return result;
}

Handle<Value> Page::ConvertTo(const Arguments& args) {
	HandleScope scope;
	Page* self = ObjectWrap::Unwrap<Page>(args.This());
	int i;

	String::AsciiValue callee(args.Callee()->GetName());
	char *name = *callee;
	int len = callee.length();
	Format format = FORMAT_UNKOWN;
	for(i = 0; formatName(i) != NULL && format == FORMAT_UNKOWN; i++) {
		if(strncmp(name, formatName(i), len) == 0)
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
