#ifndef PAGE_JOB_H
#define PAGE_JOB_H

#include <node.h>
#include <poppler.h>
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

	private:
		PageJob(Page &page, Format format);
		~PageJob();

		static void BackgroundConvert(uv_work_t *req);

		static cairo_status_t BackgroundConverting(void *closure, const unsigned char *data, unsigned int length);

		static void BackgroundConverted(uv_work_t *req);
};

#endif
