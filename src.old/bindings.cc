#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include "pdf2svg.h"

using namespace v8;
using namespace node;

static inline char *val2char(Handle<Value> val) {
	String::Utf8Value utf8(val->ToString());
	int len = utf8.length() + 1;
	char *str = (char *) calloc(sizeof(char), len);
	strncpy(str, *utf8, len);

	return str;
}

static void callbackOpen(uv_work_t *req) {
	HandleScope scope;
	ev_unref(EV_DEFAULT_UC);
	struct Document *data = (struct Document *)req->data;
	Local<Value> argv[] = {
		Integer::New(data->)
	};
	TryCatch try_catch;
	data->cb->Call(Context::GetCurrent()->Global(), 1, argv);
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
	data->cb.Dispose();
	free(data);
}

Handle<Value> CreateObject(const Arguments& args) {

	HandleScope scope;

	if(args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("function needs 2 arguments")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsString() && !Buffer::HasInstance(args[0])) {
		ThrowException(Exception::TypeError(String::New("first argument must be a String or a Buffer")));
		return scope.Close(Undefined());
	}

	if(!args[1]->IsFunction()) {
		ThrowException(Exception::TypeError(String::New("second argument must be a Function")));
		return scope.Close(Undefined());
	}
	printf("aaa\n");

	Document *doc = (Document *)malloc(sizeof(struct Document));

	Local<Function> cb = Local<Function>::Cast(args[1]);
	doc->cb = Persistent<Function>::New(cb);
	if(args[0]->IsString())
		doc->filename = val2char(args[0]);
	else {
		Local<Object> buffer=args[0]->ToObject();
		doc->buffer = Buffer::Data(buffer);
		doc->bufferlen = Buffer::Length(buffer);
	}

	uv_work_t *req = (uv_work_t *) (calloc(1, sizeof(uv_work_t)));

	uv_queue_work(uv_default_loop(), req,
			pdf2svg_openDocument,
			callbackOpen);
	ev_ref(EV_DEFAULT_UC);
	return Undefined();
}

void init(Handle<Object> target) {
	target->Set(String::NewSymbol("create"),
		        FunctionTemplate::New(CreateObject)->GetFunction());
}
NODE_MODULE(pdf2svg, init)
