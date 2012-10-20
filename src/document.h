#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <queue>
#include <node.h>
#include <poppler.h>
#include "page.h"
#include "page_job.h"


class Document : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		v8::Persistent<v8::Function> loadCb;
		PopplerDocument *doc;
		std::vector<Page*> *pages;
		std::queue<PageJob*> jobs;
		uv_mutex_t jobMutex;
		uv_async_t bgMessage;
		uv_async_t v8Message;

	private:
		bool bgInitiated;
		int buflen;
		char *buffer;
		uv_thread_t worker;
		uv_loop_t *bgLoop;

		std::queue<PageJob*> finishedJobs;
		Document(char *buffer, const int length, v8::Persistent<v8::Function> &loadCb);
		~Document();

		void loaded();
		
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetProperty(v8::Local< v8::String > property, const v8::AccessorInfo &info);

		static void WorkerInit(void *arg);
		static void Worker(uv_async_s *handle, int status);
		static void Receiver(uv_async_t *handle, int status);
};

#endif
