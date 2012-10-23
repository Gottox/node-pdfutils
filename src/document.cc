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

#define LOCK_JOB(x) uv_mutex_lock(&x->jobMutex)
#define UNLOCK_JOB(x) uv_mutex_unlock(&x->jobMutex)
//#define LOCK_JOB(x) printf("Job lock %i\n", __LINE__);uv_mutex_lock(&x->jobMutex);printf("Job got %i\n", __LINE__)
//#define UNLOCK_JOB(x) printf("Job unlock %i\n", __LINE__);uv_mutex_unlock(&x->jobMutex)

#define LOCK_CHUNK(x) uv_mutex_lock(&x->chunkMutex)
#define UNLOCK_CHUNK(x) uv_mutex_unlock(&x->chunkMutex)
//#define LOCK_CHUNK(x) printf("Chunk lock %i\n", __LINE__);uv_mutex_lock(&x->chunkMutex);printf("Chunk got %i\n", __LINE__);
//#define UNLOCK_CHUNK(x) printf("Chunk unlock %i\n", __LINE__);uv_mutex_unlock(&x->chunkMutex)

#define TRY_MESSAGE(x) (uv_sem_trywait(&x->messageSem) == 0)
#define LOCK_MESSAGE(x) uv_sem_wait(&x->messageSem)
#define UNLOCK_MESSAGE(x) uv_sem_post(&x->messageSem)

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

Document::Document(Persistent<Object> &buffer, Persistent<Function>& loadCb) {
	this->jsbuffer = buffer;
	this->buffer = Buffer::Data(buffer);
	this->buflen = Buffer::Length(buffer);
	this->loadCb = loadCb;
}

Document::~Document() {
	LOCK_JOB(this);
	while(this->finishedJobs.size())
		this->finishedJobs.pop();
	while(this->jobs.size())
		this->jobs.pop();
	UNLOCK_JOB(this);

	uv_mutex_destroy(&this->jobMutex);

	uv_mutex_destroy(&this->chunkMutex);

	uv_sem_destroy(&this->messageSem);

	this->handle_.Dispose();
	this->jsbuffer.Dispose();

}

void Document::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("PDF"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Document"), constructor);
}

Handle<Value> Document::New(const Arguments& args) {
	HandleScope scope;

	Document* self;
	
	if(args.Length() < 2)
		return ThrowException(Exception::Error(String::New("constructor needs two arguments")));
	else if(!Buffer::HasInstance(args[0]))
		return ThrowException(Exception::Error(String::New("first argument must be a Buffer")));
	else if(!args[1]->IsFunction())
		return ThrowException(Exception::Error(String::New("second argument must be a Function")));

	Persistent<Object> buffer = Persistent<Object>::New(args[0]->ToObject());
	Persistent<Function> cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
	self = new Document(buffer, cb);

	self->worker.data = self->message_finished.data = self->message_data.data = self;
	uv_sem_init(&self->messageSem, 1);
	uv_mutex_init(&self->jobMutex);
	uv_mutex_init(&self->chunkMutex);


	uv_queue_work(uv_default_loop(), &self->worker, Document::BackgroundLoad, Document::BackgroundLoaded);

	self->Wrap(Persistent<Object>::New(args.This()));

	return args.This();
}

void  Document::BackgroundLoad(uv_work_t* handle) {
	Document *self = (Document *)(handle->data);
	int i = 0;
	self->doc = poppler_document_new_from_data(self->buffer, self->buflen, NULL, NULL);
	int pages = poppler_document_get_n_pages(self->doc);

	self->pages = new std::vector<Page*>();
	for(i = 0; i < pages; i++) {
		self->pages->push_back(new Page(*self, i));
	}
}

