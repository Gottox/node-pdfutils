#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <limits>
#include <algorithm>
#include <cairo.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <string.h>
#include <uv.h>
#include "page.h"
#include "page_job.h"
#include "document.h"

using namespace v8;
using namespace node;
using namespace std;

#define LOCK_CHUNK(x) uv_mutex_lock(&x->chunkMutex)
#define UNLOCK_CHUNK(x) uv_mutex_unlock(&x->chunkMutex)

Persistent<Function> PageJob::constructor;

void PageJob::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPageJob"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("PageJob"), constructor);
}

PageJob::PageJob(Page &page, Format format) {
	this->page = &page;
	this->format = format;
	this->w = page.w;
	this->h = page.h;

	if(format == FORMAT_SVG)
		this->sizeHack = new SvgSizeHack();
	else
		this->sizeHack = NULL;


	Handle<Value> argv[] = {
		this->page->handle_
	};
	Persistent<Object> instance = Persistent<Object>::New((*constructor)->NewInstance(LENGTH(argv), argv));
	instance->SetAccessor(String::NewSymbol("height"), PageJob::GetDimension, 0 /* setter */, Handle<Value>(), 
			static_cast<v8::AccessControl>(DEFAULT), 
			static_cast<v8::PropertyAttribute>(ReadOnly)
			);
	instance->SetAccessor(String::NewSymbol("width"), PageJob::GetDimension, 0 /* setter */, Handle<Value>(), 
			static_cast<v8::AccessControl>(DEFAULT), 
			static_cast<v8::PropertyAttribute>(ReadOnly)
			);
	instance->Set(String::NewSymbol("page"), page.handle_, 
			static_cast<v8::PropertyAttribute>(v8::ReadOnly)); 


	uv_loop_t *loop = uv_default_loop();
	uv_async_init(loop, &this->message_chunk, PageJob::ChunkCompleted);
	uv_async_init(loop, &this->message_finished, PageJob::JobCompleted);
	this->message_finished.data = this->message_chunk.data = this;
	uv_mutex_init(&this->chunkMutex);

	this->Wrap(instance);
}

Handle<Value> PageJob::GetDimension(Local<String> property, const AccessorInfo &info) {
	HandleScope scope;
	PageJob* self = ObjectWrap::Unwrap<PageJob>(info.This());

	char *p = *String::Utf8Value(property);
	Local<Value> result;
	
	if(strcmp("width", p) == 0)
		result = Number::New(self->w);
	else if(strcmp("height", p) == 0)
		result = Number::New(self->h);
	
	return scope.Close(result);
}

PageJob::~PageJob() {
	this->handle_.Dispose();
}

void PageJob::calcDimensions(Local<Object> opt) {
	Local<String> s_maxWidth = String::NewSymbol("maxWidth");
	Local<String> s_minWidth = String::NewSymbol("minWidth");
	Local<String> s_maxHeight = String::NewSymbol("maxHeight");
	Local<String> s_minHeight = String::NewSymbol("minHeight");
	Local<String> s_height = String::NewSymbol("height");
	Local<String> s_width = String::NewSymbol("width");
	double maxHeight = -1.0;
	double maxWidth = -1.0;
	double minHeight = 1.0;
	double minWidth = 1.0;
	double aspect = this->w / this->h;

	if(opt->Has(s_maxWidth))
		maxWidth = max(1.0, opt->Get(s_maxWidth)->ToNumber()->Value());
	if(opt->Has(s_maxHeight))
		maxHeight = max(1.0, opt->Get(s_maxHeight)->ToNumber()->Value());
	if(opt->Has(s_minWidth))
		minWidth = max(1.0, opt->Get(s_minWidth)->ToNumber()->Value());
	if(opt->Has(s_minHeight))
		minHeight = max(1.0, opt->Get(String::NewSymbol("minHeight"))->ToNumber()->Value());
	if(opt->Has(s_width))
		maxWidth = minWidth = max(1.0, opt->Get(s_width)->ToNumber()->Value());
	if(opt->Has(s_height))
		maxHeight = minHeight = max(1.0, opt->Get(s_height)->ToNumber()->Value());


	double neverReached = minHeight * minWidth * max(1.0, maxHeight) * max(1.0, maxWidth) * this->h * this->w;
	if(maxHeight <= 0.0)
		maxHeight = neverReached;
	if(maxWidth <= 0.0)
		maxHeight = neverReached;

	for(int i = 0; true; i++) {
		if (this->w > maxWidth) {
			this->w = maxWidth;
			this->h = max(maxWidth / aspect, minHeight);
		}
		else if (this->w < minWidth) {
			this->w = minWidth;
			this->h = min(minWidth / aspect, maxHeight);
		}

		if(i)
			break;

		if (this->h > maxHeight){
			this->h = maxHeight;
			this->w = max(maxHeight * aspect, minWidth);
		}
		else if (this->h < minHeight){
			this->h = minHeight;
			this->w = min(minHeight * aspect, maxWidth);
		}
	}
}

