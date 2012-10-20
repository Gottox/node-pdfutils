#ifndef PAGE_JOB_H
#define PAGE_JOB_H

#include <node.h>
#include <poppler.h>
#include <cairo.h>
#include <queue>
#include "formats.h"

class Document;
struct Chunk {
	char *value;
	int length;
};

class PageJob : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);

		static v8::Persistent<v8::Function> constructor;
		v8::Persistent<v8::Function> cb;
		Page *page;
		Format format;
		std::queue<Chunk> data;
		uv_mutex_t dataMutex;
		PageJob(Page &page, Format format);
		~PageJob();

		void run();

	private:
		static cairo_status_t ProcChunk(void *closure, const unsigned char *data, unsigned int length);
};

#endif
