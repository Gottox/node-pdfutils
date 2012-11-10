#include <node.h>
#include <glib.h>
#include "document.h"
#include "link.h"

using namespace v8;

void InitAll(Handle<Object> target) {
	g_type_init();
	Document::Init(target);
	Page::Init(target);
	PageJob::Init(target);
	Link::Init(target);
}

NODE_MODULE(pdfutils, InitAll)
