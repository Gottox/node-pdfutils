#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <glib.h>
#include "document.h"

using namespace v8;

void InitAll(Handle<Object> target) {
	g_type_init();
	Document::Init(target);
	Page::Init(target);
	PageJob::Init(target);
}

NODE_MODULE(pdfutils, InitAll)
