
#ifndef PAGE_H
#define PAGE_H

#include <node.h>
#include <poppler.h>

class Document;

class Page : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		static v8::Persistent<v8::Function> constructor;
		Document *document;
		PopplerPage *pg;
		int index;
		double w;
		double h;
		char *label;
		Page(Document &document, int index);
		v8::Local<v8::Object> createObject();

	private:
		~Page();

		static v8::Handle<v8::Value> ConvertTo(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetWidth(const v8::Local<v8::String> property, const v8::AccessorInfo &info);

		static v8::Handle<v8::Value> GetHeight(const v8::Local<v8::String> property, const v8::AccessorInfo &info);
};

#endif
