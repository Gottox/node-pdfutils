#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <queue>
#include <node.h>
#include <poppler.h>
#include "page.h"
#include "page_job.h"
#include "util.h"


enum WorkerState {
	WORKER_INIT,
	WORKER_INACTIVE,
	WORKER_STARTING,
	WORKER_PROCESSING,
	WORKER_STOPPING
};

class Document : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		void addJob(PageJob *job);
		PopplerDocument *doc;

	private:
		v8::Persistent<v8::Function> loadCb;
		std::vector<Page*> *pages;
		std::queue<PageJob*> jobs;
		uv_mutex_t jobMutex;
		bool needMessage;
		uv_mutex_t stateMutex;
		WorkerState state;
		uv_work_t worker;
		

		v8::Persistent<v8::Object> jsbuffer;
		char *buffer;
		int buflen;

		Document(v8::Persistent<v8::Object> &buffer, v8::Persistent<v8::Function> &loadCb);
		~Document();

		void loaded();
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		v8::Handle<v8::Value> getProperty(const char *key);
		
		static void BackgroundLoad(uv_work_t* req);
		static void BackgroundLoaded(uv_work_t* req);

		static void Worker(uv_work_t *handle);
		static void WorkerClean(uv_work_t *handle);
};

#endif
