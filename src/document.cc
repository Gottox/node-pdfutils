#define BUILDING_NODE_EXTENSION
#include <sstream>
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <unistd.h>
#include "document.h"
#include "page.h"
#include "util.h"

using namespace v8;
using namespace node;

const static char *properties[] = {
	"author",
	"creation-date",
	"creator",
	"format",
	"keywords",
	"linearized",
	"metadata",
	"mod-date",
	"page-layout",
	"page-mode",
	"permissions",
	"producer",
	"subject",
	"title",
};

const static char *pageLayouts[] = {
	[POPPLER_PAGE_LAYOUT_UNSET] = NULL,
	[POPPLER_PAGE_LAYOUT_SINGLE_PAGE] = "singlePage",
	[POPPLER_PAGE_LAYOUT_ONE_COLUMN] = "oneColumn",
	[POPPLER_PAGE_LAYOUT_TWO_COLUMN_LEFT] = "twoColumnLeft",
	[POPPLER_PAGE_LAYOUT_TWO_COLUMN_RIGHT] = "twoColumnRight",
	[POPPLER_PAGE_LAYOUT_TWO_PAGE_LEFT] = "twoPageLeft",
	[POPPLER_PAGE_LAYOUT_TWO_PAGE_RIGHT] ="twoPageRight"
};
const static char *pageModes[] = {
	[POPPLER_PAGE_MODE_UNSET] = NULL,
	[POPPLER_PAGE_MODE_NONE] = "none",
	[POPPLER_PAGE_MODE_USE_OUTLINES] = "useOutlines",
	[POPPLER_PAGE_MODE_USE_THUMBS] = "useThumbs",
	[POPPLER_PAGE_MODE_FULL_SCREEN] = "fullscreen",
	[POPPLER_PAGE_MODE_USE_OC] = "useOc",
	[POPPLER_PAGE_MODE_USE_ATTACHMENTS] =  "useAttachments"
};

Document::Document(char *buffer, const int buflen, Persistent<Function>& loadCb) {
	this->buffer = buffer;
	this->buflen = buflen;
	this->loadCb = loadCb;
}

Document::~Document() {
	uv_close((uv_handle_t*) &this->v8Message, NULL);
	uv_mutex_lock(&this->jobMutex);
	while(this->finishedJobs.size())
		this->finishedJobs.pop();
	while(this->jobs.size())
		this->jobs.pop();
	uv_mutex_unlock(&this->jobMutex);

	uv_async_send(&this->bgMessage);
	uv_thread_join(&this->worker);
};

void Document::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("PDF"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Document"), constructor);
}

void  Document::WorkerInit(void *arg) {
	Document *self = (Document *)(arg);
	int i = 0;
	self->doc = poppler_document_new_from_data(self->buffer, self->buflen, NULL, NULL);
	int pages = poppler_document_get_n_pages(self->doc);

	self->pages = new std::vector<Page*>();
	for(i = 0; i < pages; i++) {
		self->pages->push_back(new Page(*self, i));
	}
	self->author = poppler_document_get_author(self->doc);
	self->subject = poppler_document_get_subject(self->doc);
	self->title = poppler_document_get_title(self->doc);
	uv_async_send(&self->v8Message);

	uv_run(self->bgLoop);
}

Handle<Value> Document::New(const Arguments& args) {
	HandleScope scope;

	Document* self;
	Persistent<Function> cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
	
	if(args.Length() < 2 || !args[1]->IsFunction())
		return ThrowException(Exception::Error(String::New("second argument must be a Function")));
	else if(Buffer::HasInstance(args[0]))
		self = new Document(Buffer::Data(args[0]->ToObject()), Buffer::Length(args[0]->ToObject()), cb);
	else
		return ThrowException(Exception::Error(String::New("first argument must be a Buffer")));

	self->bgLoop = uv_loop_new();
	self->v8Message.data = self->bgMessage.data = self;
	uv_thread_create(&self->worker, WorkerInit, self);
	uv_mutex_init(&self->jobMutex);
	uv_async_init(uv_default_loop(), &self->v8Message, Receiver);
	uv_async_init(self->bgLoop, &self->bgMessage, Worker);

	self->Wrap(args.This());
	self->MakeWeak();

	return args.This();
}

void Document::Worker(uv_async_s *handle, int status) {
	Document *self = (Document *)(handle->data);
	uv_mutex_lock(&self->jobMutex);
	if(self->jobs.size() != 0) {
		puts("foo");
		PageJob *pj = self->jobs.front();
		uv_mutex_unlock(&self->jobMutex);
		pj->run();

		uv_mutex_lock(&self->jobMutex);
		self->jobs.pop();
		self->finishedJobs.push(pj);
		uv_mutex_unlock(&self->jobMutex);

		uv_async_send(&self->v8Message);
	}
	else {
		uv_mutex_unlock(&self->jobMutex);
		uv_close((uv_handle_t*) &self->bgMessage, NULL);
	}
}