void Document::BackgroundLoaded(uv_work_t* handle) {
	Document *self = (Document *)(handle->data);
	unsigned int i;
	HandleScope scope;

	unsigned int pages = self->pages->size();
	for(i = 0; i < pages; i++) {
		std::stringstream istr;
		istr << i;
		self->handle_->Set(String::New(istr.str().c_str()),
				(*self->pages)[i]->createObject(),
				static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 
	}
	self->handle_->Set(String::New("length"), Local<Number>::New(Number::New(pages)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

	for(i = 0; i < LENGTH(properties); i++) {
		self->handle_->SetAccessor(String::New(properties[i]), GetProperty, 0 /* setter */, Handle<Value>(), 
				static_cast<v8::AccessControl>(v8::DEFAULT), 
				static_cast<v8::PropertyAttribute>(v8::ReadOnly)
				);
	}

	Local<Value> argv[] = {
		self->doc == NULL
			? (Local<Value>)String::New("Error loading PDF")
			: Local<Value>::New(Null()),
		Local<Object>::New(self->handle_)
	};
	TryCatch try_catch;
	(*self->loadCb)->Call(Context::GetCurrent()->Global(), 2, argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
	self->loadCb.Dispose();
}

void Document::addJob(PageJob *job) {
	LOCK_JOB(this);
	this->jobs.push(job);
	if(this->jobs.size() != 1) {
		UNLOCK_JOB(this);
	}
	else {
		this->handle_.ClearWeak();
		uv_loop_t *loop = uv_default_loop();
		if(TRY_MESSAGE(this)) {
			uv_async_init(loop, &this->message_finished, Document::WorkerFinished);
			uv_async_init(loop, &this->message_data, Document::WorkerChunk);
			UNLOCK_MESSAGE(this);
		}
		else {
			LOCK_MESSAGE(this);
			this->needMessage = true;
			UNLOCK_JOB(this);
		}
		UNLOCK_JOB(this);
		uv_queue_work(loop, &this->worker, Document::Worker, Document::WorkerClean);
	}
}

void Document::Worker(uv_work_t *handle) {
	Document *self = (Document *)(handle->data);

	LOCK_JOB(self);
	while(self->jobs.size()) {
		PageJob *pj = self->jobs.front();
		UNLOCK_JOB(self);

		pj->run();

		LOCK_JOB(self);
		self->jobs.pop();
		self->finishedJobs.push(pj);
		uv_async_send(&self->message_finished);
	}
	LOCK_MESSAGE(self);
	self->needMessage = false;
	UNLOCK_JOB(self);
}

void Document::WorkerFinished(uv_async_t *handle, int status /*UNUSED*/) {
	Document *self = (Document *)(handle->data);
	HandleScope scope;

	Document::WorkerChunk(handle, status);
	LOCK_JOB(self);
	while(self->finishedJobs.size()) {
		PageJob *pj = self->finishedJobs.front();
		self->finishedJobs.pop();
		UNLOCK_JOB(self);

		pj->done();
		Local<Value> argv[] = {
			Local<String>::New(String::New("end")),
			Local<Object>::New(pj->page->handle_)
		};
		TryCatch try_catch;
		Local<Function> emit = Function::Cast(*pj->handle_->Get(String::NewSymbol("emit")));
		emit->Call(pj->handle_, LENGTH(argv), argv);
		if (try_catch.HasCaught()) {
			FatalException(try_catch);
		}
		LOCK_JOB(self);
	}
	UNLOCK_JOB(self);
}

void Document::addChunk(PageJob *job, const unsigned char* data, unsigned int length) {
	Chunk *chunk = new Chunk;
	chunk->value = new char[length];
	memcpy(chunk->value, data, length);
	chunk->length = length;
	chunk->pj = job;

	LOCK_CHUNK(this);
	this->chunks.push(chunk);
	UNLOCK_CHUNK(this);
	uv_async_send(&this->message_data);
}

void Document::WorkerChunk(uv_async_t *handle, int status /*UNUSED*/) {
	Document *self = (Document *)(handle->data);
	HandleScope scope;

	LOCK_CHUNK(self);
	while(self->chunks.size()) {
		Chunk *chunk = self->chunks.front();
		self->chunks.pop();
		UNLOCK_CHUNK(self);
		PageJob *pj = chunk->pj;

		Local<Value> argv[] = {
			Local<String>::New(String::New("data")),
			Local<Object>::New(Buffer::New(chunk->value, chunk->length)->handle_),
			Local<Object>::New(pj->page->handle_)
		};
		TryCatch try_catch;
		Local<Function> emit = Function::Cast(*pj->handle_->Get(String::NewSymbol("emit")));
		emit->Call(pj->handle_, LENGTH(argv), argv);

		if (try_catch.HasCaught()) {
			FatalException(try_catch);
		}
		LOCK_CHUNK(self);
	}
	UNLOCK_CHUNK(self);
}

void Document::WorkerClean(uv_work_t *handle) {
	Document *self = (Document *)(handle->data);
	HandleScope scope;
	UNLOCK_MESSAGE(self);

	WorkerChunk(&self->message_data, 0);
	WorkerFinished(&self->message_finished, 0);

	while(!V8::IdleNotification()) {};

	if(self->needMessage == false) {
		uv_close((uv_handle_t*) &self->message_data, NULL);
		uv_close((uv_handle_t*) &self->message_finished, NULL);
		self->MakeWeak();
	}
}


Handle<Value> Document::GetProperty(Local< String > property, const AccessorInfo &info) {
	HandleScope scope;
	return scope.Close(Local<Value>::New(Null()));
	Document* self = ObjectWrap::Unwrap<Document>(info.This());
	char *key = str2chr(property);
	GParamSpec *spec = g_object_class_find_property(G_OBJECT_GET_CLASS(self->doc), key);
	GValue gvalue;
	g_value_init(&gvalue, spec->value_type);
	g_object_get_property (G_OBJECT (self->doc), key, &gvalue);

	Local<Value> val = Local<Value>::New(Null());
	const char *cValue = NULL;
	int iValue;
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
		if(spec->value_type == POPPLER_TYPE_PERMISSIONS) {
			iValue = g_value_get_flags(&gvalue);

			Local<Object> o = Object::New();
			o->Set(String::NewSymbol("print"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_PRINT)));
			o->Set(String::NewSymbol("modify"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_MODIFY)));
			o->Set(String::NewSymbol("copy"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_COPY)));
			o->Set(String::NewSymbol("notes"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_ADD_NOTES)));
			o->Set(String::NewSymbol("fillForm"), Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_FILL_FORM)));
			val = o;
		}
		else {
			iValue = g_value_get_enum(&gvalue);

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
