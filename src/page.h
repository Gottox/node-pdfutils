
#ifndef PAGE_H
#define PAGE_H

#include <node.h>
#include <poppler.h>

class Document;

class Page : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		Document *document;
		PopplerPage *pg;
		v8::Handle<v8::Object> getObject();
		Page(Document &document, int index);
		double w;
		double h;

	private:
		~Page();

		static v8::Handle<v8::Value> ConvertTo(const v8::Arguments& args);

		static v8::Persistent<v8::Function> constructor;

		v8::Handle<v8::Array> createLinks();

		int index;
		char *label;
};

#endif
