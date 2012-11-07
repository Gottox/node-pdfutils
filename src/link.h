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

		PopplerAction *action;
		double x;
		double y;
		double w;
		double h;
		char *title;

	private:
		~Link();

		static v8::Persistent<v8::Function> constructor;

		void createObject();

};

#endif
