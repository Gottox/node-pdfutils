#include <v8.h>
#include <poppler.h>

using namespace v8;

struct Document {
	char *filename;
	char *buffer;
	int bufferlen;
	PopplerDocument *pdoc;
	Persistent<Function> openCb;
};

void pdf2svg_openDocument(uv_work_t *req);
