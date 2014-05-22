#include <sstream>
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <strings.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <unistd.h>
#include <strings.h>
#include "document.h"
#include "page.h"

#define LOCK_JOB(x) uv_mutex_lock(&x->jobMutex)
#define UNLOCK_JOB(x) uv_mutex_unlock(&x->jobMutex)
//#define LOCK_JOB(x) printf("Job lock %i\n", __LINE__);uv_mutex_lock(&x->jobMutex);printf("Job got %i\n", __LINE__)
//#define UNLOCK_JOB(x) printf("Job unlock %i\n", __LINE__);uv_mutex_unlock(&x->jobMutex)

#define LOCK_STATE(x) uv_mutex_lock(&x->stateMutex)
#define UNLOCK_STATE(x) uv_mutex_unlock(&x->stateMutex)
//#define TRY_MESSAGE(x) (printf("try message lock %i\n", __LINE__) && uv_sem_trywait(&x->messageSem) == 0)
//#define LOCK_MESSAGE(x) printf("message lock %i\n", __LINE__); uv_sem_wait(&x->messageSem); printf("message got %i\n", __LINE__);
//#define UNLOCK_MESSAGE(x) printf("message unlock %i\n", __LINE__); uv_sem_post(&x->messageSem)

using namespace v8;
using namespace node;
const static char *properties[][2] = {
	{ "author", "author" },
	{ "creation-date", "creationDate" },
	{ "creator", "creator" },
	{ "format", "format" },
	{ "keywords", "keywords" },
	{ "linearized", "linearized" },
	{ "metadata", "metadata" },
	{ "mod-date", "modDate" },
	{ "page-layout", "pageLayout" },
	{ "page-mode", "pageMode" },
	{ "permissions", "permissions" },
	{ "producer", "producer" },
	{ "subject", "subject" },
	{ "title", "title" },
};

inline const char *pageLayout(int id) {
	switch(id) {
		case POPPLER_PAGE_LAYOUT_SINGLE_PAGE: return "singlePage";
		case POPPLER_PAGE_LAYOUT_ONE_COLUMN: return "oneColumn";
		case POPPLER_PAGE_LAYOUT_TWO_COLUMN_LEFT: return "twoColumnLeft";
		case POPPLER_PAGE_LAYOUT_TWO_COLUMN_RIGHT: return "twoColumnRight";
		case POPPLER_PAGE_LAYOUT_TWO_PAGE_LEFT: return "twoPageLeft";
		case POPPLER_PAGE_LAYOUT_TWO_PAGE_RIGHT: return "twoPageRight";
		default: return NULL;
	}
}
inline const char *pageMode(int id) {
	switch(id) {
		case POPPLER_PAGE_MODE_NONE: return "none";
		case POPPLER_PAGE_MODE_USE_OUTLINES: return "useOutlines";
		case POPPLER_PAGE_MODE_USE_THUMBS: return "useThumbs";
		case POPPLER_PAGE_MODE_FULL_SCREEN: return "fullscreen";
		case POPPLER_PAGE_MODE_USE_OC: return "useOc";
		case POPPLER_PAGE_MODE_USE_ATTACHMENTS: return "useAttachments";
		default: return NULL;
	}
}

void Document::Init(Handle<Object> target) {


	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("PDFDocument"));
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

	self->Wrap(Persistent<Object>::New(args.This()));

	return args.This();
}

Document::Document(Persistent<Object> &buffer, Persistent<Function>& loadCb) {
	this->jsbuffer = buffer;
	this->buffer = Buffer::Data(buffer);
	this->buflen = Buffer::Length(buffer);
	this->loadCb = loadCb;
	this->state = WORKER_INIT;

	uv_mutex_init(&this->jobMutex);
	uv_mutex_init(&this->stateMutex);

	this->worker.data = this;
	uv_queue_work(uv_default_loop(), &this->worker,
			Document::BackgroundLoad,
			(uv_after_work_cb)Document::BackgroundLoaded);

}

Document::~Document() {
	LOCK_JOB(this);
	while(this->jobs.size())
		this->jobs.pop();
	UNLOCK_JOB(this);

	g_object_unref(this->doc);
	uv_mutex_destroy(&this->jobMutex);
	uv_mutex_destroy(&this->stateMutex);

	this->handle_.Dispose();
	this->jsbuffer.Dispose();

}

