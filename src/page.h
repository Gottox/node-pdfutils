
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
		Page(Document *document, int index);
		void createObject();
		double w;
		double h;

	private:
		~Page();
		v8::Persistent<v8::Array> links;

		static v8::Handle<v8::Value> GetLinks(v8::Local<v8::String> property, const v8::AccessorInfo &info);

		static v8::Handle<v8::Value> ConvertTo(const v8::Arguments& args);

		static v8::Persistent<v8::Function> constructor;

		int index;
		char *label;
};

#endif
