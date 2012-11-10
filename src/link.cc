#include <node.h>
#include <poppler.h>
#include "link.h"
#include "page.h"
#include "document.h"

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
	const char *type;
	Handle<Value> argv[] = {
	};
	Local<Object> instance = (*constructor)->NewInstance(LENGTH(argv), argv);
	this->Wrap(Persistent<Object>::New(instance));
	instance->Set(String::NewSymbol("x"), Number::New(this->x), 
			static_cast<PropertyAttribute>(ReadOnly)); 
	instance->Set(String::NewSymbol("y"), Number::New(this->y), 
			static_cast<PropertyAttribute>(ReadOnly)); 
	instance->Set(String::NewSymbol("width"), Number::New(this->w), 
			static_cast<PropertyAttribute>(ReadOnly)); 
	instance->Set(String::NewSymbol("height"), Number::New(this->h), 
			static_cast<PropertyAttribute>(ReadOnly)); 
	instance->Set(String::NewSymbol("title"), this->title ? String::New(this->title) : Null(),
			static_cast<PropertyAttribute>(ReadOnly)); 

	Local<Object> dest = Object::New();


	switch(action->type) {
	case POPPLER_ACTION_GOTO_DEST:
		type = "goto";
		this->fillGoto(dest);
		break;
	case POPPLER_ACTION_GOTO_REMOTE:
		type = "remote";
		this->fillRemote(dest);
		break;
	case POPPLER_ACTION_LAUNCH:
		type = "launch";
		this->fillLaunch(dest);
		break;
	case POPPLER_ACTION_URI:
		type = "uri";
		this->fillUri(dest);
		break;
	case POPPLER_ACTION_NAMED:
		type = "named";
		this->fillNamed(dest);
		break;
	case POPPLER_ACTION_MOVIE:
		type = "movie";
		this->fillMovie(dest);
		break;
	case POPPLER_ACTION_RENDITION:
		type = "rendition";
		this->fillRendition(dest);
		break;
	case POPPLER_ACTION_OCG_STATE:
		type = "ocgState";
		this->fillOCGState(dest);
		break;
	case POPPLER_ACTION_JAVASCRIPT:
		type = "javascript";
		this->fillJavascript(dest);
		break;
	default:
		type = NULL;
	}

	instance->Set(String::NewSymbol("type"), type ? String::New(type) : Null());
	if(type)
		instance->Set(String::NewSymbol("dest"), dest);
	else
		instance->Set(String::NewSymbol("dest"), Null());

}


void Link::fillGoto(Local<Object> &dest) {
	ACTION(PopplerActionGotoDest);
	
	fillDest(dest, action->dest);
}

void Link::fillRemote(Local<Object> &dest) {
	ACTION(PopplerActionGotoRemote);

	dest->Set(String::NewSymbol("filename"), String::New(action->file_name));
	fillDest(dest, action->dest);
}

void Link::fillLaunch(Local<Object> &dest) {
	ACTION(PopplerActionLaunch);


	dest->Set(String::NewSymbol("filename"), String::New(action->file_name));
	dest->Set(String::NewSymbol("args"), String::New(action->params));
}

void Link::fillUri(Local<Object> &dest) {
	ACTION(PopplerActionUri);

	dest->Set(String::NewSymbol("uri"), String::New(action->uri));
}

void Link::fillNamed(Local<Object> &dest) {
	//ACTION(PopplerActionNamed);

	//TODO
}

void Link::fillMovie(Local<Object> &dest) {
	//ACTION(PopplerActionMovie);

	//TODO
}

void Link::fillRendition(Local<Object> &dest) {
	//ACTION(PopplerActionRendition);

	//TODO
}

void Link::fillOCGState(Local<Object> &dest) {
	//ACTION(PopplerActionOCGState);

	//TODO
}

void Link::fillJavascript(Local<Object> &dest) {
	ACTION(PopplerActionJavascript);

	dest->Set(String::NewSymbol("src"), String::New(action->script));
}
void Link::fillDest(Local<Object> &dest, PopplerDest *d) {
	if(d->named_dest) {
		dest->Set(String::NewSymbol("name"), d->named_dest ? String::New(d->named_dest) : Null());
		d = poppler_document_find_dest(this->page->document->doc, d->named_dest);
	}

	dest->Set(String::NewSymbol("left"), Number::New(d->left));
	dest->Set(String::NewSymbol("top"), Number::New(d->top));
	dest->Set(String::NewSymbol("bottom"), Number::New(d->bottom));
	dest->Set(String::NewSymbol("right"), Number::New(d->right));
	dest->Set(String::NewSymbol("page"), Number::New(d->page_num));
}
