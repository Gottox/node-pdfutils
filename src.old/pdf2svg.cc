#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "pdf2svg.h"

using namespace v8;
using namespace node;

void pdf2svg_openJob(uv_work_t *req) {
	struct Document *doc = req->data;

	poppler_document_new_from_data
	printf("Hello World\n");
}