void PageJob::toSVG() {
	cairo_surface_t *surface;

	surface = cairo_svg_surface_create_for_stream(PageJob::ProcChunk, this, this->w, this->h);

	this->draw(surface);

	cairo_surface_destroy(surface);
}

void PageJob::toPNG() {
	cairo_surface_t *surface;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->w, this->h);

	this->draw(surface);

	cairo_surface_write_to_png_stream(surface, PageJob::ProcChunk, this);

	cairo_surface_destroy(surface);
}

void PageJob::toPDF() {
	cairo_surface_t *surface;

	surface = cairo_pdf_surface_create_for_stream(PageJob::ProcChunk, this, this->w, this->h);

	this->draw(surface);

	cairo_surface_destroy(surface);
}


void PageJob::toText() {
	unsigned char *text = (unsigned char *)poppler_page_get_text(this->page->pg);
	ProcChunk(this, text, strlen((char *)text));
}

void PageJob::draw(cairo_surface_t *surface) {
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_rectangle(cr, 0, 0, this->w, this->h);
	cairo_fill(cr);
	cairo_scale(cr, this->w/this->page->w, this->h/this->page->h);
	poppler_page_render(this->page->pg, cr);
	if(cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
		// TODO ERROR
	}
	cairo_show_page(cr);

	cairo_destroy(cr);
}

void PageJob::run() {
	if (this->page->pg == NULL) {
		// TODO ERROR
		return; 
	}   

	switch(this->format) {
	case FORMAT_SVG:
		this->toSVG();
		break;
	case FORMAT_PDF:
		this->toPDF();
		break;
	case FORMAT_PNG:
		this->toPNG();
		break;
	case FORMAT_TEXT:
		this->toText();
		break;
	default:
		// TODO ERROR
		break;
	}

	uv_async_send(&this->message_finished);
}

cairo_status_t PageJob::ProcChunk(void *closure, const unsigned char *data, unsigned int length) {
	PageJob *self = (PageJob *)closure;

	Chunk *chunk = new Chunk;
	chunk->value = new char[length];
	chunk->length = length;
	memcpy(chunk->value, data, length);

	if(self->sizeHack != NULL) {
		self->sizeHack->parse(chunk->value, length);
		if(self->sizeHack->finished) {
			self->sizeHack = NULL;
		}
	}

	LOCK_CHUNK(self);
	self->chunks.push(chunk);
	UNLOCK_CHUNK(self);
	uv_async_send(&self->message_chunk);

	return CAIRO_STATUS_SUCCESS;
}

void PageJob::ChunkCompleted(uv_async_t* handle, int status) {
	PageJob *self = (PageJob *)(handle->data);
	HandleScope scope;

	LOCK_CHUNK(self);
	while(self->chunks.empty() == false) {
		Chunk *chunk = self->chunks.front();
		self->chunks.pop();
		UNLOCK_CHUNK(self);

		Buffer *buffer = Buffer::New(chunk->length);
		memcpy(Buffer::Data(buffer->handle_), chunk->value, chunk->length);
		delete[] chunk->value;
		delete chunk;
		Local<Value> argv[] = {
			Local<String>::New(String::New("data")),
			Local<Object>::New(buffer->handle_),
			Local<Object>::New(self->page->handle_)
		};
		TryCatch try_catch;
		Local<Function> emit = Function::Cast(*self->handle_->Get(String::NewSymbol("emit")));
		emit->Call(self->handle_, LENGTH(argv), argv);

		if (try_catch.HasCaught()) {
			FatalException(try_catch);
		}

		LOCK_CHUNK(self);
	}
	UNLOCK_CHUNK(self);
}

void PageJob::JobCompleted(uv_async_t* handle, int status) {
	PageJob *self = (PageJob *)(handle->data);
	HandleScope scope;

	PageJob::ChunkCompleted(&self->message_chunk, status);

	Local<Value> argv[] = {
		Local<String>::New(String::New("end")),
		Local<Object>::New(self->page->handle_)
	};

	TryCatch try_catch;
	Local<Function> emit = Function::Cast(*self->handle_->Get(String::NewSymbol("emit")));
	emit->Call(self->handle_, LENGTH(argv), argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}

	self->MakeWeak();
	uv_close((uv_handle_t*) &self->message_chunk, NULL);
	uv_close((uv_handle_t*) &self->message_finished, NULL);
}

