#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <limits>
#include <algorithm>
#include <cairo.h>
#include <cairo-svg.h>
#include "page.h"
#include "page_job.h"
#include "document.h"

using namespace v8;
using namespace node;
using namespace std;

Persistent<Function> PageJob::constructor;

PageJob::PageJob(Page &page, Format format) {
	this->page = &page;
	this->format = format;
	this->w = page.w;
	this->h = page.h;

	this->sizeHack = format == FORMAT_SVG ? new SvgSizeHack() : NULL;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = {
		this->page->handle_
	};
	Persistent<Object> instance = Persistent<Object>::New((*constructor)->NewInstance(argc, argv));
	this->Wrap(instance);
}

PageJob::~PageJob() {
	this->handle_.Dispose();
}

void PageJob::calcDimensions(Local<Object> opt) {
	double maxHeight = -1.0;
	double maxWidth = -1.0;
	double minHeight = 1.0;
	double minWidth = 1.0;
	double aspect = this->w / this->h;

	if(opt->Has(String::NewSymbol("maxWidth")))
		maxWidth = max(1.0, opt->Get(String::NewSymbol("maxWidth"))->ToNumber()->Value());
	if(opt->Has(String::NewSymbol("maxHeight")))
		maxHeight = max(1.0, opt->Get(String::NewSymbol("maxHeight"))->ToNumber()->Value());
	if(opt->Has(String::NewSymbol("minWidth")))
		minWidth = max(1.0, opt->Get(String::NewSymbol("minWidth"))->ToNumber()->Value());
	if(opt->Has(String::NewSymbol("minHeight")))
		minHeight = max(1.0, opt->Get(String::NewSymbol("minHeight"))->ToNumber()->Value());
	if(opt->Has(String::NewSymbol("width")))
		maxWidth = minWidth = max(1.0, opt->Get(String::NewSymbol("width"))->ToNumber()->Value());
	if(opt->Has(String::NewSymbol("height")))
		maxHeight = minHeight = max(1.0, opt->Get(String::NewSymbol("height"))->ToNumber()->Value());


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

void PageJob::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPageJob"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("PageJob"), constructor);
}

void PageJob::toSVG() {
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_svg_surface_create_for_stream(PageJob::ProcChunk, this, this->w, this->h);

	cr = cairo_create(surface);
	cairo_scale(cr, this->w/this->page->w, this->h/this->page->h);
	poppler_page_render(this->page->pg, cr);
	cairo_show_page(cr);

	if(cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
		// TODO ERROR
	}
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

void PageJob::toPNG() {
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->w, this->h);

	cr = cairo_create(surface);
	cairo_scale(cr, this->w/this->page->w, this->h/this->page->h);
	poppler_page_render(this->page->pg, cr);
	if(this->format == FORMAT_PNG)
		cairo_surface_write_to_png_stream(surface, PageJob::ProcChunk, this);
	cairo_show_page(cr);

	if(cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
		// TODO ERROR
	}
	cairo_destroy(cr);
	cairo_surface_destroy(surface);

}

void PageJob::toText() {
	char *text = poppler_page_get_text(this->page->pg);
	this->page->document->addChunk(this, (unsigned char*)text, strlen(text) + 1);
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
}

cairo_status_t PageJob::ProcChunk(void *closure, const unsigned char *data, unsigned int length) {
	PageJob *self = (PageJob *)closure;

	if(self->sizeHack != NULL) {
		unsigned char *dt = new unsigned char[length];
		memcpy(dt, data, length);
		self->sizeHack->parse(dt, length);
		data = dt;
		if(self->sizeHack->finished)
			self->sizeHack = NULL;
	}

	self->page->document->addChunk(self, data, length);

	return CAIRO_STATUS_SUCCESS;
}

void PageJob::done() {
	this->MakeWeak();
}
