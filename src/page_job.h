#ifndef PAGE_JOB_H
#define PAGE_JOB_H

#include <node.h>
#include <poppler.h>
#include <cairo.h>
#include <queue>
#include "formats.h"
#include "svg_size_hack.h"

class Document;

struct Chunk {
	char *value;
	int length;
};

class PageJob : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		static v8::Persistent<v8::Function> constructor;
		void calcDimensions(v8::Local<v8::Object> opt);

		PageJob(Page &page, Format format);
		~PageJob();

		void run();
		void done();

	private:
		std::queue<Chunk*> chunks;
		uv_mutex_t chunkMutex;

		uv_async_t message_finished;
		uv_async_t message_chunk;

		SvgSizeHack *sizeHack;
		double w;
		double h;

		static cairo_status_t ProcChunk(void *closure, const unsigned char *data, unsigned int length);
		static void ChunkCompleted(uv_async_t* handle, int status);
		static void JobCompleted(uv_async_t* handle, int status);
		static v8::Handle<v8::Value> GetDimension(v8::Local<v8::String> property, const v8::AccessorInfo &info);

		void toPNG();
		void toSVG();
		void toPDF();
		void toText();
		void draw(cairo_surface_t *surface);
		Format format;
		Page *page;
};

#endif
