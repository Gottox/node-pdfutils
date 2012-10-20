#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <poppler.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include "page.h"
#include "page_job.h"
#include "document.h"

using namespace v8;
using namespace node;


PageJob::PageJob(Page &page, Format format) {
	this->page = &page;
	this->format = format;
}

PageJob::~PageJob() {};

void PageJob::Init(Handle<Object> target) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New();
	tpl->SetClassName(String::NewSymbol("PDFPage"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	//Local< ObjectTemplate > prt = tpl->PrototypeTemplate();

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("PageJob"), constructor);
}


void PageJob::BackgroundConvert(uv_work_t *req) {
	PageJob *self = (PageJob *)(req->data);
	cairo_surface_t *surface;
	cairo_t *cr;

	if (self->page->pg == NULL) {
		return; 
	}   

	if(self->format == FORMAT_SVG)
		surface = cairo_svg_surface_create_for_stream(PageJob::BackgroundConverting, req, self->page->w, self->page->h);
	else
		surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, self->page->w, self->page->h);

	cr = cairo_create(surface);
	poppler_page_render(self->page->pg, cr);
	if(self->format == FORMAT_PNG)
		cairo_surface_write_to_png_stream(surface, PageJob::BackgroundConverting, req);
	cairo_show_page(cr);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	g_object_unref(self->page);
	if(cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
	
	}
}

cairo_status_t PageJob::BackgroundConverting(void *closure, const unsigned char *data, unsigned int length) {
	//uv_work_t *req = (uv_work_t *)closure;

	return CAIRO_STATUS_SUCCESS;
}

void PageJob::BackgroundConverted(uv_work_t *req) {
/*	HandleScope scope;
	uv_unref((uv_handle_t*) req);
	PageJob *self = (PageJob *)(req->data);
	Local<Value> argv[] = {
		self->document == NULL
			? (Local<Value>)String::New("Error loading PDF")
			: Local<Value>()
	};
	TryCatch try_catch;
	doc->loadCb->Call(Context::GetCurrent()->Global(), 1, argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
	doc->loadCb.Dispose();*/

}
