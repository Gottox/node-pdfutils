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

#define ACTION(t) t *action = (t *)this->action

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
	this->Wrap(Persistent<Object>::New(instance));
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

	const char *type;

	switch(action->type) {
	case POPPLER_ACTION_GOTO_DEST:
		type = "goto";
		this->fillGoto();
		break;
	case POPPLER_ACTION_GOTO_REMOTE:
		type = "remote";
		this->fillRemote();
		break;
	case POPPLER_ACTION_LAUNCH:
		type = "launch";
		this->fillLaunch();
		break;
	case POPPLER_ACTION_URI:
		type = "uri";
		this->fillUri();
		break;
	case POPPLER_ACTION_NAMED:
		type = "named";
		this->fillNamed();
		break;
	case POPPLER_ACTION_MOVIE:
		type = "movie";
		this->fillMovie();
		break;
	case POPPLER_ACTION_RENDITION:
		type = "rendition";
		this->fillRendition();
		break;
	case POPPLER_ACTION_OCG_STATE:
		type = "ocgState";
		this->fillOCGState();
		break;
	case POPPLER_ACTION_JAVASCRIPT:
		type = "javascript";
		this->fillJavascript();
		break;
	default:
		type = NULL;
	}

	instance->Set(String::NewSymbol("type"), type ? String::New(type) : Null());

}


void Link::fillGoto() {
	ACTION(PopplerActionGotoDest);
	

}

void Link::fillRemote() {
	ACTION(PopplerActionGotoRemote);

}

void Link::fillLaunch() {
	ACTION(PopplerActionLaunch);

}

void Link::fillUri() {
	ACTION(PopplerActionUri);

}

void Link::fillNamed() {
	ACTION(PopplerActionNamed);

}

void Link::fillMovie() {
	ACTION(PopplerActionMovie);

}

void Link::fillRendition() {
	ACTION(PopplerActionRendition);

}

void Link::fillOCGState() {
	ACTION(PopplerActionOCGState);

}

void Link::fillJavascript() {
	ACTION(PopplerActionJavascript);

}
void Link::fillDest(PopplerDestType dest) {
	
}