void  Document::BackgroundLoad(uv_work_t* handle) {
	Document *self = (Document *)(handle->data);
	unsigned int i;

	self->doc = poppler_document_new_from_data(self->buffer, self->buflen, NULL, NULL);
	self->length = poppler_document_get_n_pages(self->doc);
	self->pages = new Page *[self->length];
	for(i = 0; i < self->length; i++) {
		self->pages[i] = new Page(self, i);
	}
}

void Document::BackgroundLoaded(uv_work_t* handle) {
	Document *self = (Document *)(handle->data);
	unsigned int i;
	HandleScope scope;

	for(i = 0; i < self->length; i++) {
		std::stringstream istr;
		istr << i;
		self->handle_->SetAccessor(String::NewSymbol(istr.str().c_str()),
				Document::GetPage, 0 /* setter */, Handle<Value>(), 
				static_cast<v8::AccessControl>(DEFAULT), 
				static_cast<v8::PropertyAttribute>(ReadOnly | DontEnum)
				);
	}
	self->handle_->Set(String::New("length"), Local<Number>::New(Number::New(self->length)), 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 

	for(i = 0; i < LENGTH(properties); i++) {
		self->handle_->Set(String::New(properties[i][1]), self->getProperty(properties[i][0]),
				static_cast<v8::PropertyAttribute>(v8::ReadOnly));
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
	self->state = WORKER_INACTIVE;
}

Handle<Value> Document::GetPage(v8::Local<v8::String> property, const v8::AccessorInfo &info) {
	HandleScope scope;
	Document* self = ObjectWrap::Unwrap<Document>(info.This());

	int n = atoi(*String::Utf8Value(property));
	Page *page = self->pages[n];
	page->createObject();

	return page->handle_;
}

void Document::addJob(PageJob *job) {
	LOCK_JOB(this);
	this->jobs.push(job);
	UNLOCK_JOB(this);

	LOCK_STATE(this);
	if(this->state != WORKER_PROCESSING && this->state != WORKER_STARTING) {
		this->state = WORKER_STARTING;

		this->handle_.ClearWeak();
		uv_loop_t *loop = uv_default_loop();
		uv_queue_work(loop, &this->worker,
				Document::Worker,
				(uv_after_work_cb)Document::WorkerClean);
	}
	UNLOCK_STATE(this);
}

void Document::Worker(uv_work_t *handle) {
	Document *self = (Document *)(handle->data);

	LOCK_STATE(self);
	self->state = WORKER_PROCESSING;
	UNLOCK_STATE(self);

	LOCK_JOB(self);
	while(self->jobs.empty() == false) {
		PageJob *pj = self->jobs.front();
		self->jobs.pop();
		UNLOCK_JOB(self);

		pj->run();

		LOCK_JOB(self);
	}
	LOCK_STATE(self);
	self->state = WORKER_STOPPING;
	UNLOCK_STATE(self);

	UNLOCK_JOB(self);
}

void Document::WorkerClean(uv_work_t *handle) {
	Document *self = (Document *)(handle->data);
	HandleScope scope;

	while(!V8::IdleNotification()) {};

	//self->MakeWeak();

	LOCK_STATE(self);
	if(self->state == WORKER_STOPPING)
		self->state = WORKER_INACTIVE;
	UNLOCK_STATE(self);
}


Handle<Value> Document::getProperty(const char *key) {
	HandleScope scope;
	GValue gvalue;
	bzero(&gvalue, sizeof(GValue));
	GParamSpec *spec = g_object_class_find_property(G_OBJECT_GET_CLASS(this->doc), key);
	g_value_init(&gvalue, spec->value_type);
	g_object_get_property (G_OBJECT (this->doc), key, &gvalue);

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
			o->Set(String::NewSymbol("print"),
					Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_PRINT)));
			o->Set(String::NewSymbol("modify"),
					Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_MODIFY)));
			o->Set(String::NewSymbol("copy"),
					Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_COPY)));
			o->Set(String::NewSymbol("notes"),
					Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_ADD_NOTES)));
			o->Set(String::NewSymbol("fillForm"),
					Local<Value>::New(Boolean::New(iValue & POPPLER_PERMISSIONS_OK_TO_FILL_FORM)));
			val = o;
		}
		else {
			iValue = g_value_get_enum(&gvalue);

			if(spec->value_type == POPPLER_TYPE_PAGE_LAYOUT) {
				cValue = pageLayout(iValue);
			}
			else if(spec->value_type == POPPLER_TYPE_PAGE_MODE) {
				cValue = pageMode(iValue);
			}
			val = Local<Value>::New(cValue
				? String::New(cValue)
				: Null());
		}
	}
	g_value_unset(&gvalue);
	return scope.Close(Local<Value>::New(val));
}