void Document::Receiver(uv_async_t *handle, int status) {
	HandleScope scope;
	Document *self = (Document *)(handle->data);
	uv_mutex_lock(&self->jobMutex);
	if(self->jobs.size() != 0) {
		PageJob *pj = self->jobs.front();
		Chunk data = pj->data.front();
		pj->data.pop();
		uv_mutex_unlock(&self->jobMutex);

		Local<Value> argv[] = {
			Local<String>::New(String::New("data")),
			Local<Object>::New(Buffer::New(data.value, data.length)->handle_)
		};
		TryCatch try_catch;
		Local<Function> emit = Function::Cast(*pj->handle_->Get(String::NewSymbol("emit")));
		emit->Call(pj->handle_, 1, argv);
		if (try_catch.HasCaught()) {
			FatalException(try_catch);
		}
	}
	else if(self->finishedJobs.size() != 0) {
		PageJob *pj = self->finishedJobs.front();
		self->finishedJobs.pop();
		uv_mutex_unlock(&self->jobMutex);

		Local<Value> argv[] = {
			Local<String>::New(String::New("end"))
		};
		TryCatch try_catch;
		Local<Function> emit = Function::Cast(*pj->handle_->Get(String::NewSymbol("emit")));
		emit->Call(pj->handle_, 1, argv);
		if (try_catch.HasCaught()) {
			FatalException(try_catch);
		}
	}
	else {
		uv_mutex_unlock(&self->jobMutex);
		self->loaded();
	}
	while(!V8::IdleNotification()) {};
}

void Document::loaded() {
	unsigned int i;
	HandleScope scope;

	unsigned int pages = this->pages->size();
	for(i = 0; i < pages; i++) {
		std::stringstream istr;
		istr << i;
		this->handle_->Set(String::New(istr.str().c_str()),
				(*this->pages)[i]->createObject(),
				static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	}
	this->handle_->Set(String::New("length"), Local<Number>::New(Number::New(pages)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

	for(i = 0; i < LENGTH(properties); i++) {
		this->handle_->SetAccessor(String::New(properties[i]), GetProperty, 0 /* setter */, Handle<Value>(), 
				static_cast<v8::AccessControl>(v8::DEFAULT), 
				static_cast<v8::PropertyAttribute>(v8::ReadOnly)
				);
	}

	Local<Value> argv[] = {
		this->doc == NULL
			? (Local<Value>)String::New("Error loading PDF")
			: Local<Value>::New(Null()),
		Local<Object>::New(this->handle_)
	};
	TryCatch try_catch;
	(*this->loadCb)->Call(Context::GetCurrent()->Global(), 2, argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
	this->loadCb.Dispose();
}

Handle<Value> Document::GetProperty(Local< String > property, const AccessorInfo &info) {
	HandleScope scope;
	Document* self = ObjectWrap::Unwrap<Document>(info.This());
	char *key = str2chr(property);
	GParamSpec *spec = g_object_class_find_property(G_OBJECT_GET_CLASS(self->doc), key);
	GValue gvalue = { 0 };
	g_value_init(&gvalue, spec->value_type);
	g_object_get_property (G_OBJECT (self->doc), key, &gvalue);

	Local<Value> val = Local<Value>::New(Null());
	const char *cValue = NULL;
	int iValue;
	//printf("%s %i\n", key, spec->value_type);
	switch(spec->value_type) {
	case G_TYPE_BOOLEAN:
		val = Local<Value>::New(Boolean::New(g_value_get_boolean(&gvalue)));
		break;
	case G_TYPE_STRING:
		cValue = g_value_get_string (&gvalue);;
		val = Local<Value>::New(cValue
			? String::New(cValue)
			: Null());
		break;
	case G_TYPE_INT:
		iValue = g_value_get_int(&gvalue);
		val = Local<Value>::New(iValue >= 0
			? Number::New(iValue)
			: Null());
		break;
	default:
		iValue = g_value_get_int(&gvalue);
		if(spec->value_type == POPPLER_TYPE_PERMISSIONS) {
			Local<Object> o = Object::New();
			o->Set(String::NewSymbol("print"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_PRINT)));
			o->Set(String::NewSymbol("modify"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_MODIFY)));
			o->Set(String::NewSymbol("copy"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_COPY)));
			o->Set(String::NewSymbol("notes"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_ADD_NOTES)));
			o->Set(String::NewSymbol("fillForm"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_FILL_FORM)));
			val = o;
		}
		else {
			if(spec->value_type == POPPLER_TYPE_PAGE_LAYOUT) {
				cValue = pageLayouts[iValue];
			}
			else if(spec->value_type == POPPLER_TYPE_PAGE_MODE) {
				cValue = pageModes[iValue];
			}
			val = Local<Value>::New(cValue
				? String::New(cValue)
				: Null());
		}
	}
	g_value_unset(&gvalue);
	return scope.Close(Local<Value>::New(val));
}
