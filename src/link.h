#ifndef LINK_H
#define LINK_H

#include <node.h>
#include <poppler.h>
#include "page.h"

class Document;

class Link : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		Page *page;

		Link(Page *page, PopplerLinkMapping *link);


	private:
		~Link();

		static v8::Persistent<v8::Function> constructor;

		void createObject();

		PopplerAction *action;
		double x;
		double y;
		double w;
		double h;
		char *title;
		void fillGoto(v8::Local<v8::Object> &dest);
		void fillRemote(v8::Local<v8::Object> &dest);
		void fillLaunch(v8::Local<v8::Object> &dest);
		void fillUri(v8::Local<v8::Object> &dest);
		void fillNamed(v8::Local<v8::Object> &dest);
		void fillMovie(v8::Local<v8::Object> &dest);
		void fillRendition(v8::Local<v8::Object> &dest);
		void fillOCGState(v8::Local<v8::Object> &dest);
		void fillJavascript(v8::Local<v8::Object> &dest);

		void fillDest(v8::Local<v8::Object> &dest, PopplerDest *d);
};

#endif
