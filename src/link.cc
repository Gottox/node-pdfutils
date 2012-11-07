#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include "link.h"
#include "page.h"
#include "document.h"
#include "formats.h"

using namespace v8;
using namespace node;
using namespace std;


Persistent<Function> Link::constructor;

void Link::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFLink"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Link"), constructor);
}

Link::Link(Page *page, PopplerLinkMapping *link) {
	this->page = page;

	this->x = link->area.x1;
	this->y = page->h - link->area.y1;
	this->w = link->area.x2 - link->area.x1;
	this->h = link->area.y2 - link->area.y1;
	this->action = poppler_action_copy(link->action);
	this->title = ((PopplerActionAny *)this->action)->title;

	this->createObject();
}

Link::~Link() {
	poppler_action_free(this->action);
	if(!this->handle_.IsEmpty())
		this->handle_.Dispose();
};

void Link::createObject() {
	Handle<Value> argv[] = {
	};
	Local<Object> instance = (*constructor)->NewInstance(LENGTH(argv), argv);
	instance->Set(String::NewSymbol("x"), Number::New(this->x), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("y"), Number::New(this->y), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("width"), Number::New(this->w), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("height"), Number::New(this->h), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	instance->Set(String::NewSymbol("title"), this->title ? String::New(this->title) : Null(),
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

	Handle<String> type = String::NewSymbol("type"); 
	switch(action->type) {
	case POPPLER_ACTION_GOTO_DEST:
		instance->Set(type, String::New("goto"));
		break;
	case POPPLER_ACTION_GOTO_REMOTE:
		instance->Set(type, String::New("remote"));
		break;
	case POPPLER_ACTION_LAUNCH:
		instance->Set(type, String::New("launch"));
		break;
	case POPPLER_ACTION_URI:
		instance->Set(type, String::New("uri"));
		break;
	case POPPLER_ACTION_NAMED:
		instance->Set(type, String::New("named"));
		break;
	case POPPLER_ACTION_MOVIE:
		instance->Set(type, String::New("movie"));
		break;
	case POPPLER_ACTION_RENDITION:
		instance->Set(type, String::New("rendition"));
		break;
	case POPPLER_ACTION_OCG_STATE:
		instance->Set(type, String::New("ocgState"));
		break;
	case POPPLER_ACTION_JAVASCRIPT:
		instance->Set(type, String::New("javascript"));
		break;
	default:
		instance->Set(type, Null());
	}
	
	this->Wrap(Persistent<Object>::New(instance));
}
