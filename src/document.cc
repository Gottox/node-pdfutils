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

Document::Document(char *buffer, const int buflen, Persistent<Function>& loadCb) {
	this->buffer = buffer;
	this->buflen = buflen;
	this->loadCb = loadCb;
}

Document::~Document() {
	uv_close((uv_handle_t*) &this->v8Message, NULL);
	uv_close((uv_handle_t*) &this->bgMessage, NULL);
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

	//uv_run(self->bgLoop);
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
	//Document *self = (Document *)(handle->data);
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
	this->handle_->Set(String::New("author"), Local<Value>::New(this->author ? String::New(this->author) : Null()), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->handle_->Set(String::New("title"), Local<Value>::New(this->title ? String::New(this->title) : Null()), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	this->handle_->Set(String::New("subject"), Local<Value>::New(this->subject ? String::New(this->subject) : Null()), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

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
